package dev.twilitrealm.dusk;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import dev.encounter.borealis.BorealisActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class DuskActivity extends BorealisActivity {
    private static final String TAG = "DuskActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        extractBundledMods();
        super.onCreate(savedInstanceState);
    }

    // Bundled mod packages ship as APK assets, which the native loader cannot read directly;
    // mirror them into internal storage (the loader's CachePath/bundled_mods search dir)
    // before SDL_main starts.
    private void extractBundledMods() {
        File outDir = new File(getFilesDir(), "bundled_mods");
        try {
            deleteRecursively(outDir); // drop packages removed by an app update
            String[] names = getAssets().list("mods");
            if (names == null || names.length == 0) {
                return;
            }
            if (!outDir.mkdirs()) {
                Log.w(TAG, "Unable to create " + outDir);
                return;
            }
            byte[] buffer = new byte[65536];
            for (String name : names) {
                if (!name.endsWith(".dusk")) {
                    continue;
                }
                try (InputStream in = getAssets().open("mods/" + name);
                     OutputStream out = new FileOutputStream(new File(outDir, name)))
                {
                    int count;
                    while ((count = in.read(buffer)) > 0) {
                        out.write(buffer, 0, count);
                    }
                }
            }
        } catch (IOException e) {
            Log.w(TAG, "Failed to extract bundled mods", e);
        }
    }

    private static void deleteRecursively(File file) {
        File[] children = file.listFiles();
        if (children != null) {
            for (File child : children) {
                deleteRecursively(child);
            }
        }
        file.delete();
    }

    @Override
    protected String[] getArguments() {
        String[] arguments = super.getArguments();
        if (arguments.length > 0) {
            return arguments;
        }

        Intent intent = getIntent();
        if (intent == null) {
            return arguments;
        }
        String[] argv = intent.getStringArrayExtra("dusk_argv");
        if (argv != null && argv.length > 0) {
            return argv;
        }
        String rawArgs = intent.getStringExtra("dusk_args");
        return rawArgs == null ? arguments : splitArguments(rawArgs.trim());
    }

}
