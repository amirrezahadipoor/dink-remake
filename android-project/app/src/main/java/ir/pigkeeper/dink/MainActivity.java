package ir.pigkeeper.dink;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.system.Os;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;

/**
 * «آخرین خوک‌چران» — فعالیت SDL.
 * استخراج دیتا در SplashActivity انجام می‌شود (3-9)؛ اینجا فقط
 * محیط موتور (فونت فارسی، اتوسیو، تنظیمات لمس 3-8) ست می‌شود.
 */
public class MainActivity extends SDLActivity {

    private static final String TAG = "pigkeeper";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        try {
            File dataDir = new File(getFilesDir(), "freedink-data");
            Os.setenv("FREEDINK_FONT",
                    new File(dataDir, "fonts/Vazirmatn-Regular.ttf").getAbsolutePath(), true);
            Os.setenv("FREEDINK_AUTOSAVE", "180", true);
            Os.setenv("HOME", getFilesDir().getAbsolutePath(), true);
            /* 3-8: تنظیمات لمس از SharedPreferences (UI کامل در M12) */
            SharedPreferences p = getSharedPreferences("pigkeeper", MODE_PRIVATE);
            Os.setenv("FREEDINK_TOUCH", "1", true);
            Os.setenv("FREEDINK_TOUCH_SCALE", p.getString("touch_scale", "1.0"), true);
            Os.setenv("FREEDINK_TOUCH_LEFTY", p.getBoolean("touch_lefty", false) ? "1" : "0", true);
            /* فقط CI: زنجیره دمو با extra (کاربر عادی هرگز ست نمی‌شود) */
            String ci = getIntent() != null ? getIntent().getStringExtra("ci") : null;
            if (ci != null) {
                Os.setenv("PIG_DEMO_STOP", ci, true);
                Log.i(TAG, "CI demo mode: PIG_DEMO_STOP=" + ci);
            }
        } catch (Exception e) {
            Log.e(TAG, "env setup failed", e);
        }
        super.onCreate(savedInstanceState);
    }

    @Override
    protected String[] getArguments() {
        String refdir = new File(getFilesDir(), "freedink-data").getAbsolutePath();
        String game = new File(getFilesDir(), "freedink-data/pigkeeper").getAbsolutePath();
        return new String[]{"--refdir", refdir, "--game", game, "--truecolor"};
    }

    @Override
    protected String[] getLibraries() {
        return new String[]{"SDL2", "SDL2_image", "SDL2_mixer", "SDL2_ttf", "main"};
    }
}
