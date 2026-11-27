package ir.pigkeeper.dink;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.system.Os;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * «آخرین خوک‌چران» — Dink Smallwood remake.
 * Extracts bundled game data to internal storage on first run
 * (the engine reads plain files via stdio), sets up the Persian
 * font environment, then hands over to SDLActivity.
 */
public class MainActivity extends SDLActivity {

    private static final String TAG = "pigkeeper";
    /* bump when bundled data changes to force re-extraction */
    private static final String DATA_VERSION = "1";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        try {
            File dataDir = new File(getFilesDir(), "freedink-data");
            File marker = new File(dataDir, ".version");
            if (!markerMatches(marker)) {
                Log.i(TAG, "Extracting game data...");
                deleteRecursive(dataDir);
                copyAssetDir("freedink-data", dataDir);
                writeMarker(marker);
                Log.i(TAG, "Game data extracted to " + dataDir);
            }
            /* Persian-capable default font + sane engine defaults */
            Os.setenv("FREEDINK_FONT",
                    new File(dataDir, "fonts/Vazirmatn-Regular.ttf").getAbsolutePath(), true);
            Os.setenv("FREEDINK_AUTOSAVE", "180", true);
            Os.setenv("HOME", getFilesDir().getAbsolutePath(), true);
        } catch (Exception e) {
            Log.e(TAG, "asset extraction failed", e);
        }
        super.onCreate(savedInstanceState);
    }

    @Override
    protected String[] getArguments() {
        String refdir = new File(getFilesDir(), "freedink-data").getAbsolutePath();
        return new String[]{"--refdir", refdir, "--truecolor"};
    }

    @Override
    protected String[] getLibraries() {
        return new String[]{"SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "main"};
    }

    private boolean markerMatches(File marker) {
        try {
            if (!marker.exists()) return false;
            byte[] buf = new byte[16];
            InputStream in = new java.io.FileInputStream(marker);
            int n = in.read(buf);
            in.close();
            return n > 0 && new String(buf, 0, n).trim().equals(DATA_VERSION);
        } catch (Exception e) {
            return false;
        }
    }

    private void writeMarker(File marker) throws Exception {
        OutputStream out = new FileOutputStream(marker);
        out.write(DATA_VERSION.getBytes());
        out.close();
    }

    private void deleteRecursive(File f) {
        if (f.isDirectory()) {
            File[] kids = f.listFiles();
            if (kids != null)
                for (File k : kids) deleteRecursive(k);
        }
        f.delete();
    }

    private void copyAssetDir(String assetPath, File dest) throws Exception {
        AssetManager am = getAssets();
        String[] children = am.list(assetPath);
        if (children == null || children.length == 0) {
            /* it's a file */
            dest.getParentFile().mkdirs();
            InputStream in = am.open(assetPath);
            OutputStream out = new FileOutputStream(dest);
            byte[] buf = new byte[65536];
            int n;
            while ((n = in.read(buf)) > 0) out.write(buf, 0, n);
            out.close();
            in.close();
        } else {
            dest.mkdirs();
            for (String child : children)
                copyAssetDir(assetPath + "/" + child, new File(dest, child));
        }
    }
}
