package ir.pigkeeper.dink;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Gravity;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * اسپلش + استخراج async اسِت‌ها (3-9) — حل ریسک ANR استخراج همگام.
 * پس از آماده شدن دیتا، MainActivity (SDL) را اجرا می‌کند.
 */
public class SplashActivity extends Activity {

    static final String DATA_VERSION = "2";

    @Override
    protected void onCreate(Bundle b) {
        super.onCreate(b);
        final File dataDir = new File(getFilesDir(), "freedink-data");
        final File marker = new File(dataDir, ".version");
        if (markerMatches(marker)) { startGame(); return; }

        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setGravity(Gravity.CENTER);
        root.setBackgroundColor(Color.BLACK);
        TextView title = new TextView(this);
        title.setText("آخرین خوک‌چران");
        title.setTextColor(Color.WHITE);
        title.setTextSize(24f);
        title.setGravity(Gravity.CENTER);
        ProgressBar pb = new ProgressBar(this);
        TextView msg = new TextView(this);
        msg.setText("در حال آماده‌سازی فایل‌های بازی…");
        msg.setTextColor(0xFFBBBBBB);
        msg.setTextSize(15f);
        msg.setGravity(Gravity.CENTER);
        root.addView(title);
        root.addView(pb);
        root.addView(msg);
        setContentView(root);

        new Thread(() -> {
            try {
                android.util.Log.i("pigkeeper", "Extracting game data...");
                deleteRecursive(dataDir);
                copyAssetDir("freedink-data", dataDir);
                OutputStream out = new FileOutputStream(marker);
                out.write(DATA_VERSION.getBytes());
                out.close();
                android.util.Log.i("pigkeeper", "Game data extracted to " + dataDir);
            } catch (Exception e) {
                android.util.Log.e("pigkeeper", "extraction failed", e);
            }
            runOnUiThread(this::startGame);
        }).start();
    }

    private void startGame() {
        Intent fwd = new Intent(this, MainActivity.class);
        if (getIntent() != null && getIntent().getExtras() != null) {
            fwd.putExtras(getIntent().getExtras());
        }
        startActivity(fwd);
        finish();
    }

    private boolean markerMatches(File marker) {
        try {
            if (!marker.exists()) return false;
            byte[] buf = new byte[16];
            InputStream in = new java.io.FileInputStream(marker);
            int n = in.read(buf);
            in.close();
            return n > 0 && new String(buf, 0, n).trim().equals(DATA_VERSION);
        } catch (Exception e) { return false; }
    }

    private void deleteRecursive(File f) {
        if (f.isDirectory()) {
            File[] kids = f.listFiles();
            if (kids != null) for (File k : kids) deleteRecursive(k);
        }
        f.delete();
    }

    private void copyAssetDir(String assetPath, File dest) throws Exception {
        AssetManager am = getAssets();
        String[] children = am.list(assetPath);
        if (children == null || children.length == 0) {
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
