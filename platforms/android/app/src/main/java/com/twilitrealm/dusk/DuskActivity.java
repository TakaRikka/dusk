package com.twilitrealm.dusk;

import android.app.ActionBar;
import android.content.Intent;
import android.content.UriPermission;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;

import org.libsdl.app.SDLActivity;

import java.util.ArrayList;
import java.util.List;

public class DuskActivity extends SDLActivity {
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

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowInsetsController ctrl = getWindow().getDecorView().getWindowInsetsController();
            if(ctrl != null)
                ctrl.hide(WindowInsets.Type.systemBars());
        }else {
            View decorView = getWindow().getDecorView();
            // Hide the status bar.
            int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
            decorView.setSystemUiVisibility(uiOptions);
            // Remember that you should never show the action bar if the
            // status bar is hidden, so hide that too if necessary.
            ActionBar actionBar = getActionBar();
            if(actionBar != null)
                actionBar.hide();
        }
    }

    @Override
    protected String[] getLibraries() {
        // SDL3 is statically linked into libmain.so in this build.
        return new String[] {
            "main"
        };
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

            String discPath = intent.getStringExtra("dusk_disc");
            if (discPath != null && !discPath.isEmpty()) {
                return new String[] { discPath };
            }
        }
        return new String[0];
    }

    // Called by JNI from Dusk.
    public static void takeUriPermissions(String uri) {
        if(mSingleton != null) {
            mSingleton.getContentResolver().takePersistableUriPermission(Uri.parse(uri), Intent.FLAG_GRANT_READ_URI_PERMISSION);
            System.out.println("Saved uri permissions.");
        }else {
            System.out.println("Unable to save uri permissions.");
        }
    }

    // Called by JNI from Dusk.
    public static boolean checkUriPermissions(String uri) {
        if(mSingleton != null) {
            Uri suppliedUri = Uri.parse(uri);

            System.out.println("Checking uri permissions.");
            for (UriPermission permission : mSingleton.getContentResolver().getPersistedUriPermissions()) {
                if(permission.getUri().equals(suppliedUri) && permission.isReadPermission()) {
                    System.out.println("Uri has valid persistent permissions.");
                    return true;
                }
            }

            System.out.println("Uri permission was not persisted, unable to use.");
            return false;
        }
        System.out.println("Unable to check uri permissions.");
        return false;
    }
}
