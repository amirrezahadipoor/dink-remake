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
/* unused on Android (we always pass --refdir), but referenced in paths.cpp */
#define BUILD_DATA_DIR "/usr/local/share"

/* gnulib attribute macros (normally emitted by autotools config.h) */
#ifndef _GL_ATTRIBUTE_PURE
# define _GL_ATTRIBUTE_PURE __attribute__((__pure__))
#endif
#ifndef _GL_ATTRIBUTE_CONST
# define _GL_ATTRIBUTE_CONST __attribute__((__const__))
#endif
#ifndef _GL_ATTRIBUTE_MALLOC
# define _GL_ATTRIBUTE_MALLOC __attribute__((__malloc__))
#endif
#ifndef _GL_ATTRIBUTE_WUR
# define _GL_ATTRIBUTE_WUR __attribute__((__warn_unused_result__))
#endif
#ifndef _GL_ATTRIBUTE_DEPRECATED
# define _GL_ATTRIBUTE_DEPRECATED
#endif
#ifndef _GL_ATTRIBUTE_NOTHROW
# define _GL_ATTRIBUTE_NOTHROW
#endif
#ifndef _GL_INLINE_HEADER_BEGIN
# define _GL_INLINE_HEADER_BEGIN
#endif
#ifndef _GL_INLINE_HEADER_END
# define _GL_INLINE_HEADER_END
#endif
#ifndef _GL_INLINE
# define _GL_INLINE static inline
#endif
#ifndef _GL_EXTERN_INLINE
# define _GL_EXTERN_INLINE static inline
#endif
#ifndef _GL_UNUSED
# define _GL_UNUSED __attribute__((__unused__))
#endif
#if defined __cplusplus && !defined _Noreturn
# define _Noreturn __attribute__((__noreturn__))
#endif

#endif
