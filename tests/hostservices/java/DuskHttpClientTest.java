package dev.twilitrealm.dusk;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLStreamHandler;
import java.net.URLStreamHandlerFactory;
import java.security.Principal;
import java.security.cert.Certificate;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;

public final class DuskHttpClientTest {
    private static final Map<String, ArrayDeque<Script>> SCRIPTS = new LinkedHashMap<>();
    private static final Map<String, FakeConnection> CONNECTIONS = new LinkedHashMap<>();

    private DuskHttpClientTest() {
    }

    public static void main(String[] args) throws Exception {
        URL.setURLStreamHandlerFactory(new FakeFactory());
        testGetAndPostBytes();
        testRedirectPolicy();
        testLimitsAndDeadline();
    }

    private static void testGetAndPostBytes() {
        script("https://test.invalid/get", 204, null, new byte[] {0, 1, 2});
        DuskHttpClient.Response get = DuskHttpClient.get("https://test.invalid/get",
                new String[] {"Authorization"}, new String[] {"distinctive-secret"}, 1000, 16);
        check(get.error == DuskHttpClient.ERROR_NONE && get.statusCode == 204);
        check(Arrays.equals(get.body, new byte[] {0, 1, 2}));
        check("GET".equals(CONNECTIONS.get("https://test.invalid/get").getRequestMethod()));

        byte[] body = new byte[] {0, 3, 0, 4};
        script("https://test.invalid/post", 201, null, new byte[] {9});
        DuskHttpClient.Response post = DuskHttpClient.request(1, "https://test.invalid/post",
                new String[] {"Content-Type"}, new String[] {"application/octet-stream"}, body, 1000, 16);
        FakeConnection connection = CONNECTIONS.get("https://test.invalid/post");
        check(post.error == DuskHttpClient.ERROR_NONE && "POST".equals(connection.getRequestMethod()));
        check(Arrays.equals(body, connection.output.toByteArray()));
        check(connection.outputClosed);

        script("https://test.invalid/path@ok?q=@ok", 200, null, new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/path@ok?q=@ok", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_NONE);
        check(DuskHttpClient.get("https://user@test.invalid/no", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_INVALID_URL);
        check(DuskHttpClient.get("https://test.invalid/no#fragment", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_INVALID_URL);
    }

    private static void testRedirectPolicy() {
        for (int status : new int[] {301, 302, 303}) {
            String old = "https://test.invalid/old" + status;
            String next = "https://test.invalid/new" + status;
            script(old, status, "/new" + status, new byte[0]);
            script(next, 200, null, new byte[0]);
            DuskHttpClient.Response converted = DuskHttpClient.request(1, old,
                    new String[] {"Content-Type", "Content-Encoding", "Authorization"},
                    new String[] {"x", "identity", "same-origin"},
                    new byte[] {7}, 1000, 16);
            FakeConnection newConnection = CONNECTIONS.get(next);
            check(converted.error == DuskHttpClient.ERROR_NONE &&
                    "GET".equals(newConnection.getRequestMethod()));
            check(newConnection.output.size() == 0 &&
                    newConnection.requestHeaders.get("Content-Type") == null);
            check(newConnection.requestHeaders.get("Content-Encoding") == null);
            check("same-origin".equals(newConnection.requestHeaders.get("Authorization")));
        }

        String getOld = "https://test.invalid/get-old";
        String getNew = "https://test.invalid/get-new";
        script(getOld, 302, "/get-new", new byte[0]);
        script(getNew, 200, null, new byte[0]);
        check(DuskHttpClient.get(getOld, new String[] {"Content-Type", "Content-Encoding"},
                new String[] {"x", "identity"}, 1000, 16).error == DuskHttpClient.ERROR_NONE);
        FakeConnection redirectedGet = CONNECTIONS.get(getNew);
        check("GET".equals(redirectedGet.getRequestMethod()));
        check("x".equals(redirectedGet.requestHeaders.get("Content-Type")) &&
                "identity".equals(redirectedGet.requestHeaders.get("Content-Encoding")));

        for (int status : new int[] {307, 308}) {
            String keep = "https://test.invalid/keep" + status;
            String keptUrl = "https://other.invalid/kept" + status;
            script(keep, status, keptUrl, new byte[0]);
            script(keptUrl, 200, null, new byte[0]);
            DuskHttpClient.Response preserved = DuskHttpClient.request(1, keep,
                    new String[] {"Authorization", "Cookie"}, new String[] {"not-loggable", "not-loggable"},
                    new byte[] {5, 6}, 1000, 16);
            FakeConnection kept = CONNECTIONS.get(keptUrl);
            check(preserved.error == DuskHttpClient.ERROR_NONE &&
                    "POST".equals(kept.getRequestMethod()));
            check(Arrays.equals(new byte[] {5, 6}, kept.output.toByteArray()));
            check(kept.requestHeaders.get("Authorization") == null &&
                    kept.requestHeaders.get("Cookie") == null);
        }

        script("https://test.invalid:443/default-port", 307, "https://test.invalid/same-origin",
                new byte[0]);
        script("https://test.invalid/same-origin", 200, null, new byte[0]);
        check(DuskHttpClient.request(1, "https://test.invalid:443/default-port",
                new String[] {"Authorization"}, new String[] {"same-origin"}, new byte[] {1},
                1000, 16).error == DuskHttpClient.ERROR_NONE);
        check("same-origin".equals(CONNECTIONS.get("https://test.invalid/same-origin")
                .requestHeaders.get("Authorization")));

        script("https://test.invalid/insecure", 302, "http://network.invalid/no", new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/insecure", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_UNSUPPORTED_SCHEME);
        script("https://test.invalid/userinfo", 302, "https://user@network.invalid/no", new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/userinfo", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_UNSUPPORTED_SCHEME);
        script("https://test.invalid/fragment", 302, "https://network.invalid/no#fragment", new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/fragment", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_UNSUPPORTED_SCHEME);
        for (int i = 0; i < 5; ++i) {
            script("https://test.invalid/loop" + i, 302, "/loop" + (i + 1), new byte[0]);
        }
        script("https://test.invalid/loop5", 200, null, new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/loop0", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_NONE);
        for (int i = 0; i <= 5; ++i) {
            script("https://test.invalid/over" + i, 302, "/over" + (i + 1), new byte[0]);
        }
        check(DuskHttpClient.get("https://test.invalid/over0", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_NETWORK);
    }

    private static void testLimitsAndDeadline() {
        Script largeHeaders = new Script(200, null, new byte[0]);
        largeHeaders.headers.put("X-Large", List.of(repeat('a', 4097)));
        script("https://test.invalid/headers", largeHeaders);
        check(DuskHttpClient.get("https://test.invalid/headers", new String[0], new String[0], 1000, 16).error
                == DuskHttpClient.ERROR_NETWORK);
        Script tooManyHeaders = new Script(200, null, new byte[0]);
        for (int i = 0; i < 33; ++i) {
            tooManyHeaders.headers.put("X-" + i, List.of("x"));
        }
        script("https://test.invalid/header-count", tooManyHeaders);
        check(DuskHttpClient.get("https://test.invalid/header-count", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_NETWORK);
        Script aggregateHeaders = new Script(200, null, new byte[0]);
        for (int i = 0; i < 5; ++i) {
            aggregateHeaders.headers.put("X-Aggregate-" + i, List.of(repeat('a', 4096)));
        }
        script("https://test.invalid/header-aggregate", aggregateHeaders);
        check(DuskHttpClient.get("https://test.invalid/header-aggregate", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_NETWORK);
        script("https://test.invalid/body", 200, null, new byte[] {1, 2, 3});
        check(DuskHttpClient.get("https://test.invalid/body", new String[0], new String[0], 1000, 2).error
                == DuskHttpClient.ERROR_TOO_LARGE);
        Script redirectHeaders = new Script(302, "/never", new byte[0]);
        redirectHeaders.headers.put("X-Large", List.of(repeat('a', 4097)));
        script("https://test.invalid/redirect-headers", redirectHeaders);
        check(DuskHttpClient.get("https://test.invalid/redirect-headers", new String[0],
                new String[0], 1000, 16).error == DuskHttpClient.ERROR_NETWORK);
        Script delayed = new Script(302, "/after-delay", new byte[0]);
        delayed.delayMs = 20;
        script("https://test.invalid/deadline", delayed);
        script("https://test.invalid/after-delay", 200, null, new byte[0]);
        check(DuskHttpClient.get("https://test.invalid/deadline", new String[0], new String[0], 1, 16).error
                == DuskHttpClient.ERROR_TIMEOUT);
        Script trickled = new Script(200, null, new byte[] {1, 2, 3});
        trickled.chunkDelayMs = 20;
        script("https://test.invalid/trickle", trickled);
        check(DuskHttpClient.get("https://test.invalid/trickle", new String[0], new String[0], 1, 16).error
                == DuskHttpClient.ERROR_TIMEOUT);
    }

    private static void script(String url, int status, String location, byte[] body) {
        script(url, new Script(status, location, body));
    }

    private static void script(String url, Script script) {
        SCRIPTS.computeIfAbsent(url, ignored -> new ArrayDeque<>()).add(script);
    }

    private static String repeat(char value, int count) {
        char[] chars = new char[count];
        Arrays.fill(chars, value);
        return new String(chars);
    }

    private static void check(boolean value) {
        if (!value) {
            throw new AssertionError();
        }
    }

    private static final class FakeFactory implements URLStreamHandlerFactory {
        @Override
        public URLStreamHandler createURLStreamHandler(String protocol) {
            if (!"https".equals(protocol)) {
                return null;
            }
            return new URLStreamHandler() {
                @Override
                protected URLConnection openConnection(URL url) throws IOException {
                    ArrayDeque<Script> scripts = SCRIPTS.get(url.toExternalForm());
                    if (scripts == null || scripts.isEmpty()) {
                        throw new IOException("real network forbidden");
                    }
                    FakeConnection connection = new FakeConnection(url, scripts.removeFirst());
                    CONNECTIONS.put(url.toExternalForm(), connection);
                    return connection;
                }
            };
        }
    }

    private static final class Script {
        final int status;
        final String location;
        final byte[] body;
        final Map<String, List<String>> headers = new LinkedHashMap<>();
        int delayMs;
        int chunkDelayMs;

        Script(int status, String location, byte[] body) {
            this.status = status;
            this.location = location;
            this.body = body;
            if (location != null) {
                headers.put("Location", List.of(location));
            }
        }
    }

    private static final class FakeConnection extends HttpsURLConnection {
        final Script script;
        final ByteArrayOutputStream output = new ByteArrayOutputStream();
        final Map<String, String> requestHeaders = new LinkedHashMap<>();
        boolean outputClosed;

        FakeConnection(URL url, Script script) {
            super(url);
            this.script = script;
        }

        @Override public void connect() {}
        @Override public void disconnect() {}
        @Override public boolean usingProxy() { return false; }
        @Override public String getCipherSuite() { return "test"; }
        @Override public Certificate[] getLocalCertificates() { return null; }
        @Override public Certificate[] getServerCertificates() { return null; }
        @Override public Principal getPeerPrincipal() { return null; }
        @Override public Principal getLocalPrincipal() { return null; }
        @Override public OutputStream getOutputStream() {
            return new OutputStream() {
                @Override public void write(int value) { output.write(value); }
                @Override public void write(byte[] bytes, int offset, int length) {
                    output.write(bytes, offset, length);
                }
                @Override public void close() { outputClosed = true; }
            };
        }
        @Override public void setRequestProperty(String key, String value) {
            requestHeaders.put(key, value);
        }
        @Override public int getResponseCode() throws IOException {
            if (script.delayMs != 0) {
                try {
                    Thread.sleep(script.delayMs);
                } catch (InterruptedException e) {
                    throw new IOException(e);
                }
            }
            return script.status;
        }
        @Override public String getHeaderField(String name) {
            List<String> values = script.headers.get(name);
            return values == null || values.isEmpty() ? null : values.get(0);
        }
        @Override public Map<String, List<String>> getHeaderFields() { return script.headers; }
        @Override public InputStream getInputStream() {
            return new ByteArrayInputStream(script.body) {
                @Override public synchronized int read(byte[] bytes, int offset, int length) {
                    if (pos != 0 && script.chunkDelayMs != 0) {
                        try {
                            Thread.sleep(script.chunkDelayMs);
                        } catch (InterruptedException e) {
                            return -1;
                        }
                    }
                    return super.read(bytes, offset, Math.min(1, length));
                }
            };
        }
        @Override public InputStream getErrorStream() {
            return script.status >= 400 ? new ByteArrayInputStream(script.body) : null;
        }
    }
}
