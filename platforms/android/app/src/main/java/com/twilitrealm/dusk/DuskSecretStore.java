package dev.twilitrealm.dusk;

import android.content.Context;
import android.security.keystore.KeyGenParameterSpec;
import android.security.keystore.KeyPermanentlyInvalidatedException;
import android.security.keystore.KeyProperties;
import android.util.AtomicFile;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.util.Arrays;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;

/** Android facade. JNI loads this class through the Activity class loader. */
public final class DuskSecretStore {
    private static final Object LOCK = new Object();

    static final class Result {
        final int code;
        final byte[] value;

        Result(int code, byte[] value) {
            this.code = code;
            this.value = value;
        }
    }

    private DuskSecretStore() {
    }

    static Result get(Context context, byte[] modId, byte[] key) {
        byte[] plaintext = null;
        try {
            if (context == null) {
                return new Result(DuskSecretStoreCore.UNAVAILABLE, null);
            }
            synchronized (LOCK) {
                DuskSecretStoreCore.Result result = store(context).get(modId, key);
                plaintext = result.value;
                return new Result(result.code, plaintext == null ? null : plaintext.clone());
            }
        } catch (RuntimeException ignored) {
            return new Result(DuskSecretStoreCore.UNAVAILABLE, null);
        } finally {
            DuskSecretStoreCore.wipe(modId);
            DuskSecretStoreCore.wipe(key);
            DuskSecretStoreCore.wipe(plaintext);
        }
    }

    static int put(Context context, byte[] modId, byte[] key, byte[] value) {
        try {
            if (context == null) {
                return DuskSecretStoreCore.UNAVAILABLE;
            }
            synchronized (LOCK) {
                return store(context).put(modId, key, value);
            }
        } catch (RuntimeException ignored) {
            return DuskSecretStoreCore.UNAVAILABLE;
        } finally {
            DuskSecretStoreCore.wipe(modId);
            DuskSecretStoreCore.wipe(key);
            DuskSecretStoreCore.wipe(value);
        }
    }

    static int remove(Context context, byte[] modId, byte[] key) {
        try {
            if (context == null) {
                return DuskSecretStoreCore.UNAVAILABLE;
            }
            synchronized (LOCK) {
                return store(context).remove(modId, key);
            }
        } catch (RuntimeException ignored) {
            return DuskSecretStoreCore.UNAVAILABLE;
        } finally {
            DuskSecretStoreCore.wipe(modId);
            DuskSecretStoreCore.wipe(key);
        }
    }

    private static DuskSecretStoreCore store(Context context) {
        return new DuskSecretStoreCore(new AndroidKeyRepository(),
            new AtomicMetadataRepository(new File(context.getNoBackupFilesDir(), "dusklight-secret-storage-v1")),
            context.getPackageName());
    }

    private static final class AndroidKeyRepository implements DuskSecretStoreCore.KeyRepository {
        @Override
        public SecretKey find(String alias) throws DuskSecretStoreCore.MissingKeyException,
            DuskSecretStoreCore.InvalidatedKeyException, DuskSecretStoreCore.UnavailableException {
            try {
                KeyStore keyStore = keyStore();
                if (!keyStore.containsAlias(alias)) {
                    throw new DuskSecretStoreCore.MissingKeyException();
                }
                SecretKey key = (SecretKey) keyStore.getKey(alias, null);
                if (key == null) {
                    throw new DuskSecretStoreCore.MissingKeyException();
                }
                return key;
            } catch (KeyPermanentlyInvalidatedException invalidated) {
                throw new DuskSecretStoreCore.InvalidatedKeyException();
            } catch (DuskSecretStoreCore.MissingKeyException missing) {
                throw missing;
            } catch (GeneralSecurityException | IOException unavailable) {
                throw new DuskSecretStoreCore.UnavailableException();
            }
        }

        @Override
        public SecretKey create(String alias) throws DuskSecretStoreCore.InvalidatedKeyException,
            DuskSecretStoreCore.UnavailableException {
            try {
                KeyGenerator generator = KeyGenerator.getInstance("AES", "AndroidKeyStore");
                generator.init(new KeyGenParameterSpec.Builder(alias,
                    KeyProperties.PURPOSE_ENCRYPT | KeyProperties.PURPOSE_DECRYPT)
                    .setKeySize(256)
                    .setBlockModes(KeyProperties.BLOCK_MODE_GCM)
                    .setEncryptionPaddings(KeyProperties.ENCRYPTION_PADDING_NONE)
                    .setRandomizedEncryptionRequired(true)
                    .setUserAuthenticationRequired(false)
                    .build());
                return generator.generateKey();
            } catch (GeneralSecurityException unavailable) {
                throw new DuskSecretStoreCore.UnavailableException();
            }
        }

        @Override
        public boolean exists(String alias) throws DuskSecretStoreCore.UnavailableException {
            try {
                return keyStore().containsAlias(alias);
            } catch (GeneralSecurityException | IOException unavailable) {
                throw new DuskSecretStoreCore.UnavailableException();
            }
        }

        @Override
        public void delete(String alias) throws IOException, DuskSecretStoreCore.UnavailableException {
            try {
                KeyStore store = keyStore();
                store.deleteEntry(alias);
                if (store.containsAlias(alias)) {
                    throw new IOException();
                }
            } catch (GeneralSecurityException unavailable) {
                throw new DuskSecretStoreCore.UnavailableException();
            }
        }

        private static KeyStore keyStore() throws GeneralSecurityException, IOException {
            KeyStore result = KeyStore.getInstance("AndroidKeyStore");
            result.load(null);
            return result;
        }
    }

    private static final class AtomicMetadataRepository implements DuskSecretStoreCore.MetadataRepository {
        private static final int MAX_ENVELOPE = 8 + 4 + 4 + 4 + 4 + 32 + 12 + (64 * 1024) + 16;
        private final File directory;

        AtomicMetadataRepository(File directory) {
            this.directory = directory;
        }

        @Override
        public DuskSecretStoreCore.Metadata read(String filename) throws IOException {
            File base = file(filename);
            File pending = new File(base.getPath() + ".new");
            File backup = new File(base.getPath() + ".bak");
            requireRegularOrAbsent(base);
            requireRegularOrAbsent(pending);
            requireRegularOrAbsent(backup);
            if (!base.exists() && !backup.exists()) {
                if (!pending.exists()) {
                    return DuskSecretStoreCore.Metadata.notFound();
                }
                if (!pending.delete() || pending.exists()) {
                    throw new IOException();
                }
                return DuskSecretStoreCore.Metadata.notFound();
            }
            AtomicFile atomic = new AtomicFile(base);
            try (FileInputStream input = atomic.openRead()) {
                if (base.length() > MAX_ENVELOPE) {
                    return DuskSecretStoreCore.Metadata.committed(new byte[0]);
                }
                return DuskSecretStoreCore.Metadata.committed(readBounded(input, base.length()));
            }
        }

        @Override
        public boolean hasArtifacts(String filename) throws IOException {
            File base = file(filename);
            File pending = new File(base.getPath() + ".new");
            File backup = new File(base.getPath() + ".bak");
            requireRegularOrAbsent(base);
            requireRegularOrAbsent(pending);
            requireRegularOrAbsent(backup);
            return base.exists() || pending.exists() || backup.exists();
        }

        @Override
        public void write(String filename, byte[] data) throws IOException {
            if (!directory.isDirectory() && !directory.mkdirs()) {
                throw new IOException();
            }
            AtomicFile output = new AtomicFile(file(filename));
            FileOutputStream stream = null;
            try {
                stream = output.startWrite();
                stream.write(data);
                stream.flush();
                output.finishWrite(stream);
            } catch (IOException failure) {
                if (stream != null) {
                    output.failWrite(stream);
                }
                throw failure;
            }
        }

        @Override
        public void remove(String filename) throws IOException {
            File base = file(filename);
            requireRegularOrAbsent(base);
            requireRegularOrAbsent(new File(base.getPath() + ".new"));
            requireRegularOrAbsent(new File(base.getPath() + ".bak"));
            deleteAndVerify(base);
            deleteAndVerify(new File(base.getPath() + ".new"));
            deleteAndVerify(new File(base.getPath() + ".bak"));
        }

        private File file(String filename) {
            return new File(directory, filename);
        }

        private static void deleteAndVerify(File file) throws IOException {
            if (file.exists() && !file.delete()) {
                throw new IOException();
            }
            if (file.exists()) {
                throw new IOException();
            }
        }

        private static void requireRegularOrAbsent(File file) throws IOException {
            if (file.exists() && !file.isFile()) {
                throw new IOException();
            }
        }

        private static byte[] readBounded(FileInputStream input, long length) throws IOException {
            if (length < 0 || length > MAX_ENVELOPE) {
                throw new IOException();
            }
            byte[] result = new byte[(int) length];
            int offset = 0;
            try {
                while (offset < result.length) {
                    int read = input.read(result, offset, result.length - offset);
                    if (read < 0) {
                        throw new IOException();
                    }
                    offset += read;
                }
                if (input.read() != -1) {
                    throw new IOException();
                }
                return result;
            } catch (IOException failure) {
                Arrays.fill(result, (byte) 0);
                throw failure;
            }
        }
    }
}
