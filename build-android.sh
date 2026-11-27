#!/usr/bin/env bash
# 3-11: one-command local Android build (mirrors .github/workflows/android.yml).
# Requires: ANDROID_HOME with SDK+NDK 26, gradle, curl, java 17.
set -e
cd "$(dirname "$0")"

SDL_VER=2.30.9; IMG_VER=2.8.2; MIX_VER=2.8.0; TTF_VER=2.22.0; GFX_VER=1.0.4

mkdir -p sdl-src && cd sdl-src
[ -f SDL2-$SDL_VER.tar.gz ]        || curl -sLO https://github.com/libsdl-org/SDL/releases/download/release-$SDL_VER/SDL2-$SDL_VER.tar.gz
[ -f SDL2_image-$IMG_VER.tar.gz ]  || curl -sLO https://github.com/libsdl-org/SDL_image/releases/download/release-$IMG_VER/SDL2_image-$IMG_VER.tar.gz
[ -f SDL2_mixer-$MIX_VER.tar.gz ]  || curl -sLO https://github.com/libsdl-org/SDL_mixer/releases/download/release-$MIX_VER/SDL2_mixer-$MIX_VER.tar.gz
[ -f SDL2_ttf-$TTF_VER.tar.gz ]    || curl -sLO https://github.com/libsdl-org/SDL_ttf/releases/download/release-$TTF_VER/SDL2_ttf-$TTF_VER.tar.gz
[ -f SDL2_gfx-$GFX_VER.tar.gz ]    || curl -sL -o SDL2_gfx-$GFX_VER.tar.gz "https://downloads.sourceforge.net/project/sdl2gfx/SDL2_gfx-$GFX_VER.tar.gz"
[ -f glm.tar.gz ]                  || curl -sL -o glm.tar.gz https://github.com/g-truc/glm/archive/refs/tags/1.0.1.tar.gz
for t in *.tar.gz; do tar xzf "$t"; done
cd ..

J=android-project/app/jni
ln -sfn "$PWD/sdl-src/SDL2-$SDL_VER"       $J/SDL
ln -sfn "$PWD/sdl-src/SDL2_image-$IMG_VER" $J/SDL2_image
ln -sfn "$PWD/sdl-src/SDL2_mixer-$MIX_VER" $J/SDL2_mixer
ln -sfn "$PWD/sdl-src/SDL2_ttf-$TTF_VER"   $J/SDL2_ttf
ln -sfn "$PWD/sdl-src/SDL2_gfx-$GFX_VER"   $J/SDL2_gfx
ln -sfn "$PWD/sdl-src/glm-1.0.1/glm"       $J/glm
mkdir -p android-project/app/src/main/java/org/libsdl/app
cp sdl-src/SDL2-$SDL_VER/android-project/app/src/main/java/org/libsdl/app/*.java \
   android-project/app/src/main/java/org/libsdl/app/

sh fetch-data.sh
A=android-project/app/src/main/assets/freedink-data
rm -rf "$A"; mkdir -p "$A"
cp -r gamedata/dink "$A/dink"
cp -r pigkeeper "$A/pigkeeper"
cp gamedata/dink/Dink.dat "$A/pigkeeper/dink.dat"
cp gamedata/dink/Map.dat  "$A/pigkeeper/map.dat"
cp gamedata/dink/Hard.dat "$A/pigkeeper/hard.dat"
mkdir -p "$A/fonts"; cp fonts/Vazirmatn-Regular.ttf "$A/fonts/"

cd android-project && gradle --no-daemon assembleDebug "$@"
echo "APK: android-project/app/build/outputs/apk/debug/app-debug.apk"
