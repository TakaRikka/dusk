package dev.twilitrealm.dusk;

import android.app.ActionBar;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.ClipData;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.OpenableColumns;
import android.provider.Settings;
import android.util.Log;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;

import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class DuskActivity extends SDLActivity {
    private static final String TAG = "DuskActivity";
    private static final float DEFAULT_SURFACE_FRAME_RATE = 60.0f;
    private static final int FOLDER_DIALOG_REQUEST_CODE = 0x4455;
    private static final int MANAGE_STORAGE_REQUEST_CODE = 0x4456;
    private static final String EXTERNAL_STORAGE_AUTHORITY =
        "com.android.externalstorage.documents";

    private long folderDialogUserdata = 0;
    private boolean awaitingManageStoragePermission = false;

    private static native void nativeFolderDialogResult(long userdata, String path, String error);

    private static String[] splitArgs(String raw) {
        List<String> out = new ArrayList<>();
        StringBuilder current = new StringBuilder();
        boolean inSingle = false;
        boolean inDouble = false;
        boolean escaped = false;

        for (int i = 0; i < raw.length(); ++i) {
            char c = raw.charAt(i);
            if (escaped) {
                current.append(c);
                escaped = false;
                continue;
            }
            if (c == '\\' && !inSingle) {
                escaped = true;
                continue;
            }
            if (c == '"' && !inSingle) {
                inDouble = !inDouble;
                continue;
            }
            if (c == '\'' && !inDouble) {
                inSingle = !inSingle;
                continue;
            }
            if (!inSingle && !inDouble && Character.isWhitespace(c)) {
                if (current.length() > 0) {
                    out.add(current.toString());
                    current.setLength(0);
                }
                continue;
            }
            current.append(c);
        }

        if (escaped) {
            current.append('\\');
        }
        if (current.length() > 0) {
            out.add(current.toString());
        }
        return out.toArray(new String[0]);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        hideSystemBars();
    }

    @Override
    protected SDLSurface createSDLSurface(Context context) {
        return new DuskSurface(context);
    }

    @Override
    protected void onResume() {
        super.onResume();
        hideSystemBars();
        if (awaitingManageStoragePermission) {
            resumeFolderDialogAfterPermissionGrant();
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemBars();
        }
    }

    private void hideSystemBars() {
        Window window = getWindow();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.setDecorFitsSystemWindows(false);
            WindowInsetsController ctrl = window.getDecorView().getWindowInsetsController();
            if (ctrl != null) {
                ctrl.setSystemBarsBehavior(
                    WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
                ctrl.hide(WindowInsets.Type.systemBars());
            }
        } else {
            View decorView = window.getDecorView();
            int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
            decorView.setSystemUiVisibility(uiOptions);
            ActionBar actionBar = getActionBar();
            if (actionBar != null) {
                actionBar.hide();
            }
        }
    }

    @Override
    protected String[] getLibraries() {
        // SDL3 is statically linked into libmain.so in this build.
        return new String[] {
            "main"
        };
    }

    public void setPreferredSurfaceFrameRate(float frameRate) {
        runOnUiThread(() -> {
            if (mSurface instanceof DuskSurface) {
                ((DuskSurface)mSurface).setPreferredFrameRate(frameRate);
            }
        });
    }

    private static final class DuskSurface extends SDLSurface {
        private float preferredFrameRate = DEFAULT_SURFACE_FRAME_RATE;

        DuskSurface(Context context) {
            super(context);
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            super.surfaceChanged(holder, format, width, height);
            setTargetFrameRate(holder);
        }

        void setPreferredFrameRate(float frameRate) {
            preferredFrameRate = frameRate;
            setTargetFrameRate(getHolder());
        }

        private void setTargetFrameRate(SurfaceHolder holder) {
            if (!mIsSurfaceReady || Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
                return;
            }

            Surface surface = holder != null ? holder.getSurface() : getHolder().getSurface();
            if (surface == null || !surface.isValid()) {
                return;
            }

            float targetFrameRate = getMaxSupportedFrameRate();
            if (preferredFrameRate > 0.0f) {
                targetFrameRate = preferredFrameRate;
            }
            if (targetFrameRate <= 0.0f) {
                return;
            }

            try {
                surface.setFrameRate(
                    targetFrameRate, Surface.FRAME_RATE_COMPATIBILITY_DEFAULT);
                Log.v(TAG, "Requested surface frame rate " + targetFrameRate + " fps");
            } catch (RuntimeException e) {
                Log.w(TAG, "Failed to request surface frame rate", e);
            }
        }

        private float getMaxSupportedFrameRate() {
            if (mDisplay == null) {
                return 0.0f;
            }

            float maxFrameRate = mDisplay.getRefreshRate();
            Display.Mode[] modes = mDisplay.getSupportedModes();
            if (modes == null) {
                return maxFrameRate;
            }

            for (Display.Mode mode : modes) {
                maxFrameRate = Math.max(maxFrameRate, mode.getRefreshRate());
            }
            return maxFrameRate;
        }
    }

    @Override
    protected String[] getArguments() {
        Intent intent = getIntent();
        if (intent != null) {
            String[] argv = intent.getStringArrayExtra("dusk_argv");
            if (argv != null && argv.length > 0) {
                return argv;
            }

            String rawArgs = intent.getStringExtra("dusk_args");
            if (rawArgs != null) {
                String trimmed = rawArgs.trim();
                if (!trimmed.isEmpty()) {
                    return splitArgs(trimmed);
                }
            }
        }
        return new String[0];
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            persistUriPermissions(data);
        }
        if (requestCode == FOLDER_DIALOG_REQUEST_CODE) {
            finishFolderDialog(resultCode, data);
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    public boolean showFolderDialog(long userdata) {
        if (userdata == 0 || folderDialogUserdata != 0) {
            return false;
        }

        folderDialogUserdata = userdata;
        if (requiresManageStoragePermission() && !hasManageStoragePermission()) {
            if (!requestManageStoragePermission()) {
                finishFolderDialogWithError("Unable to request Android file access permission");
                return false;
            }
            return true;
        }

        openFolderDialog();
        return true;
    }

    private void openFolderDialog() {
        runOnUiThread(() -> {
            Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT_TREE);
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION |
                Intent.FLAG_GRANT_WRITE_URI_PERMISSION |
                Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION |
                Intent.FLAG_GRANT_PREFIX_URI_PERMISSION);

            try {
                startActivityForResult(intent, FOLDER_DIALOG_REQUEST_CODE);
            } catch (ActivityNotFoundException e) {
                Log.w(TAG, "Unable to open folder dialog.", e);
                finishFolderDialog(Activity.RESULT_CANCELED, null);
            }
        });
    }

    private boolean requiresManageStoragePermission() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.R;
    }

    private boolean hasManageStoragePermission() {
        return !requiresManageStoragePermission() || Environment.isExternalStorageManager();
    }

    private boolean requestManageStoragePermission() {
        if (!requiresManageStoragePermission()) {
            return true;
        }

        awaitingManageStoragePermission = true;
        runOnUiThread(() -> {
            if (tryStartManageStorageIntent(
                    new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION)
                        .setData(Uri.parse("package:" + getPackageName()))) ||
                tryStartManageStorageIntent(
                    new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION)))
            {
                return;
            }

            finishFolderDialogWithError("Unable to request Android file access permission");
        });
        return true;
    }

    private boolean tryStartManageStorageIntent(Intent intent) {
        try {
            startActivityForResult(intent, MANAGE_STORAGE_REQUEST_CODE);
            return true;
        } catch (ActivityNotFoundException e) {
            Log.w(TAG, "Unable to open all-files access settings.", e);
            return false;
        }
    }

    private void resumeFolderDialogAfterPermissionGrant() {
        awaitingManageStoragePermission = false;
        if (folderDialogUserdata == 0) {
            return;
        }

        if (hasManageStoragePermission()) {
            openFolderDialog();
            return;
        }

        finishFolderDialogWithError(
            "Allow \"All files access\" for Dusklight before choosing a custom data folder");
    }

    private void finishFolderDialogWithError(String error) {
        long userdata = folderDialogUserdata;
        folderDialogUserdata = 0;
        awaitingManageStoragePermission = false;
        if (userdata != 0) {
            nativeFolderDialogResult(userdata, null, error);
        }
    }

    private void finishFolderDialog(int resultCode, Intent data) {
        long userdata = folderDialogUserdata;
        folderDialogUserdata = 0;
        if (userdata == 0) {
            return;
        }

        if (resultCode == RESULT_OK && data != null && data.getData() != null) {
            String path = getRealPathForUri(data.getData());
            if (path != null && !path.isEmpty()) {
                nativeFolderDialogResult(userdata, path, null);
            } else {
                nativeFolderDialogResult(
                    userdata, null, "Selected folder is not available as a filesystem path");
            }
            return;
        }

        nativeFolderDialogResult(userdata, null, null);
    }

    private String getRealPathForUri(Uri uri) {
        if (uri == null) {
            return null;
        }

        String scheme = uri.getScheme();
        if ("file".equals(scheme)) {
            return uri.getPath();
        }

        if (!"content".equals(scheme) ||
            !EXTERNAL_STORAGE_AUTHORITY.equals(uri.getAuthority()) ||
            Build.VERSION.SDK_INT < Build.VERSION_CODES.KITKAT)
        {
            return null;
        }

        try {
            return getExternalStoragePathForDocumentId(getExternalStorageDocumentId(uri));
        } catch (IllegalArgumentException e) {
            Log.w(TAG, "Unable to resolve URI: " + uri, e);
            return null;
        }
    }

    private static String getExternalStorageDocumentId(Uri uri) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP && isTreeDocumentUri(uri)) {
            return DocumentsContract.getTreeDocumentId(uri);
        }

        return DocumentsContract.getDocumentId(uri);
    }

    private static boolean isTreeDocumentUri(Uri uri) {
        List<String> segments = uri.getPathSegments();
        return segments.size() >= 2 && "tree".equals(segments.get(0));
    }

    private String getExternalStoragePathForDocumentId(String documentId) {
        if (documentId == null || documentId.isEmpty()) {
            return null;
        }
        if (documentId.startsWith("raw:")) {
            return documentId.substring("raw:".length());
        }

        String[] parts = documentId.split(":", 2);
        String volumeId = parts[0];
        String relativePath = parts.length > 1 ? parts[1] : "";

        File root = getExternalStorageRoot(volumeId);
        if (root == null) {
            return null;
        }

        return relativePath.isEmpty()
            ? root.getAbsolutePath()
            : new File(root, relativePath).getAbsolutePath();
    }

    private File getExternalStorageRoot(String volumeId) {
        if ("primary".equalsIgnoreCase(volumeId)) {
            return Environment.getExternalStorageDirectory();
        }
        if ("home".equalsIgnoreCase(volumeId)) {
            return new File(
                Environment.getExternalStorageDirectory(), Environment.DIRECTORY_DOCUMENTS);
        }

        File[] externalFilesDirs = getExternalFilesDirs(null);
        if (externalFilesDirs != null) {
            for (File externalFilesDir : externalFilesDirs) {
                File root = getStorageRootForExternalFilesDir(externalFilesDir);
                if (root != null && volumeId.equalsIgnoreCase(root.getName())) {
                    return root;
                }
            }
        }

        File fallback = new File("/storage", volumeId);
        return fallback.exists() ? fallback : null;
    }

    private File getStorageRootForExternalFilesDir(File externalFilesDir) {
        if (externalFilesDir == null) {
            return null;
        }

        String path = externalFilesDir.getAbsolutePath();
        int androidDir = path.indexOf("/Android/");
        if (androidDir <= 0) {
            return null;
        }

        return new File(path.substring(0, androidDir));
    }

    private void persistUriPermissions(Intent data) {
        if (data == null) {
            return;
        }

        int permissionFlags =
            data.getFlags() & (Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
        if (permissionFlags == 0) {
            return;
        }

        Uri uri = data.getData();
        if (uri != null) {
            persistUriPermission(uri, permissionFlags);
        }

        ClipData clipData = data.getClipData();
        if (clipData == null) {
            return;
        }
        for (int i = 0; i < clipData.getItemCount(); ++i) {
            Uri itemUri = clipData.getItemAt(i).getUri();
            if (itemUri != null) {
                persistUriPermission(itemUri, permissionFlags);
            }
        }
    }

    private void persistUriPermission(Uri uri, int permissionFlags) {
        if ((permissionFlags & Intent.FLAG_GRANT_READ_URI_PERMISSION) != 0) {
            persistUriPermission(uri, Intent.FLAG_GRANT_READ_URI_PERMISSION, "read");
        }
        if ((permissionFlags & Intent.FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
            persistUriPermission(uri, Intent.FLAG_GRANT_WRITE_URI_PERMISSION, "write");
        }
    }

    private void persistUriPermission(Uri uri, int permissionFlag, String permissionName) {
        try {
            getContentResolver().takePersistableUriPermission(uri, permissionFlag);
        } catch (SecurityException | IllegalArgumentException e) {
            Log.w(TAG, "Unable to persist " + permissionName + " URI permission for " + uri, e);
        }
    }

    public String getDisplayNameForUri(String uriString) {
        if (uriString == null || uriString.isEmpty()) {
            return "";
        }

        Uri uri = Uri.parse(uriString);
        if ("content".equals(uri.getScheme())) {
            try (Cursor cursor = getContentResolver().query(
                uri, new String[] { OpenableColumns.DISPLAY_NAME }, null, null, null))
            {
                if (cursor != null && cursor.moveToFirst()) {
                    int displayNameColumn = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                    if (displayNameColumn >= 0) {
                        String displayName = cursor.getString(displayNameColumn);
                        if (displayName != null && !displayName.isEmpty()) {
                            return displayName;
                        }
                    }
                }
            } catch (SecurityException | IllegalArgumentException e) {
                Log.w(TAG, "Unable to query display name for " + uri, e);
            }
        } else if ("file".equals(uri.getScheme())) {
            String path = uri.getPath();
            if (path != null && !path.isEmpty()) {
                String name = new File(path).getName();
                if (!name.isEmpty()) {
                    return name;
                }
            }
        }

        String lastSegment = uri.getLastPathSegment();
        return lastSegment != null ? lastSegment : "";
    }
}
