package com.axiodl.metaforce;

import android.content.Intent;

import org.libsdl.app.SDLActivity;

import java.util.ArrayList;
import java.util.List;

public class MetaforceActivity extends SDLActivity {
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
            String[] argv = intent.getStringArrayExtra("metaforce_argv");
            if (argv != null && argv.length > 0) {
                return argv;
            }

            String rawArgs = intent.getStringExtra("metaforce_args");
            if (rawArgs != null) {
                String trimmed = rawArgs.trim();
                if (!trimmed.isEmpty()) {
                    return splitArgs(trimmed);
                }
            }

            String discPath = intent.getStringExtra("metaforce_disc");
            if (discPath != null && !discPath.isEmpty()) {
                return new String[] { discPath };
            }
        }
        return new String[0];
    }
}
