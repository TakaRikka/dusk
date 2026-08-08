package dev.twilitrealm.dusk;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;

public final class DuskHttpClient {
    public static final int ERROR_NONE = 0;
    public static final int ERROR_INVALID_URL = 1;
    public static final int ERROR_UNSUPPORTED_SCHEME = 2;
    public static final int ERROR_TIMEOUT = 3;
    public static final int ERROR_TOO_LARGE = 4;
    public static final int ERROR_NETWORK = 5;

    private static final int METHOD_GET = 0;
    private static final int METHOD_POST = 1;
    private static final int MAX_REDIRECTS = 5;
    private static final int MAX_HEADERS = 32;
    private static final int MAX_HEADER_NAME_BYTES = 128;
    private static final int MAX_HEADER_VALUE_BYTES = 4096;
    private static final int MAX_HEADER_BYTES = 16 * 1024;
    private static final int MAX_URL_BYTES = 4096;

    public static final class Response {
        public int error;
        public String message;
        public int statusCode;
        public String[] headerNames;
        public String[] headerValues;
        public byte[] body;

        Response(int error, String message, int statusCode, String[] headerNames,
                 String[] headerValues, byte[] body) {
            this.error = error;
            this.message = message;
            this.statusCode = statusCode;
            this.headerNames = headerNames != null ? headerNames : new String[0];
            this.headerValues = headerValues != null ? headerValues : new String[0];
            this.body = body != null ? body : new byte[0];
        }
    }

    private DuskHttpClient() {
    }

    public static Response get(String url, String[] headerNames, String[] headerValues,
                               int timeoutMs, long maxBodyBytes) {
        return request(METHOD_GET, url, headerNames, headerValues, new byte[0], timeoutMs,
                maxBodyBytes);
    }

    public static Response request(int method, String url, String[] headerNames,
                                   String[] headerValues, byte[] body, int timeoutMs,
                                   long maxBodyBytes) {
        if (!validUrlText(url)) {
            return fail(ERROR_INVALID_URL, "Invalid HTTPS URL");
        }
        try {
            return request(method, new URL(url), headerNames, headerValues, body, timeoutMs,
                    maxBodyBytes);
        } catch (MalformedURLException e) {
            return fail(ERROR_INVALID_URL, "Failed to parse URL");
        }
    }

    // Package-private for dependency-free URLStreamHandler tests. Production always uses URL(String).
    static Response request(int method, URL initialUrl, String[] headerNames, String[] headerValues,
                            byte[] body, int timeoutMs, long maxBodyBytes) {
        if ((method != METHOD_GET && method != METHOD_POST) || initialUrl == null ||
                timeoutMs <= 0 || maxBodyBytes < 0 || !validUrl(initialUrl) ||
                !validHeaders(headerNames, headerValues)) {
            return fail(ERROR_INVALID_URL, "Invalid HTTP request");
        }
        byte[] requestBody = body != null ? body : new byte[0];
        if (method == METHOD_GET && requestBody.length != 0) {
            return fail(ERROR_INVALID_URL, "GET requests cannot include a body");
        }

        final long deadline = System.nanoTime() + timeoutMs * 1_000_000L;
        URL currentUrl = initialUrl;
        int currentMethod = method;
        byte[] currentBody = requestBody;
        List<Header> currentHeaders = headerList(headerNames, headerValues);
        for (int redirects = 0; redirects <= MAX_REDIRECTS; ++redirects) {
            int remainingMs = remainingMs(deadline);
            if (remainingMs <= 0) {
                return fail(ERROR_TIMEOUT, "Request timed out");
            }
            HttpsURLConnection connection = null;
            try {
                connection = (HttpsURLConnection) currentUrl.openConnection();
                connection.setRequestMethod(currentMethod == METHOD_POST ? "POST" : "GET");
                connection.setConnectTimeout(remainingMs);
                connection.setReadTimeout(remainingMs);
                connection.setUseCaches(false);
                connection.setInstanceFollowRedirects(false);
                applyHeaders(connection, currentHeaders);
                if (currentMethod == METHOD_POST) {
                    connection.setDoOutput(true);
                    connection.setFixedLengthStreamingMode(currentBody.length);
                    if (remainingMs(deadline) <= 0) {
                        return fail(ERROR_TIMEOUT, "Request timed out");
                    }
                    try (OutputStream output = connection.getOutputStream()) {
                        output.write(currentBody);
                        if (remainingMs(deadline) <= 0) {
                            return fail(ERROR_TIMEOUT, "Request timed out");
                        }
                    }
                }

                int statusCode = connection.getResponseCode();
                if (remainingMs(deadline) <= 0) {
                    return fail(ERROR_TIMEOUT, "Request timed out");
                }
                HeaderLists headers = readHeaders(connection);
                if (headers == null) {
                    return fail(ERROR_NETWORK, "Response headers rejected");
                }
                if (isRedirect(statusCode)) {
                    String location = connection.getHeaderField("Location");
                    if (location == null || location.isEmpty() || redirects == MAX_REDIRECTS) {
                        return fail(ERROR_NETWORK, "Redirect rejected");
                    }
                    URL nextUrl = new URL(currentUrl, location);
                    if (!validUrl(nextUrl)) {
                        return fail(ERROR_UNSUPPORTED_SCHEME, "Redirect rejected");
                    }
                    boolean sameOrigin = sameOrigin(currentUrl, nextUrl);
                    if (currentMethod == METHOD_POST &&
                            (statusCode == HttpURLConnection.HTTP_MOVED_PERM ||
                            statusCode == HttpURLConnection.HTTP_MOVED_TEMP ||
                            statusCode == HttpURLConnection.HTTP_SEE_OTHER)) {
                        currentMethod = METHOD_GET;
                        currentBody = new byte[0];
                        currentHeaders = removeContentHeaders(currentHeaders, sameOrigin);
                    }
                    if (!sameOrigin) {
                        currentHeaders = removeSensitiveHeaders(currentHeaders);
                    }
                    currentUrl = nextUrl;
                    continue;
                }
                if (remainingMs(deadline) <= 0) {
                    return fail(ERROR_TIMEOUT, "Request timed out");
                }
                byte[] responseBody = readBody(connection, statusCode, maxBodyBytes, deadline);
                return new Response(ERROR_NONE, "", statusCode, headers.names, headers.values,
                        responseBody);
            } catch (ResponseTooLargeException e) {
                return fail(ERROR_TOO_LARGE, "Response body exceeded the configured limit");
            } catch (SocketTimeoutException e) {
                return fail(ERROR_TIMEOUT, "Request timed out");
            } catch (IOException | ClassCastException e) {
                return fail(ERROR_NETWORK, "HTTP request failed");
            } finally {
                if (connection != null) {
                    connection.disconnect();
                }
            }
        }
        return fail(ERROR_NETWORK, "Redirect rejected");
    }

    private static int remainingMs(long deadline) {
        long remaining = deadline - System.nanoTime();
        if (remaining <= 0) {
            return 0;
        }
        return (int) Math.min(Integer.MAX_VALUE, Math.max(1L, (remaining + 999_999L) / 1_000_000L));
    }

    private static boolean validUrlText(String value) {
        if (value == null || !value.startsWith("https://") ||
                value.getBytes(StandardCharsets.UTF_8).length > MAX_URL_BYTES ||
                value.indexOf('\0') >= 0 || value.indexOf('\r') >= 0 || value.indexOf('\n') >= 0) {
            return false;
        }
        int authorityStart = "https://".length();
        int authorityEnd = value.length();
        for (int i = authorityStart; i < value.length(); ++i) {
            char ch = value.charAt(i);
            if (ch == '/' || ch == '?' || ch == '#') {
                authorityEnd = i;
                break;
            }
        }
        if (authorityEnd <= authorityStart) {
            return false;
        }
        int userInfo = value.indexOf('@', authorityStart);
        return userInfo < 0 || userInfo >= authorityEnd;
    }

    private static boolean validUrl(URL url) {
        String external = url.toExternalForm();
        return external.startsWith("https://") && external.getBytes(StandardCharsets.UTF_8).length <= MAX_URL_BYTES &&
                "https".equals(url.getProtocol()) && url.getUserInfo() == null &&
                url.getRef() == null && url.getHost() != null && !url.getHost().isEmpty();
    }

    private static boolean validHeaders(String[] names, String[] values) {
        if (names == null || values == null || names.length != values.length || names.length > MAX_HEADERS) {
            return false;
        }
        int total = 0;
        for (int i = 0; i < names.length; ++i) {
            if (names[i] == null || values[i] == null || !isToken(names[i]) ||
                    values[i].indexOf('\r') >= 0 || values[i].indexOf('\n') >= 0 ||
                    values[i].indexOf('\0') >= 0) {
                return false;
            }
            int nameBytes = names[i].getBytes(StandardCharsets.UTF_8).length;
            int valueBytes = values[i].getBytes(StandardCharsets.UTF_8).length;
            if (nameBytes == 0 || nameBytes > MAX_HEADER_NAME_BYTES ||
                    valueBytes > MAX_HEADER_VALUE_BYTES || total > MAX_HEADER_BYTES - nameBytes ||
                    total + nameBytes > MAX_HEADER_BYTES - valueBytes) {
                return false;
            }
            total += nameBytes + valueBytes;
        }
        return true;
    }

    private static boolean isToken(String value) {
        for (int i = 0; i < value.length(); ++i) {
            char ch = value.charAt(i);
            if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
                    (ch >= '0' && ch <= '9') || "!#$%&'*+-.^_`|~".indexOf(ch) >= 0)) {
                return false;
            }
        }
        return !value.isEmpty();
    }

    private static List<Header> headerList(String[] names, String[] values) {
        List<Header> headers = new ArrayList<>();
        for (int i = 0; i < names.length; ++i) {
            headers.add(new Header(names[i], values[i]));
        }
        return headers;
    }

    private static void applyHeaders(HttpsURLConnection connection, List<Header> headers) {
        for (Header header : headers) {
            connection.setRequestProperty(header.name, header.value);
        }
    }

    private static List<Header> removeSensitiveHeaders(List<Header> headers) {
        return removeHeaders(headers, false, true);
    }

    private static List<Header> removeContentHeaders(List<Header> headers, boolean sameOrigin) {
        List<Header> result = removeHeaders(headers, true, false);
        return sameOrigin ? result : removeSensitiveHeaders(result);
    }

    private static List<Header> removeHeaders(List<Header> headers, boolean content,
                                              boolean sensitive) {
        List<Header> result = new ArrayList<>();
        for (Header header : headers) {
            String name = header.name;
            if ((sensitive && ("Authorization".equalsIgnoreCase(name) ||
                    "Cookie".equalsIgnoreCase(name))) ||
                    (content && name.regionMatches(true, 0, "Content-", 0, 8))) {
                continue;
            }
            result.add(header);
        }
        return result;
    }

    private static boolean sameOrigin(URL first, URL second) {
        return first.getProtocol().equalsIgnoreCase(second.getProtocol()) &&
                first.getHost().equalsIgnoreCase(second.getHost()) &&
                effectivePort(first) == effectivePort(second);
    }

    private static int effectivePort(URL url) {
        return url.getPort() >= 0 ? url.getPort() : 443;
    }

    private static boolean isRedirect(int statusCode) {
        return statusCode == HttpURLConnection.HTTP_MOVED_PERM ||
                statusCode == HttpURLConnection.HTTP_MOVED_TEMP ||
                statusCode == HttpURLConnection.HTTP_SEE_OTHER ||
                statusCode == 307 || statusCode == 308;
    }

    private static byte[] readBody(HttpsURLConnection connection, int statusCode, long maxBodyBytes,
                                   long deadline)
            throws IOException, ResponseTooLargeException {
        InputStream stream = statusCode >= HttpURLConnection.HTTP_BAD_REQUEST ?
                connection.getErrorStream() : connection.getInputStream();
        if (stream == null) {
            return new byte[0];
        }
        try (InputStream bodyStream = stream; ByteArrayOutputStream out = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[8192];
            long total = 0;
            for (;;) {
                int remaining = remainingMs(deadline);
                if (remaining <= 0) {
                    throw new SocketTimeoutException("Request timed out");
                }
                connection.setReadTimeout(remaining);
                int read = bodyStream.read(buffer);
                if (remainingMs(deadline) <= 0) {
                    throw new SocketTimeoutException("Request timed out");
                }
                if (read < 0) {
                    return out.toByteArray();
                }
                if (read > 0) {
                    if (read > maxBodyBytes || total > maxBodyBytes - read) {
                        throw new ResponseTooLargeException();
                    }
                    out.write(buffer, 0, read);
                    total += read;
                }
            }
        }
    }

    private static HeaderLists readHeaders(HttpsURLConnection connection) {
        List<String> names = new ArrayList<>();
        List<String> values = new ArrayList<>();
        Map<String, List<String>> fields = connection.getHeaderFields();
        if (fields == null) {
            return new HeaderLists(new String[0], new String[0]);
        }
        int total = 0;
        for (Map.Entry<String, List<String>> entry : fields.entrySet()) {
            if (entry.getKey() == null || entry.getValue() == null) {
                continue;
            }
            for (String value : entry.getValue()) {
                String safeValue = value != null ? value : "";
                int nameBytes = entry.getKey().getBytes(StandardCharsets.UTF_8).length;
                int valueBytes = safeValue.getBytes(StandardCharsets.UTF_8).length;
                if (names.size() == MAX_HEADERS || !isToken(entry.getKey()) ||
                        safeValue.indexOf('\r') >= 0 || safeValue.indexOf('\n') >= 0 ||
                        safeValue.indexOf('\0') >= 0 || nameBytes > MAX_HEADER_NAME_BYTES ||
                        valueBytes > MAX_HEADER_VALUE_BYTES || total > MAX_HEADER_BYTES - nameBytes ||
                        total + nameBytes > MAX_HEADER_BYTES - valueBytes) {
                    return null;
                }
                names.add(entry.getKey());
                values.add(safeValue);
                total += nameBytes + valueBytes;
            }
        }
        return new HeaderLists(names.toArray(new String[0]), values.toArray(new String[0]));
    }

    private static Response fail(int error, String message) {
        return new Response(error, message, 0, null, null, null);
    }

    private static final class Header {
        final String name;
        final String value;

        Header(String name, String value) {
            this.name = name;
            this.value = value;
        }
    }

    private static final class HeaderLists {
        final String[] names;
        final String[] values;

        HeaderLists(String[] names, String[] values) {
            this.names = names;
            this.values = values;
        }
    }

    private static final class ResponseTooLargeException extends Exception {
    }
}
