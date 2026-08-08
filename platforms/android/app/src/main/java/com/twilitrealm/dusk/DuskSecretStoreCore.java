package dev.twilitrealm.dusk;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.security.GeneralSecurityException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.util.Arrays;
import javax.crypto.AEADBadTagException;
import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.GCMParameterSpec;

/** Android-free envelope and state-machine core for SecretStorageService@1. */
public final class DuskSecretStoreCore {
    static final int OK = 0;
    static final int NOT_FOUND = 1;
    static final int INVALID_ARGUMENT = 2;
    static final int TOO_LARGE = 3;
    static final int UNAVAILABLE = 4;
    static final int CORRUPT = 5;
    static final int KEY_INVALIDATED = 6;
    static final int IO_ERROR = 7;
    static final int MAX_VALUE = 64 * 1024;
    static final int MAX_KEY = 128;
    static final int MAX_MOD_ID = 240;
    static final int MAX_MOD_LABEL = 64;

    private static final byte[] DOMAIN =
        "dev.twilitrealm.dusklight.secret_storage.identity.v1".getBytes(StandardCharsets.US_ASCII);
    private static final byte[] MAGIC = "DUSKSEC1".getBytes(StandardCharsets.US_ASCII);
    private static final int VERSION = 1;
    private static final int ALGORITHM_AES_256_GCM = 1;
    private static final int IV_LENGTH = 12;
    private static final int TAG_LENGTH = 16;
    private static final int HEADER_LENGTH = 8 + 4 + 4 + 4 + 4 + 32 + IV_LENGTH;

    public interface KeyRepository {
        SecretKey find(String alias) throws MissingKeyException, InvalidatedKeyException, UnavailableException;
        SecretKey create(String alias) throws InvalidatedKeyException, UnavailableException;
        boolean exists(String alias) throws UnavailableException;
        void delete(String alias) throws IOException, UnavailableException;
    }

    public interface MetadataRepository {
        Metadata read(String filename) throws IOException;
        boolean hasArtifacts(String filename) throws IOException;
        void write(String filename, byte[] data) throws IOException;
        void remove(String filename) throws IOException;
    }

    public static final class Metadata {
        final boolean committed;
        final byte[] data;

        private Metadata(boolean committed, byte[] data) {
            this.committed = committed;
            this.data = data;
        }

        public static Metadata notFound() {
            return new Metadata(false, null);
        }

        public static Metadata committed(byte[] data) {
            return new Metadata(true, data);
        }
    }

    public static final class MissingKeyException extends Exception {
        private static final long serialVersionUID = 1L;
    }

    public static final class InvalidatedKeyException extends Exception {
        private static final long serialVersionUID = 1L;
    }

    public static final class UnavailableException extends Exception {
        private static final long serialVersionUID = 1L;
    }

    public static final class Result {
        public final int code;
        public final byte[] value;

        Result(int code, byte[] value) {
            this.code = code;
            this.value = value;
        }

        static Result code(int code) {
            return new Result(code, null);
        }
    }

    static final class Identity {
        final byte[] digest;
        final String hex;
        final String alias;
        final String filename;

        Identity(byte[] digest, String hex) {
            this.digest = digest;
            this.hex = hex;
            this.alias = "dusk.secret.v1." + hex;
            this.filename = hex + ".bin";
        }
    }

    private final KeyRepository keys;
    private final MetadataRepository metadata;
    private final byte[] applicationId;

    public DuskSecretStoreCore(KeyRepository keys, MetadataRepository metadata, String applicationId) {
        this.keys = keys;
        this.metadata = metadata;
        this.applicationId = applicationId == null ? new byte[0] :
            applicationId.getBytes(StandardCharsets.UTF_8);
    }

    public synchronized Result get(byte[] modId, byte[] key) {
        Identity identity = identityOrNull(modId, key);
        if (identity == null) {
            return Result.code(INVALID_ARGUMENT);
        }
        Metadata record;
        try {
            record = metadata.read(identity.filename);
        } catch (IOException ignored) {
            return Result.code(IO_ERROR);
        }
        if (!record.committed) {
            return Result.code(NOT_FOUND);
        }
        return decryptRecord(identity, record.data);
    }

    public synchronized int put(byte[] modId, byte[] key, byte[] value) {
        Identity identity = identityOrNull(modId, key);
        if (identity == null || value == null) {
            return INVALID_ARGUMENT;
        }
        if (value.length > MAX_VALUE) {
            return TOO_LARGE;
        }
        Metadata existing;
        try {
            existing = metadata.read(identity.filename);
        } catch (IOException ignored) {
            return IO_ERROR;
        }
        if (existing.committed) {
            Result checked = decryptRecord(identity, existing.data);
            wipe(checked.value);
            if (checked.code != OK) {
                return checked.code;
            }
        }
        SecretKey secretKey;
        try {
            secretKey = keys.find(identity.alias);
        } catch (MissingKeyException ignored) {
            try {
                secretKey = keys.create(identity.alias);
            } catch (InvalidatedKeyException invalidated) {
                return KEY_INVALIDATED;
            } catch (UnavailableException unavailable) {
                return UNAVAILABLE;
            }
        } catch (InvalidatedKeyException invalidated) {
            return KEY_INVALIDATED;
        } catch (UnavailableException unavailable) {
            return UNAVAILABLE;
        }
        byte[] envelope = null;
        try {
            envelope = encrypt(identity, secretKey, value);
            metadata.write(identity.filename, envelope);
            return OK;
        } catch (InvalidKeyException invalidated) {
            return KEY_INVALIDATED;
        } catch (GeneralSecurityException unavailable) {
            return UNAVAILABLE;
        } catch (IOException io) {
            return IO_ERROR;
        } finally {
            wipe(envelope);
        }
    }

    public synchronized int remove(byte[] modId, byte[] key) {
        Identity identity = identityOrNull(modId, key);
        if (identity == null) {
            return INVALID_ARGUMENT;
        }
        final boolean hasArtifacts;
        final boolean hasKey;
        try {
            hasArtifacts = metadata.hasArtifacts(identity.filename);
            hasKey = keys.exists(identity.alias);
        } catch (IOException io) {
            return IO_ERROR;
        } catch (UnavailableException unavailable) {
            return UNAVAILABLE;
        }
        if (!hasArtifacts && !hasKey) {
            return NOT_FOUND;
        }
        if (hasArtifacts) {
            try {
                metadata.remove(identity.filename);
            } catch (IOException io) {
                return IO_ERROR;
            }
        }
        if (!hasKey) {
            return OK;
        }
        try {
            keys.delete(identity.alias);
            return OK;
        } catch (IOException io) {
            return IO_ERROR;
        } catch (UnavailableException unavailable) {
            return UNAVAILABLE;
        }
    }

    static boolean validModId(byte[] value) {
        if (value == null || value.length == 0 || value.length > MAX_MOD_ID || value[0] == '.' ||
            value[value.length - 1] == '.') {
            return false;
        }
        int labelLength = 0;
        for (byte raw : value) {
            int character = raw & 0xff;
            if (character == '.') {
                if (labelLength == 0 || labelLength > MAX_MOD_LABEL) {
                    return false;
                }
                labelLength = 0;
            } else if ((character >= 'a' && character <= 'z') ||
                (character >= '0' && character <= '9') || character == '_') {
                labelLength++;
            } else {
                return false;
            }
        }
        return labelLength > 0 && labelLength <= MAX_MOD_LABEL;
    }

    static boolean validKey(byte[] value) {
        if (value == null || value.length == 0 || value.length > MAX_KEY || value[0] == '.') {
            return false;
        }
        for (byte raw : value) {
            int character = raw & 0xff;
            if (!((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') ||
                character == '_' || character == '.' || character == '-')) {
                return false;
            }
        }
        return true;
    }

    static Identity identity(byte[] applicationId, byte[] modId, byte[] key) {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            digest.update(DOMAIN);
            updateLengthPrefixed(digest, applicationId);
            updateLengthPrefixed(digest, modId);
            updateLengthPrefixed(digest, key);
            byte[] hash = digest.digest();
            String hex = hex(hash);
            return new Identity(hash, hex);
        } catch (GeneralSecurityException impossible) {
            throw new IllegalStateException(impossible);
        }
    }

    private Identity identityOrNull(byte[] modId, byte[] key) {
        return validModId(modId) && validKey(key) ? identity(applicationId, modId, key) : null;
    }

    private Result decryptRecord(Identity identity, byte[] envelope) {
        if (envelope == null || envelope.length < HEADER_LENGTH + TAG_LENGTH) {
            return Result.code(CORRUPT);
        }
        ByteBuffer input = ByteBuffer.wrap(envelope).order(ByteOrder.LITTLE_ENDIAN);
        byte[] magic = new byte[8];
        input.get(magic);
        int version = input.getInt();
        int algorithm = input.getInt();
        int ivLength = input.getInt();
        int cipherLength = input.getInt();
        byte[] storedIdentity = new byte[32];
        input.get(storedIdentity);
        byte[] iv = new byte[IV_LENGTH];
        input.get(iv);
        if (!Arrays.equals(magic, MAGIC) || version != VERSION || algorithm != ALGORITHM_AES_256_GCM ||
            ivLength != IV_LENGTH || cipherLength < 0 || cipherLength > MAX_VALUE ||
            envelope.length != HEADER_LENGTH + cipherLength + TAG_LENGTH ||
            !MessageDigest.isEqual(storedIdentity, identity.digest)) {
            wipe(magic);
            wipe(storedIdentity);
            wipe(iv);
            return Result.code(CORRUPT);
        }
        SecretKey secretKey;
        try {
            secretKey = keys.find(identity.alias);
        } catch (MissingKeyException missing) {
            wipe(magic);
            wipe(storedIdentity);
            wipe(iv);
            return Result.code(KEY_INVALIDATED);
        } catch (InvalidatedKeyException invalidated) {
            wipe(magic);
            wipe(storedIdentity);
            wipe(iv);
            return Result.code(KEY_INVALIDATED);
        } catch (UnavailableException unavailable) {
            wipe(magic);
            wipe(storedIdentity);
            wipe(iv);
            return Result.code(UNAVAILABLE);
        }
        byte[] combined = new byte[cipherLength + TAG_LENGTH];
        input.get(combined);
        byte[] header = Arrays.copyOf(envelope, HEADER_LENGTH);
        try {
            Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
            cipher.init(Cipher.DECRYPT_MODE, secretKey, new GCMParameterSpec(128, iv));
            cipher.updateAAD(header);
            byte[] value = cipher.doFinal(combined);
            if (value.length != cipherLength) {
                wipe(value);
                return Result.code(CORRUPT);
            }
            return new Result(OK, value);
        } catch (AEADBadTagException tag) {
            return Result.code(CORRUPT);
        } catch (InvalidKeyException invalidated) {
            return Result.code(KEY_INVALIDATED);
        } catch (GeneralSecurityException unavailable) {
            return Result.code(UNAVAILABLE);
        } finally {
            wipe(magic);
            wipe(storedIdentity);
            wipe(iv);
            wipe(combined);
            wipe(header);
        }
    }

    private static byte[] encrypt(Identity identity, SecretKey secretKey, byte[] value)
        throws GeneralSecurityException {
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
        cipher.init(Cipher.ENCRYPT_MODE, secretKey);
        byte[] iv = cipher.getIV();
        if (iv == null || iv.length != IV_LENGTH) {
            wipe(iv);
            throw new GeneralSecurityException();
        }
        byte[] header = ByteBuffer.allocate(HEADER_LENGTH).order(ByteOrder.LITTLE_ENDIAN)
            .put(MAGIC).putInt(VERSION).putInt(ALGORITHM_AES_256_GCM).putInt(IV_LENGTH)
            .putInt(value.length).put(identity.digest).put(iv).array();
        try {
            cipher.updateAAD(header);
            byte[] encrypted = cipher.doFinal(value);
            if (encrypted.length != value.length + TAG_LENGTH) {
                wipe(encrypted);
                throw new GeneralSecurityException();
            }
            byte[] result = new byte[HEADER_LENGTH + encrypted.length];
            System.arraycopy(header, 0, result, 0, HEADER_LENGTH);
            System.arraycopy(encrypted, 0, result, HEADER_LENGTH, encrypted.length);
            wipe(encrypted);
            return result;
        } finally {
            wipe(iv);
            wipe(header);
        }
    }

    private static void updateLengthPrefixed(MessageDigest digest, byte[] value) {
        byte[] length = ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(value.length).array();
        digest.update(length);
        digest.update(value);
        wipe(length);
    }

    static String hex(byte[] data) {
        char[] result = new char[data.length * 2];
        final char[] digits = "0123456789abcdef".toCharArray();
        for (int index = 0; index < data.length; index++) {
            int value = data[index] & 0xff;
            result[index * 2] = digits[value >>> 4];
            result[index * 2 + 1] = digits[value & 15];
        }
        return new String(result);
    }

    static void wipe(byte[] value) {
        if (value != null) {
            Arrays.fill(value, (byte) 0);
        }
    }
}
