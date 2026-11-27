/* Hand-written config.h for the Android (bionic/NDK) build of the
   FreeDink engine — remake phase 3. Desktop builds keep autotools. */
#ifndef ANDROID_CONFIG_H
#define ANDROID_CONFIG_H

#define PACKAGE "freedink"
#define PACKAGE_NAME "GNU FreeDink (Pigkeeper remake)"
#define PACKAGE_STRING "freedink 109.6-remake"
#define PACKAGE_VERSION "109.6-remake"
#define VERSION "109.6-remake"
#define PACKAGE_BUGREPORT "https://github.com/amirrezahadipoor/dink-remake"

/* bionic has all of these */
#define HAVE_STDINT_H 1
#define HAVE_UNISTD_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_FCNTL_H 1
#define HAVE_MMAP 1
#define HAVE_ALLOCA 1
#define HAVE_ALLOCA_H 1
#define HAVE_GETCWD 1
#define HAVE_STRCASECMP 1
#define HAVE_STRNCASECMP 1
#define HAVE_VASPRINTF 1
#define HAVE_ASPRINTF 1

/* explicitly absent on Android */
/* #undef HAVE_FONTCONFIG */
/* #undef ENABLE_NLS */

#define LOCALEDIR "/sdcard/nonexistent-locale"

#endif
