APP_ABI := arm64-v8a
APP_PLATFORM := android-23
APP_STL := c++_shared
APP_CPPFLAGS := -std=gnu++14 -fexceptions -frtti

# SDL2_ttf: فونت TTF کافیه، هارف‌باز لازم نیست (شکل‌دهی فارسی داخل موتور ماست)
SUPPORT_HARFBUZZ := false

# SDL2_mixer: فقط فرمت‌های خود بازی (WAV + OGG/stb + MIDI/timidity داخلی)
SUPPORT_WAVPACK := false
SUPPORT_FLAC_LIBFLAC := false
SUPPORT_FLAC_DRFLAC := false
SUPPORT_OGG_VORBIS := false
SUPPORT_OGG_STB := true
SUPPORT_MP3_MPG123 := false
SUPPORT_MP3_DRMP3 := false
SUPPORT_MOD_XMP := false
SUPPORT_OPUS := false
SUPPORT_GME := false
SUPPORT_MID_TIMIDITY := true

# SDL2_image: بازی فقط BMP می‌خواند؛ همه کدک‌های خارجی خاموش
# (PNG در فاز ۴ برای اطلس‌ها با ماژول درست روشن می‌شود)
SUPPORT_AVIF := false
SUPPORT_JXL := false
SUPPORT_WEBP := false
SUPPORT_JPG := false
SUPPORT_PNG := false
