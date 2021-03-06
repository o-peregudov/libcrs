AC_INIT([crs],[2.0.0],[o.peregudov@gmail.com])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_SILENT_RULES([yes])

dnl  Here are a set of rules to help you update your library version information:
dnl 1. Start with version information of '0:0:0' for each libtool library.
dnl 2. Update the version information only immediately before a public release of your soft-
dnl    ware. More frequent updates are unnecessary, and only guarantee that the current
dnl    interface number gets larger faster.
dnl 3. If the library source code has changed at all since the last update, then increment
dnl    revision ('c:r:a' becomes 'c:r + 1:a').
dnl 4. If any interfaces have been added, removed, or changed since the last update, increment
dnl    current, and set revision to 0.
dnl 5. If any interfaces have been added since the last public release, then increment age.
dnl 6. If any interfaces have been removed since the last public release, then set age to 0.

CRS_CURRENT=2
CRS_REVISION=0
CRS_AGE=0

CRS_LT_VERSION="${CRS_CURRENT}:${CRS_REVISION}:${CRS_AGE}"
AC_SUBST(CRS_LT_VERSION)

dnl
dnl Split VERSION into CRS_VERSION_MAJOR and CRS_VERSION_MINOR
dnl Follows AX_SPLIT_VERSION macro from AC-Archive
dnl Rhys Ulerich <rhys.ulerich@gmail.com>
dnl
AC_PROG_SED
CRS_MAJOR_VERSION=`echo "$VERSION" | $SED 's/\([[^.]][[^.]]*\).*/\1/'`
CRS_MINOR_VERSION=`echo "$VERSION" | $SED 's/[[^.]][[^.]]*.\([[^.]][[^.]]*\).*/\1/'`
AC_SUBST(CRS_MAJOR_VERSION)
AC_SUBST(CRS_MINOR_VERSION)

dnl CRS_CPPFLAGS=
dnl CRS_CFLAGS=
dnl CRS_LIBS=

AC_DEFINE_UNQUOTED(CONFIG_HOST_TYPE, "$host", [host type from configure])
AC_DEFINE_UNQUOTED(CONFIG_BUILD_TYPE, "$build", [build type from configure])
AC_DEFINE_UNQUOTED(LIBCRS_CONFIGURE_OPTIONS, "$ac_configure_args", [configure command line used to configure libcrs])

dnl
dnl Check for which system
dnl
AC_CANONICAL_BUILD
AC_CANONICAL_HOST

dnl
dnl DEFAULT_PREFIX
dnl
if test $prefix = NONE; then
  DEFAULT_PREFIX="/usr/local"
else
  DEFAULT_PREFIX="$prefix"
fi

if test $exec_prefix = NONE; then
  DEFAULT_EPREFIX="$DEFAULT_PREFIX"
else
  DEFAULT_EPREFIX="$exec_prefix"
fi

dnl
dnl Checks for programs
dnl
AM_PROG_AR
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

LT_INIT([dlopen])

dnl
dnl gcov/lcov stuff
dnl
AX_CODE_COVERAGE
if test $CODE_COVERAGE_ENABLED = yes; then
  CFLAGS="$CODE_COVERAGE_CFLAGS"
  CXXFLAGS="$CODE_COVERAGE_CFLAGS"
  CPPFLAGS="$CODE_COVERAGE_CPPFLAGS"
  LIBS="$CODE_COVERAGE_LDFLAGS"
fi

dnl ------------------------------------------------------------
dnl
dnl Check for host platform
dnl
dnl ------------------------------------------------------------
PLATFORM_LINUX=0
PLATFORM_MINGW=0
PLATFORM_CYGWIN=0

case "$host_os" in
  linux*)
    PLATFORM_LINUX=1
    ;;
  mingw*)
    PLATFORM_MINGW=1
    ;;
  cygwin*)
    PLATFORM_CYGWIN=1
    ;;
  *)
    AC_MSG_FAILURE([unsupported target platform - report to $PACKAGE_BUGREPORT])
    ;;
esac
AC_SUBST(PLATFORM_LINUX)
AC_SUBST(PLATFORM_MINGW)
AC_SUBST(PLATFORM_CYGWIN)

AM_CONDITIONAL([PLATFORM_LINUX], [test $PLATFORM_LINUX = 1] )
AM_CONDITIONAL([PLATFORM_MINGW], [test $PLATFORM_MINGW = 1] )
AM_CONDITIONAL([PLATFORM_CYGWIN], [test $PLATFORM_CYGWIN = 1] )

dnl
dnl check if compiler can be pedantic
dnl
AC_MSG_CHECKING([if $CXX supports -pedantic])
ac_pedantic_supported=no
save_CXXFLAGS="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -pedantic"
AC_LANG_PUSH([C++])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],
  [ac_pedantic_supported=yes],
  [ac_pedantic_supported=no])
AC_LANG_POP([C++])
CXXFLAGS="$save_CXXFLAGS"
AC_MSG_RESULT([$ac_pedantic_supported])
if test $ac_pedantic_supported = yes; then
  CXXFLAGS="$CXXFLAGS -pedantic"
fi

dnl
dnl check if compiler supports -Wall -Werror switches
dnl
AC_MSG_CHECKING([if $CXX supports -Wall -Werror])
ac_report_all_warnings_supported=no
save_CXXFLAGS="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -Wall -Werror"
AC_LANG_PUSH([C++])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],
  [ac_report_all_warnings_supported=yes],
  [ac_report_all_warnings_supported=no])
AC_LANG_POP([C++])
CXXFLAGS="$save_CXXFLAGS"
AC_MSG_RESULT([$ac_report_all_warnings_supported])
if test $ac_report_all_warnings_supported = yes; then
  CXXFLAGS="$CXXFLAGS -Wall -Werror"
fi

dnl
dnl Check for supported C++ standard
dnl
HAVE_CXX14=0
HAVE_CXX11=0
AX_CXX_COMPILE_STDCXX([14], [noext], [optional])
if test $HAVE_CXX14 -ne 1; then
  AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])
else
  HAVE_CXX11=1
fi
AC_SUBST(HAVE_CXX14)
AC_SUBST(HAVE_CXX11)

dnl
dnl We need special define to build static library only
dnl
if test $enable_shared = yes; then
  DEFINE_CRS_EXPORT=0
else
  DEFINE_CRS_EXPORT=1
fi
AC_SUBST(DEFINE_CRS_EXPORT)

dnl
dnl Check whether we have to compile Windows' resource file
dnl
if ((test $PLATFORM_MINGW = 1) || (test $PLATFORM_CYGWIN = 1)) && (test $enable_shared = yes); then
  LT_INIT([win32-dll])
  LT_LANG([Windows Resource])
  WINDOWS_RESOURCE=1
else
  WINDOWS_RESOURCE=0
fi
AC_SUBST(WINDOWS_RESOURCE)
AM_CONDITIONAL([WINDOWS_RESOURCE], [test $WINDOWS_RESOURCE = 1] )

dnl
dnl Checks for header files
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS([dlfcn.h	\
		  syslog.h	\
		  time.h	\
		  signal.h	\
		  pthread.h	\
		  windows.h	\
		  semaphore.h	])

dnl
dnl Check for integer types
dnl
if test $ac_cv_header_inttypes_h = yes; then
  HAVE_INTTYPES_H=1
else
  HAVE_INTTYPES_H=0
fi
AC_SUBST(HAVE_INTTYPES_H)

if test $ac_cv_header_stdint_h = yes; then
  HAVE_STDINT_H=1
else
  HAVE_STDINT_H=0
fi
AC_SUBST(HAVE_STDINT_H)

if test $ac_cv_header_sys_types_h = yes; then
  HAVE_SYS_TYPES_H=1
else
  HAVE_SYS_TYPES_H=0
fi
AC_SUBST(HAVE_SYS_TYPES_H)

if test $ac_cv_header_signal_h = yes; then
  HAVE_SIGNAL_H=1
else
  HAVE_SIGNAL_H=0
fi
AC_SUBST(HAVE_SIGNAL_H)

if test $ac_cv_header_time_h = yes; then
  HAVE_TIME_H=1
else
  HAVE_TIME_H_0
fi
AC_SUBST(HAVE_TIME_H)

if test $ac_cv_header_windows_h = yes; then
  HAVE_WINDOWS_H=1
else
  HAVE_WINDOWS_H=0
fi
AC_SUBST(HAVE_WINDOWS_H)

AX_PTHREAD([ac_pthread_lib_found=yes], [ac_pthread_lib_found=no])
if test $ac_pthread_lib_found = yes; then
  CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
  CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
  LIBS="$PTHREAD_LIBS $LIBS"
fi

HAVE_PTHREAD_SPIN_LOCK=0
if test $ac_cv_header_pthread_h = yes; then
  HAVE_PTHREAD_H=1
  AC_SEARCH_LIBS([pthread_create], [pthread])
  AC_CHECK_FUNCS([pthread_spin_lock])
  if test $ac_cv_func_pthread_spin_lock = yes ; then
    HAVE_PTHREAD_SPIN_LOCK=1
  fi
else
  HAVE_PTHREAD_H=0
fi
AC_SUBST(HAVE_PTHREAD_H)
AC_SUBST(HAVE_PTHREAD_SPIN_LOCK)
AM_CONDITIONAL([HAVE_PTHREAD_SPIN_LOCK], [test $HAVE_PTHREAD_SPIN_LOCK = 1])

HAVE_LIBGSL=0
AC_ARG_ENABLE(gsl,
    AS_HELP_STRING([--disable-gsl], [Disable use of GNU Scientific Library]),
    [do_check_gsl=$enable_gsl], [do_check_gsl=yes])
if test $do_check_gsl = yes; then
  dnl
  dnl Check for GSL
  dnl
  AX_PATH_GSL([1.14],[ac_have_libgsl=yes],[ac_have_libgsl=no])
  if test $ac_have_libgsl = yes; then
    CFLAGS="$CFLAGS $GSL_CFLAGS"
    CPPFLAGS="$CPPFLAGS -DGSL_RANGE_CHECK_OFF -DHAVE_INLINE"
    CXXFLAGS="$CXXFLAGS $GSL_CFLAGS"
    LIBS="$LIBS $GSL_LIBS"
    HAVE_LIBGSL=1
  fi
fi
AC_SUBST(HAVE_LIBGSL)

dnl ------------------------------------------------------------
dnl
dnl Check for target platform specific API
dnl
dnl ------------------------------------------------------------

dnl
dnl Check for Winsock2 only on MinGW, on Cygwin we must use emulated BSD socket API
dnl
HAVE_WINSOCK2_API=0
case "$host_os" in
  mingw*)
    AC_LANG_PUSH([C])
    save_LIBS="$LIBS"
    LIBS="$LIBS -lws2_32"
    AC_CACHE_CHECK(for winsock2, ac_cv_winsock2_works,
      AC_LINK_IFELSE([AC_LANG_SOURCE([[
        #include <winsock2.h>
        int main (int argc, char ** argv)
        {
          socket (0, 0, 0);
          select (0, NULL, NULL, NULL, NULL);
          closesocket (0);
          gethostname (NULL, 0);
          return 0;
        }
      ]])],[ac_cv_winsock2_works=yes],[ac_cv_winsock2_works=no]))
    LIBS="$save_LIBS"
    AC_LANG_POP([C])
    if test $ac_cv_winsock2_works = yes; then
      LIBS="$LIBS -lws2_32"
      CRS_LIBS="$CRS_LIBS -lws2_32"
      HAVE_WINSOCK2_API=1
    else
      LIBS="$save_LIBS"
      AC_MSG_WARN([failed to link with winsock2])
    fi
    ;;
  *)
    ;;
esac
AC_SUBST(HAVE_WINSOCK2_API)
AM_CONDITIONAL([HAVE_WINSOCK2_API], [test $HAVE_WINSOCK2_API = 1] )

dnl
dnl Check whether we have epoll () support on Linux
dnl
AX_HAVE_EPOLL([HAVE_EPOLL=1], [HAVE_EPOLL=0])
AC_SUBST(HAVE_EPOLL)

dnl
dnl Check for poll () on POSIX systems
dnl
AX_HAVE_POLL([HAVE_POLL=1], [HAVE_POLL=0])
AC_SUBST(HAVE_POLL)

dnl
dnl Check for select () on POSIX systems
dnl
AX_HAVE_SELECT([HAVE_SELECT=1], [HAVE_SELECT=0])
AC_SUBST(HAVE_SELECT)

if test "x$ac_cv_winsock2_works" = "xyes" ; then
  SELECT_TYPE="winsock2"
  AC_DEFINE(WINSOCK2_IO_MULTIPLEXING, 1, [Define to 1 if winsock2 should be used for I/O multiplexing])
elif test $HAVE_EPOLL = 1 ; then
  SELECT_TYPE="epoll"
  AC_DEFINE(EPOLL_IO_MULTIPLEXING, 1, [Define to 1 if epoll should be used for I/O multiplexing])
elif test $HAVE_POLL = 1 ; then
  SELECT_TYPE="poll"
  AC_DEFINE(POLL_IO_MULTIPLEXING, 1, [Define to 1 if poll should be used for I/O multiplexing])
elif test $HAVE_SELECT = 1 ; then
  SELECT_TYPE="select"
  AC_DEFINE(SELECT_IO_MULTIPLEXING_POSIX, 1, [Define to 1 if POSIX select should be used for I/O multiplexing])
else
  AC_MSG_ERROR([I/O multiplexing method can't be detected])
fi
AC_DEFINE_UNQUOTED(SELECT_TYPE, "$SELECT_TYPE", [Define to inform about available I/O multiplexing method])

dnl
dnl auto-detect and verify <atomic> header present and really works
dnl
USE_CXX11_ATOMIC=0
AC_LANG_PUSH([C++])
AC_CACHE_CHECK(whether <atomic> is usable, ac_cv_std_atomic_works,
  AC_LINK_IFELSE([AC_LANG_SOURCE([[
    #include <atomic>
    int main (int argc, char ** argv)
    {
      std::atomic<bool> flag_bool (false);
      return (flag_bool.is_lock_free () ? 0 : 1);
    }
  ]])],[ac_cv_std_atomic_works=yes],[ac_cv_std_atomic_works=no],[:]))
AC_LANG_POP([C++])
if test x$ac_cv_std_atomic_works = xyes ; then
  USE_CXX11_ATOMIC=1
else
  AC_MSG_ERROR([ISO C++ 2011 standard <atomic> is required])
fi
AC_SUBST(USE_CXX11_ATOMIC)

dnl
dnl auto-detect and verify <chrono> header present and really works
dnl
AC_LANG_PUSH([C++])
AC_CACHE_CHECK(whether <chrono> is usable, ac_cv_std_chrono_works,
  AC_LINK_IFELSE([AC_LANG_SOURCE([[
    #include <chrono>
    int main (int argc, char ** argv)
    {
       typedef std::chrono::high_resolution_clock clock_type;
       std::chrono::time_point<clock_type> startTime = clock_type::now ();
       clock_type::duration usExeTime = clock_type::now () - startTime;
       return 0;
    }
  ]])],[ac_cv_std_chrono_works=yes],[ac_cv_std_chrono_works=no],[:]))
AC_LANG_POP([C++])
if test $ac_cv_std_chrono_works = yes ; then
  USE_CXX11_CHRONO=1
else
  AC_MSG_ERROR([ISO C++ 2011 standard <chrono> is required])
fi
AC_SUBST(USE_CXX11_CHRONO)

dnl
dnl auto-detect and verify <mutex> header present and really works
dnl
USE_CXX11_MUTEX=0
USE_CXX11_CONDITION_VARIABLE=0
AC_LANG_PUSH([C++])
AC_CACHE_CHECK(whether <mutex> is usable, ac_cv_std_mutex_works,
  AC_LINK_IFELSE([AC_LANG_SOURCE([[
    #include <mutex>
    int main (int argc, char ** argv)
    {
      std::mutex mutex;
      std::unique_lock<std::mutex> lock (mutex);
      return ((mutex.try_lock () == false) ? 0 : 1);
    }
  ]])],[ac_cv_std_mutex_works=yes],[ac_cv_std_mutex_works=no],[:]))
AC_LANG_POP([C++])
if test x$ac_cv_std_mutex_works = xyes ; then
  USE_CXX11_MUTEX=1

  dnl
  dnl auto-detect and verify <condition_variable> header present and really works
  dnl
  AC_LANG_PUSH([C++])
  AC_CACHE_CHECK(whether <condition_variable> is usable, ac_cv_std_condition_variable_works,
    AC_LINK_IFELSE([AC_LANG_SOURCE([[
      #include <mutex>
      #include <condition_variable>
      int main (int argc, char ** argv)
      {
        std::mutex mutex;
        std::condition_variable condition;
        std::unique_lock<std::mutex> lock (mutex);
        condition.notify_one ();
        return 0;
      }
    ]])],[ac_cv_std_condition_variable_works=yes],[ac_cv_std_condition_variable_works=no],[:]))
  AC_LANG_POP([C++])
  if test x$ac_cv_std_condition_variable_works = xyes ; then
    USE_CXX11_CONDITION_VARIABLE=1
  else
    AC_MSG_ERROR([ISO C++ 2011 standard <condition_variable> is required])
  fi
else
  AC_MSG_ERROR([ISO C++ 2011 standard <mutex> is required])
fi
AC_SUBST(USE_CXX11_MUTEX)
AC_SUBST(USE_CXX11_CONDITION_VARIABLE)

dnl
dnl libcrs flags
dnl
CRS_CFLAGS="$CRS_CFLAGS -I$includedir"
CRS_CXXFLAGS="$CRS_CXXFLAGS -I$includedir"
CRS_LIBS="$CRS_LIBS -L$libdir -lcrs"
AC_SUBST(CRS_CFLAGS)
AC_SUBST(CRS_CXXFLAGS)
AC_SUBST(CRS_LIBS)

dnl
dnl Check for types
dnl
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(int8_t)
AC_CHECK_SIZEOF(uint8_t)
AC_CHECK_SIZEOF(u_int8_t)

AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int16_t)
AC_CHECK_SIZEOF(uint16_t)
AC_CHECK_SIZEOF(u_int16_t)

AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(int32_t)
AC_CHECK_SIZEOF(uint32_t)
AC_CHECK_SIZEOF(u_int32_t)

AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(int64_t)
AC_CHECK_SIZEOF(__int64)

AC_CHECK_SIZEOF(void*)

case 1 in
  $ac_cv_sizeof_char) SIZE8="char";;
  $ac_cv_sizeof_int8_t) SIZE8="int8_t";;
esac

case 1 in
  $ac_cv_sizeof_char) USIZE8="unsigned char";;
  $ac_cv_sizeof_uint8_t) USIZE8="uint8_t";;
  $ac_cv_sizeof_u_int8_t) USIZE8="u_int8_t";;
esac

case 2 in
  $ac_cv_sizeof_short) SIZE16="short";;
  $ac_cv_sizeof_int) SIZE16="int";;
  $ac_cv_sizeof_int16_t) SIZE16="int16_t";;
esac

case 2 in
  $ac_cv_sizeof_short) USIZE16="unsigned short";;
  $ac_cv_sizeof_int) USIZE16="unsigned int";;
  $ac_cv_sizeof_u_int16_t) USIZE16="u_int16_t";;
  $ac_cv_sizeof_uint16_t) USIZE16="uint16_t";;
esac

case 4 in
  $ac_cv_sizeof_short) SIZE32="short";;
  $ac_cv_sizeof_int) SIZE32="int";;
  $ac_cv_sizeof_long) SIZE32="long";;
  $ac_cv_sizeof_int32_t) SIZE32="int32_t";;
esac

case 4 in
  $ac_cv_sizeof_short) USIZE32="unsigned short";;
  $ac_cv_sizeof_int) USIZE32="unsigned int";;
  $ac_cv_sizeof_long) USIZE32="unsigned long";;
  $ac_cv_sizeof_u_int32_t) USIZE32="u_int32_t";;
  $ac_cv_sizeof_uint32_t) USIZE32="uint32_t";;
esac

case 8 in
  $ac_cv_sizeof_int) SIZE64="int";;
  $ac_cv_sizeof_long) SIZE64="long";;
  $ac_cv_sizeof_long_long) SIZE64="long long";;
  $ac_cv_sizeof_int64_t) SIZE64="int64_t";;
  $ac_cv_sizeof___int64) SIZE64="__int64";;
esac

if test -z "$SIZE8"; then
  AC_MSG_ERROR(No 8 bit type found on this platform!)
fi
if test -z "$USIZE8"; then
  AC_MSG_ERROR(No unsigned 8 bit type found on this platform!)
fi
if test -z "$SIZE16"; then
  AC_MSG_ERROR(No 16 bit type found on this platform!)
fi
if test -z "$USIZE16"; then
  AC_MSG_ERROR(No unsigned 16 bit type found on this platform!)
fi
if test -z "$SIZE32"; then
  AC_MSG_ERROR(No 32 bit type found on this platform!)
fi
if test -z "$USIZE32"; then
  AC_MSG_ERROR(No unsigned 32 bit type found on this platform!)
fi
if test -z "$SIZE64"; then
  AC_MSG_ERROR(No 64 bit type found on this platform!)
fi

dnl Make substitutions
AC_SUBST(SIZE8)
AC_SUBST(USIZE8)
AC_SUBST(SIZE16)
AC_SUBST(USIZE16)
AC_SUBST(SIZE32)
AC_SUBST(USIZE32)
AC_SUBST(SIZE64)

AC_ARG_ENABLE(destructor-exceptions,
[  --disable-destructor-exceptions  Don't throw exceptions from destructors.
  --enable-destructor-exceptions   Throw all exceptions (even from destructors).])
if test x$enable_destructor_exceptions != xno; then
  AC_DEFINE(DESTRUCTOR_EXCEPTIONS_ALLOWED, 1, [Define to 1 if you want to get all exceptions (even from destructors).])
fi

AC_MSG_NOTICE(["${SELECT_TYPE}" will be used for the internal I/O loops])
AC_CONFIG_FILES([
	Makefile
	crs.pc
	crs/defsys.h
	crs/crs-version.h
	crs/Makefile
	test/Makefile
])
AC_OUTPUT
