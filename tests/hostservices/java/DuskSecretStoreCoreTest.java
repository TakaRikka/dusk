package dev.twilitrealm.dusk;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.security.KeyStore;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;

public final class DuskSecretStoreCoreTest {
    private static final byte[] MOD = "com.example_mod".getBytes(StandardCharsets.US_ASCII);
    private static final byte[] KEY = "token".getBytes(StandardCharsets.US_ASCII);

    private static final class Keys implements DuskSecretStoreCore.KeyRepository {
        final Map<String, SecretKey> values = new HashMap<>();
        boolean invalidated;
        boolean unavailable;
        boolean deleteFails;

        public SecretKey find(String alias) throws DuskSecretStoreCore.MissingKeyException,
            DuskSecretStoreCore.InvalidatedKeyException, DuskSecretStoreCore.UnavailableException {
            if (unavailable) throw new DuskSecretStoreCore.UnavailableException();
            if (invalidated) throw new DuskSecretStoreCore.InvalidatedKeyException();
            SecretKey key = values.get(alias);
            if (key == null) throw new DuskSecretStoreCore.MissingKeyException();
            return key;
        }
        public SecretKey create(String alias) throws DuskSecretStoreCore.UnavailableException {
            if (unavailable) throw new DuskSecretStoreCore.UnavailableException();
            try {
                KeyGenerator generator = KeyGenerator.getInstance("AES");
                generator.init(256);
                SecretKey key = generator.generateKey();
                values.put(alias, key);
                return key;
            } catch (Exception failure) {
                throw new DuskSecretStoreCore.UnavailableException();
            }
        }
        public boolean exists(String alias) throws DuskSecretStoreCore.UnavailableException {
            if (unavailable) throw new DuskSecretStoreCore.UnavailableException();
            return values.containsKey(alias);
        }
        public void delete(String alias) throws IOException, DuskSecretStoreCore.UnavailableException {
            if (unavailable) throw new DuskSecretStoreCore.UnavailableException();
            if (deleteFails) throw new IOException();
            values.remove(alias);
        }
    }

    private static final class Metadata implements DuskSecretStoreCore.MetadataRepository {
        final Map<String, byte[]> values = new HashMap<>();
        boolean newOnly;
        boolean cleanupFails;
        boolean removeFails;
        boolean writeFails;

        public DuskSecretStoreCore.Metadata read(String filename) throws IOException {
            if (newOnly) {
                if (cleanupFails) throw new IOException();
                newOnly = false;
            }
            byte[] bytes = values.get(filename);
            return bytes == null ? DuskSecretStoreCore.Metadata.notFound() :
                DuskSecretStoreCore.Metadata.committed(bytes);
        }
        public boolean hasArtifacts(String filename) {
            return newOnly || values.containsKey(filename);
        }
        public void write(String filename, byte[] data) throws IOException {
            if (writeFails) throw new IOException();
            values.put(filename, Arrays.copyOf(data, data.length));
        }
        public void remove(String filename) throws IOException {
            if (removeFails) throw new IOException();
            values.remove(filename);
            newOnly = false;
        }
    }

    private static final class InvalidAesKey implements SecretKey {
        public String getAlgorithm() { return "AES"; }
        public String getFormat() { return "RAW"; }
        public byte[] getEncoded() { return new byte[] {1}; }
    }

    private static DuskSecretStoreCore store(Keys keys, Metadata metadata) {
        return new DuskSecretStoreCore(keys, metadata, "dev.twilitrealm.dusk");
    }

    private static void assertCode(int expected, int actual) {
        assert expected == actual : expected + " != " + actual;
    }

    private static String filename() {
        return DuskSecretStoreCore.identity("dev.twilitrealm.dusk".getBytes(StandardCharsets.UTF_8), MOD, KEY).filename;
    }

    private static void testIdentity() {
        DuskSecretStoreCore.Identity identity = DuskSecretStoreCore.identity(
            "dev.twilitrealm.dusk".getBytes(StandardCharsets.UTF_8), MOD, KEY);
        assert "4657e16d2c559494f24ca4cc9b183d33e01905ecfca085275a2848674296ec5b".equals(identity.hex);
        assert !DuskSecretStoreCore.identity("a".getBytes(StandardCharsets.UTF_8),
            "a".getBytes(StandardCharsets.US_ASCII), "bc".getBytes(StandardCharsets.US_ASCII)).hex.equals(
            DuskSecretStoreCore.identity("a".getBytes(StandardCharsets.UTF_8),
                "ab".getBytes(StandardCharsets.US_ASCII), "c".getBytes(StandardCharsets.US_ASCII)).hex);
    }

    private static void testCryptoRestartAndCorruption() {
        Keys keys = new Keys();
        Metadata metadata = new Metadata();
        DuskSecretStoreCore first = store(keys, metadata);
        byte[] one = "secret-one".getBytes(StandardCharsets.US_ASCII);
        assertCode(DuskSecretStoreCore.OK, first.put(MOD, KEY, one));
        byte[] initial = Arrays.copyOf(metadata.values.get(filename()), metadata.values.get(filename()).length);
        assertCode(DuskSecretStoreCore.OK, first.put(MOD, KEY, one));
        byte[] second = metadata.values.get(filename());
        assert !Arrays.equals(initial, second);
        DuskSecretStoreCore restarted = store(keys, metadata);
        DuskSecretStoreCore.Result read = restarted.get(MOD, KEY);
        assertCode(DuskSecretStoreCore.OK, read.code);
        assert Arrays.equals(one, read.value);
        DuskSecretStoreCore.wipe(read.value);

        byte[] valid = Arrays.copyOf(second, second.length);
        int[] corruptOffsets = {0, 24, 40, valid.length - 1};
        for (int offset : corruptOffsets) {
            byte[] corrupted = Arrays.copyOf(valid, valid.length);
            corrupted[offset] ^= 1;
            metadata.values.put(filename(), corrupted);
            assertCode(DuskSecretStoreCore.CORRUPT, restarted.get(MOD, KEY).code);
        }
        byte[] impossibleLength = Arrays.copyOf(valid, valid.length);
        ByteBuffer.wrap(impossibleLength).order(ByteOrder.LITTLE_ENDIAN).putInt(20, 65537);
        metadata.values.put(filename(), impossibleLength);
        assertCode(DuskSecretStoreCore.CORRUPT, restarted.get(MOD, KEY).code);
        metadata.values.put(filename(), Arrays.copyOf(valid, valid.length + 1));
        assertCode(DuskSecretStoreCore.CORRUPT, restarted.get(MOD, KEY).code);
        DuskSecretStoreCore.wipe(initial);
        DuskSecretStoreCore.wipe(valid);
    }

    private static void testInvalidationPutAndRemove() {
        Keys keys = new Keys();
        Metadata metadata = new Metadata();
        DuskSecretStoreCore secret = store(keys, metadata);
        assertCode(DuskSecretStoreCore.OK, secret.put(MOD, KEY, new byte[] {1}));
        String alias = DuskSecretStoreCore.identity(
            "dev.twilitrealm.dusk".getBytes(StandardCharsets.UTF_8), MOD, KEY).alias;
        keys.values.remove(alias);
        assertCode(DuskSecretStoreCore.KEY_INVALIDATED, secret.get(MOD, KEY).code);
        assertCode(DuskSecretStoreCore.KEY_INVALIDATED, secret.put(MOD, KEY, new byte[] {2}));
        metadata.values.put(filename(), new byte[] {1, 2, 3});
        assertCode(DuskSecretStoreCore.CORRUPT, secret.put(MOD, KEY, new byte[] {2}));

        metadata.values.clear();
        metadata.newOnly = true;
        assertCode(DuskSecretStoreCore.NOT_FOUND, secret.get(MOD, KEY).code);
        metadata.newOnly = true;
        metadata.cleanupFails = true;
        assertCode(DuskSecretStoreCore.IO_ERROR, secret.get(MOD, KEY).code);
        metadata.cleanupFails = false;
        metadata.newOnly = false;
        assertCode(DuskSecretStoreCore.OK, secret.put(MOD, KEY, new byte[] {3}));
        metadata.removeFails = true;
        keys.deleteFails = false;
        assertCode(DuskSecretStoreCore.IO_ERROR, secret.remove(MOD, KEY));
        assert keys.values.containsKey(alias);
        metadata.removeFails = false;
        keys.deleteFails = true;
        assertCode(DuskSecretStoreCore.IO_ERROR, secret.remove(MOD, KEY));
        assert keys.values.containsKey(alias);
        keys.deleteFails = false;
        assertCode(DuskSecretStoreCore.OK, secret.remove(MOD, KEY));
        assertCode(DuskSecretStoreCore.NOT_FOUND, secret.remove(MOD, KEY));
        keys.values.put(alias, newKey());
        assertCode(DuskSecretStoreCore.OK, secret.remove(MOD, KEY));
    }

    private static SecretKey newKey() {
        try {
            KeyGenerator generator = KeyGenerator.getInstance("AES");
            generator.init(256);
            return generator.generateKey();
        } catch (Exception failure) {
            throw new AssertionError(failure);
        }
    }

    private static void testBoundsAndConcurrency() throws InterruptedException {
        Keys keys = new Keys();
        Metadata metadata = new Metadata();
        DuskSecretStoreCore secret = store(keys, metadata);
        assertCode(DuskSecretStoreCore.INVALID_ARGUMENT, secret.put(
            "Com.example".getBytes(StandardCharsets.US_ASCII), KEY, new byte[0]));
        assertCode(DuskSecretStoreCore.INVALID_ARGUMENT, secret.put(
            MOD, "Upper".getBytes(StandardCharsets.US_ASCII), new byte[0]));
        assertCode(DuskSecretStoreCore.TOO_LARGE, secret.put(MOD, KEY, new byte[65537]));
        byte[] maxMod = (new String(new char[64]).replace('\0', 'a') + "." +
            new String(new char[64]).replace('\0', 'b') + "." +
            new String(new char[64]).replace('\0', 'c') + "." +
            new String(new char[45]).replace('\0', 'd')).getBytes(StandardCharsets.US_ASCII);
        assert maxMod.length == DuskSecretStoreCore.MAX_MOD_ID;
        assert DuskSecretStoreCore.validModId(maxMod);
        byte[] tooLongMod = Arrays.copyOf(maxMod, maxMod.length + 1);
        tooLongMod[tooLongMod.length - 1] = 'e';
        assert !DuskSecretStoreCore.validModId(tooLongMod);
        assert !DuskSecretStoreCore.validModId(
            new String(new char[65]).replace('\0', 'a').getBytes(StandardCharsets.US_ASCII));
        metadata.writeFails = true;
        assertCode(DuskSecretStoreCore.IO_ERROR, secret.put(MOD, KEY, new byte[0]));
        metadata.writeFails = false;
        Thread first = new Thread(() -> assertCode(DuskSecretStoreCore.OK,
            secret.put(MOD, KEY, new byte[] {1})));
        Thread second = new Thread(() -> assertCode(DuskSecretStoreCore.OK,
            secret.put(MOD, KEY, new byte[] {2})));
        first.start();
        second.start();
        first.join();
        second.join();
        assertCode(DuskSecretStoreCore.OK, secret.get(MOD, KEY).code);
    }

    private static void testInvalidAesKey() {
        Keys keys = new Keys();
        Metadata metadata = new Metadata();
        DuskSecretStoreCore secret = store(keys, metadata);
        String alias = DuskSecretStoreCore.identity(
            "dev.twilitrealm.dusk".getBytes(StandardCharsets.UTF_8), MOD, KEY).alias;
        keys.values.put(alias, new InvalidAesKey());
        assertCode(DuskSecretStoreCore.KEY_INVALIDATED, secret.put(MOD, KEY, new byte[] {1}));
        keys.values.remove(alias);
        assertCode(DuskSecretStoreCore.OK, secret.put(MOD, KEY, new byte[] {1}));
        keys.values.put(alias, new InvalidAesKey());
        assertCode(DuskSecretStoreCore.KEY_INVALIDATED, secret.get(MOD, KEY).code);
    }

    public static void main(String[] args) throws Exception {
        testIdentity();
        testCryptoRestartAndCorruption();
        testInvalidationPutAndRemove();
        testBoundsAndConcurrency();
        testInvalidAesKey();
    }
}
