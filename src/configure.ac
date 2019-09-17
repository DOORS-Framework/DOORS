AC_PREREQ([2.50])

# Process this file with autoconf to produce a configure script.
# Make sure that the files configure.ac, aclocal.m4 and each
# of the Makefile.am files are older than configure, doorsconf.h.in
# and each of the Makefile.in files to prevent unnecessary calls
# to configure during compile time.

# Set version information to AC_INIT and AM_INIT_AUTOMAKE

AC_INIT(DOORS, 0.6.0, bilhanan@cs.tut.fi)

AC_CONFIG_SRCDIR([base/base.C])
#AM_CONFIG_HEADER(doors/doorsconf.h)
AC_CONFIG_HEADERS([include/doors/doorsconf.h])

# We do not use AM_INIT_AUTOMAKE but do its stuff manually because
# we don't want configure to find different (older) versions of
# autoconf tools. It seems that even though Makefile.in-files are
# not changed, Makefile-files are still rewritten. In other words,
# dependency does not work as it should. At this point, no reason
# was found for this so we force all the autoconf tools to be
# missing in the following code (Makefiles still get rewritten by
# config.status but at least it compiles in the target machine with
# older versions of auto*).
AM_INIT_AUTOMAKE
#VERSION=0.6.0
#PACKAGE=doors
#AC_SUBST(VERSION)
#AC_SUBST(PACKAGE)
#AM_SANITY_CHECK
#AC_PROG_INSTALL
#ACLOCAL="`cd $ac_aux_dir && pwd`/missing aclocal > /dev/null 2>&1" 
#AUTOCONF="`cd $ac_aux_dir && pwd`/missing autoconf > /dev/null 2>&1 "
#AUTOMAKE="`cd $ac_aux_dir && pwd`/missing automake > /dev/null 2>&1"
#AUTOHEADER="`cd $ac_aux_dir && pwd`/missing autoheader > /dev/null 2>&1"
#MEKEINFO="`cd $ac_aux_dir && pwd`/missing makeinfo > /dev/null 2>&1"
#AC_SUBST(ACLOCAL)
#AC_SUBST(AUTOCONF)
#AC_SUBST(AUTOMAKE)
#AC_SUBST(AUTOHEADER)
#AC_SUBST(MAKEINFO)

# Find a strip-capable install program
AM_PROG_INSTALL_STRIP

# Target subdirectories to compile. The others are dependend 
# on the libraries to be found in the tests after this.

ROOTSUBS='include base tools-offline protocols tools-online apps' 
BASESUBS='dev hsi ptb sys util'
CORBASUBS='broker services'
ONTOOLSSUBS='lemon' 
OFFTOOLSSUBS='xcg'
PROTOSUBS='rtp rtcp udp phys_virtual link sap tcpmux tcp6mux altbit'
APPSUBS='udp link sap tcpmux tcp6mux altbit'
MANAGERSUBS=
AC_SUBST(ROOTSUBS)
AC_SUBST(BASESUBS)
AC_SUBST(CORBASUBS)
AC_SUBST(APPSUBS)
AC_SUBST(ONTOOLSSUBS)
AC_SUBST(OFFTOOLSSUBS)
AC_SUBST(PROTOSUBS)
AC_SUBST(MANAGERSUBS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_LN_S
#AC_PROG_RANLIB

AM_PROG_LIBTOOL

# Use C++-compiler in the tests
AC_LANG([C++])

dnl gethostbyname is sometimes in libnsl
AC_CHECK_FUNC(gethostbyname, , AC_CHECK_LIB(nsl, gethostbyname))

dnl getsockopt is sometimes in libsocket
AC_CHECK_FUNC(getsockopt, , AC_CHECK_LIB(socket, getsockopt))

dnl math library used by Mico and rtp; this is possibly removed in future
AC_CHECK_FUNC(sqrt, , AC_CHECK_LIB(m, sqrt))

dnl check for various libc functions
AC_CHECK_FUNCS([isinf isnan _stat class finite fp_class fpclass printf fprintf ftime isnand signal snprintf sprintf sscanf stat strdup strndup vfprintf vsnprintf vsprintf])

dnl check for pthread library
AC_CHECK_LIB(pthread, pthread_create)

dnl check for libz
AC_CHECK_LIB(z, zlibVersion)

dnl Check for the west
AC_MSG_CHECKING([whether the west is the best])
AC_MSG_RESULT([yes])

dnl Check for Qt
QTLIBS=-lqt
QTINCS=
_qtfound=no
AC_ARG_WITH(qt,
[  --with-qt=DIR           Qt root directory],
[
 QTLIBS="-L${withval}/lib -lqt-mt"
 QTINCS="-DQT_THREAD_SUPPORT -I${withval}/include"
],[])
AC_SUBST(QTLIBS)
AC_SUBST(QTINCS)
_libs="${LIBS}"
LIBS="${LIBS} ${QTLIBS}"
_cppflags="${CPPFLAGS}"
CPPFLAGS="${CPPFLAGS} ${QTINCS}"
QTMOC=
AC_CHECK_HEADER([qobject.h])
AC_CHECK_PROG([QTMOC],[moc],[moc])
if test x"${ac_cv_header_qobject_h}" = xyes && test x"${QTMOC}" != x""; then
AC_MSG_CHECKING([whether Qt source compiles])
AC_TRY_COMPILE([
#include <qapplication.h>
#include <qwaitcondition.h>
],[
int argc;
char ** argv;
QApplication a( argc, argv );
QWaitCondition cond;
a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
],[
AC_MSG_RESULT([yes])
_qtfound=yes
MANAGERSUBS=qt
AC_DEFINE([_CF_WITH_QT],1,[Define to 1 if Qt library is found.])
],[
AC_MSG_RESULT([no])
AC_DEFINE([_CF_WITH_QT],0,[Define to 0 if Qt library is not found.])
])
fi
LIBS=${_libs}
CPPFLAGS=${_cppflags}
AC_SUBST(QTMOC)
AM_CONDITIONAL(_CF_WITH_QT, test x"${_qtfound}" = xyes)

dnl Check for SLP library
AC_CHECK_HEADER([slp.h])
AC_CHECK_FUNC(SLPOpen, , AC_CHECK_LIB(slp, SLPOpen))
_slpfound=no
if test x"${ac_cv_func_SLPOpen}" = xyes || test x"${ac_cv_lib_slp_SLPOpen}" = xyes; then
  if test x"${ac_cv_header_slp_h}" = xyes; then
    _slpfound=yes
  fi
fi
if test x"${_slpfound}" = xyes; then
  AC_DEFINE([HAVE_SLP], 1, [Define to 1 if SLP library is found.])
else
  AC_MSG_WARN([SLP library not found. Not building modules depending on it.])
  AC_DEFINE([HAVE_SLP], 0, [Define to 0 if SLP library is not found.])
fi

dnl LDAP-library
_ldapfound=no
AC_CHECK_HEADER([ldap.h])
AC_CHECK_FUNC(ldap_init, , AC_CHECK_LIB(ldap, ldap_init))
if test x"${ac_cv_func_ldap_init}" = xyes || test x"${ac_cv_lib_ldap_ldap_init}" = xyes; then
  # Fine but if you are unhappy, you use OpenLDAP; check for extra libs
  AC_CHECK_FUNC(ber_free, , AC_CHECK_LIB(lber, ber_free))
  if test x"${ac_cv_header_ldap_h}" = xyes; then
    _ldapfound=yes
  fi
fi

if test x"${_ldapfound}" = xno; then
  AC_MSG_WARN([LDAP library not found. Not building modules depending on it.])
fi
dnl Check for Mico
dnl Because Mico is only needed in some parts of DOORS, create
dnl MICOLIBS and MICOINCS variables for them
dnl NOTE: Because Mico will be temporary stuff here no checking is done!
AC_ARG_WITH(mico-version,
[  --with-mico-version=VER set Mico version [default=2.3.5]],
[mico_version="$withval"],[mico_version="2.3.5"])
MICOLIBS=
MICOINCS=
IDL=
AC_ARG_WITH(mico,
[  --with-mico=DIR         compile with Mico [default=no]],
[
 mico_library="$withval"
 case "$mico_library" in
 no)
  AC_DEFINE([_CF_WITH_MICO],0,[Define to 0 if Mico not found.])
  ;;
 yes)
  MICOLIBS="-lmico${mico_version}"
  AC_DEFINE([_CF_WITH_MICO],1,[Define to 1 if Mico is found.])
  AC_CHECK_PROG(IDL,idl,idl)
  ;;
 *)
  MICOINCS="-I${mico_library}/include"
  MICOLIBS="-L${mico_library}/lib -lmico${mico_version}"
  AC_DEFINE([_CF_WITH_MICO],1,[Define to 1 if Mico is found.])
  _path=${PATH}
  PATH=${mico_library}/bin
  AC_CHECK_PROG(IDL,idl,${mico_library}/bin/idl)
  PATH=${_path}
  ;;
 esac
],[AC_DEFINE([_CF_WITH_MICO],0,[Define to 0 if Mico not found.])])
AC_SUBST(MICOLIBS)
AC_SUBST(MICOINCS)
AC_SUBST(IDL)

_micofound=no
if test x"${MICOLIBS}" != x"" && test x"${MICOINCS}" != x"" && test x"${IDL}" != x""; then
  _micofound=yes
fi

dnl Check for --enable-programs
_compile_daemons=no
AC_ARG_ENABLE(daemons,[  --enable-daemons       build the CORBA daemons [default=yes]],
[
 case "$enableval" in
  no)
  ;;
  yes)
   _compile_daemons=yes
  ;;
 esac
],[_compile_daemons=yes])

# If Mico, SLP and LDAP were found, compile applications that use them
AC_MSG_CHECKING([whether to compile applications that require SLP, LDAP and Mico])
if test x"${_micofound}" = xyes && test x"${_ldapfound}" = xyes && test x"${_slpfound}" = xyes; then
  # CORBA-library must be compiled before apps because there are 
  # CORBA-applications there.
  ROOTSUBS='include base tools-offline protocols corba tools-online apps' 
  if test x"${_compile_daemons}" = xyes; then
    APPSUBS="${APPSUBS} propertyd namingd"
  fi
  APPSUBS="${APPSUBS} browser chatservice"
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

dnl Set the Mico make flag according to previous findings
AM_CONDITIONAL(_CF_WITH_MICO, test x"${_micofound}" = xyes)

_cxxflags="$CXXFLAGS"
_ldflags="$LDFLAGS"
AC_MSG_CHECKING([for compiler type])
_compiler=
if test X"$ac_compiler_gnu" = Xno; then
	case $host in
	*hp-hpux*)
		# Guessing that the other compiler in HP-UX is aCC
		# Hope this works
		AC_MSG_RESULT([HP-UX aCC])
		_compiler=acc
		# -AA = namespace std, C++ Standard Library
		# -Aa = Koenig lookup, variable scope fix
		# -ext = C++ extensions like long long type
		_acc_common="-AA -Aa -ext"
		CXXFLAGS="$_acc_common"
                if test X"$enable_profile" = Xyes; then
			CXXFLAGS="-G $_acc_common"
  		fi
                if test X"$enable_debug" = Xyes; then
			CXXFLAGS="-g0 $_acc_common"
  		fi
		;;
	*solaris*)
		# We make a wild guess here that nobody uses any other
		# compiler than SunCC in Solaris in addition to g++.
		AC_MSG_RESULT([Sun CC])
		_compiler=suncc
		_suncc_common="-template=wholeclass,extdef -library=Crun,Cstd -instances=static -features=anachronisms,namespace,rtti"
		CXXFLAGS="$_suncc_common"
                if test X"$enable_profile" = Xyes; then
			CXXFLAGS="-xpg -g $_suncc_common"
  		fi
                if test X"$enable_debug" = Xyes; then
			CXXFLAGS="-g $_suncc_common"
  		fi
                if test X"$enable_64bit" = Xyes; then
			CXXFLAGS="${CXXFLAGS} -xarch=native64"
  		fi
		;;
	*)
		AC_MSG_RESULT([unknown])
		_compiler=unknown
		CXXFLAGS="-O"
                if test X"$enable_profile" = Xyes; then
			CXXFLAGS="-g -p"
  		fi
                if test X"$enable_debug" = Xyes; then
			CXXFLAGS="-g"
  		fi
		;;
        esac
else
	AC_MSG_RESULT([g++])
	_compiler=gxx
	CXXFLAGS="-g"
	if test X"$enable_profile" = Xyes; then
		CXXFLAGS="-g -p"
  	fi
	if test X"$enable_debug" = Xyes; then
		CXXFLAGS="-g"
  	fi
        if test X"$enable_64bit" = Xyes; then
		CXXFLAGS="${CXXFLAGS} -m64"
  	fi
fi
#CXXFLAGS="$CXXFLAGS $_cxxflags"
LDFLAGS="$LDFLAGS $_ldflags"

dnl Check for SDL, rt is needed by SDL in some OS's
SDLLIBS=
_rtlib=
AC_CHECK_LIB(rt, sem_init, _rtlib="-lrt")
_libs=${LIBS}
_sdlfound=yes
LIBS="${LIBS} ${_rtlib}"
AC_CHECK_LIB(SDL, SDL_Init, SDLLIBS="-lSDL ${_rtlib}", _sdlfound=no)
LIBS=${_libs}
AC_SUBST(SDLLIBS)
if test x"${_sdlfound}" = xyes; then
  AC_DEFINE([HAVE_SDL], 1, [Define to 1 if SDL library is found.])
  AM_CONDITIONAL(HAVE_SDL, true)
else
  AC_DEFINE([HAVE_SDL], 0, [Define to 0 if SDL library not found.])
  AM_CONDITIONAL(HAVE_SDL, false)
fi

dnl Guess audio system
_audiofound=no

dnl Check for libossaudio which is used by some BSD systems
_libs=${LIBS}
OSSAUDIO=
AC_CHECK_LIB(ossaudio,_oss_ioctl,[OSSAUDIO="-lossaudio"])
AC_SUBST(OSSAUDIO)
LIBS=${_libs}

dnl e.g. FreeBSD-OSS
if test x"${_audiofound}" = xno; then
AC_CHECK_HEADER(machine/soundcard.h,[
  AC_MSG_CHECKING([whether OSS-audio source compiles])
  AC_TRY_COMPILE([
#include <stdio.h>
#include <unistd.h>
#include <machine/soundcard.h>
],[
ioctl(0, SNDCTL_DSP_GETBLKSIZE, NULL);],[
AC_DEFINE([HAVE_MACHINE_SOUNDCARD_H],1,[Define to 1 if FreeBSD-compatible sound is found.])
AC_MSG_RESULT([yes])
_audiofound=yes],
AC_DEFINE([HAVE_MACHINE_SOUNDCARD_H],0,[Define to 0 if FreeBSD-compatible sound not found.])
AC_MSG_RESULT([no]))
])
fi

dnl Linux-OSS
if test x"${_audiofound}" = xno; then
AC_CHECK_HEADER(linux/soundcard.h,[
  AC_MSG_CHECKING([whether OSS-audio source compiles])
  AC_TRY_COMPILE([
#include <stdio.h>
#include <unistd.h>
#include <linux/soundcard.h>
],[
ioctl(0, SNDCTL_DSP_GETBLKSIZE, NULL);],[
AC_DEFINE([HAVE_LINUX_SOUNDCARD_H],1,[Define to 1 if Linux-compatible sound found.])
AC_MSG_RESULT([yes])
_audiofound=yes],
AC_DEFINE([HAVE_LINUX_SOUNDCARD_H],0,[Define to 0 if Linux-compatible sound not found.])
AC_MSG_RESULT([no]))
])
fi

dnl e.g. OpenBSD-OSS
if test x"${_audiofound}" = xno; then
AC_CHECK_HEADER(soundcard.h,[
  AC_MSG_CHECKING([whether OSS-audio source compiles])
  AC_TRY_COMPILE([
#include <stdio.h>
#include <unistd.h>
#include <soundcard.h>
],[
ioctl(0, SNDCTL_DSP_GETBLKSIZE, NULL);],[
AC_DEFINE([HAVE_SOUNDCARD_H],1,[Define to 1 if OpenBSD-compatible sound found.])
AC_MSG_RESULT([yes])
_audiofound=yes],A
AC_DEFINE([HAVE_SOUNDCARD_H],0,[Define to 0 if OpenBSD-compatible sound not found.])
AC_MSG_RESULT([no]))
])
fi

dnl Sun-audio
if test x"${_audiofound}" = xno; then
AC_CHECK_HEADER(sys/audioio.h,[
  AC_MSG_CHECKING([whether Sun-audio source compiles])
  AC_TRY_COMPILE([
#include <stdio.h>
#include <sys/audioio.h>
#include <unistd.h>
],[
ioctl(0, AUDIO_DRAIN, NULL);],[
AC_DEFINE([HAVE_SYS_AUDIOIO_H],1,[Define to 1 if Sun-compatible sound found.])
AC_MSG_RESULT([yes])
_audiofound=yes],
AC_DEFINE([HAVE_SYS_AUDIOIO_H],0,[Define to 0 if Sun-compatible sound not found.])
AC_MSG_RESULT([no]))
])
fi

if test x"${_audiofound}" = xno; then
  AC_MSG_WARN([no known audio system found; audio is disabled in demos])
fi

# If lame library and working audio exists, compile rtp app
if test x"${_audiofound}" = xyes && test x"${_lamefound}" = xyes; then
  APPSUBS="${APPSUBS} rtp"
fi

dnl Check for Lame-library
dnl Because libraries '-lmp3lame -lm' are only needed in mp3_demo, create
dnl LAMELIBS-variable that includes all the needed libraries
LAMELIBS=
LAMEINCS=
_lamefound=no
AC_ARG_WITH(lame,
[  --with-lame=DIR         compile with Lame mp3 library [default=no]],
[
 lame_library="$withval"
 case "$lame_library" in
 no)
  AC_DEFINE([_CF_WITH_LAME],0,[Define to 0 if Lame library not found.])
  ;;
 yes)
  _libs=${LIBS}
  LIBS="${LIBS} -lm"
  AC_CHECK_LIB(mp3lame,lame_decode_init)
  if test x"${ac_cv_lib_mp3lame_lame_encode}" = xno; then
     AC_MSG_ERROR(libmp3lame not found)
  fi
  _lamefound=yes
  LAMELIBS="-lmp3lame -lm"
  AC_DEFINE([_CF_WITH_LAME],1,[Define to 1 if Lame library found.])
  LIBS=${_libs}
  ;;
 *)
  _libs=${LIBS}
  _cflags=${CFLAGS}
  LIBS="${LIBS} -L${lame_library}/lib -lmp3lame -lm"
  CFLAGS="${CFLAGS} -I${lame_library}/include"
  AC_CHECK_LIB(mp3lame,lame_decode_init)
  if test x"${ac_cv_lib_mp3lame_lame_encode}" = xno; then
     AC_MSG_ERROR(libmp3lame not found)
  fi
  _lamefound=yes
  LAMELIBS="-L${lame_library}/lib -lmp3lame -lm"
  LAMEINCS="-I${lame_library}/include"
  AC_DEFINE([_CF_WITH_LAME],1,[Define to 1 if Lame library is found.])
  LIBS=${_libs}
  CFLAGS=${_cflags}
  ;;
 esac
],[AC_DEFINE([_CF_WITH_LAME],0,[Define to 0 if Lame library not found.])])
AM_CONDITIONAL(_CF_WITH_LAME, test x"${with_lame-no}" != xno)
AC_SUBST(LAMELIBS)
AC_SUBST(LAMEINCS)

AC_ARG_ENABLE(demon,[  --enable-demon          compile DEMON [default=no]],
[
 case "$enableval" in
  no)
  ;;
  yes)
   ONTOOLSSUBS="${ONTOOLSSUBS} demon"
  ;;
 esac
]
)

dnl Check for --enable-debug
AC_ARG_ENABLE(debug,[  --enable-debug          turn on debugging compiler flags [default=no]])

dnl Check for --enable-64bit
AC_ARG_ENABLE(64bit,[  --enable-64bit          turn on 64-bit compiler flags if applicable [default=no]])

dnl Check for --enable-profile
AC_ARG_ENABLE(profile,[  --enable-profile        turn on profiling compiler flags [default=no]])

dnl Check for --enable-debugmsg
AC_ARG_ENABLE(debugmsg,[  --enable-debugmsg       turn on printing of debugging messages at runtime [default=no]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_DEBUG],0,[Define to 1 if you need DOORS debug macros, else 0.])
  ;;
  yes)
   AC_DEFINE([_CF_DEBUG],1,[Define to 1 if you need DOORS debug macros, else 0.])
  ;;
 esac
],[AC_DEFINE([_CF_DEBUG],0,[Define to 1 if you need DOORS debug macros, else 0.])])

dnl Check for --enable-optimising
AC_ARG_ENABLE(optimising,[  --enable-optimising     compile optimised code [default=no]])

dnl Check for --enable-trace
AC_ARG_ENABLE(trace,[  --enable-trace          turn on user interface tracing [default=yes]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_TRACE],0,[Define to 1 if you need DOORS tracing macros, else 0.])
  ;;
  yes)
   AC_DEFINE([_CF_TRACE],1,[Define to 1 if you need DOORS tracing macros, else 0.])
  ;;
 esac
],[AC_DEFINE([_CF_TRACE],1,[Define to 1 if you need tracing macros, else 0.])])

dnl Check for --enable-fault
AC_ARG_ENABLE(fault,[  --enable-fault          turn on library internal fault checking [default=yes]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_FAULT],0,[Define to 1 if you need DOORS fault checking macros, else 0.])
  ;;
  yes)
   AC_DEFINE([_CF_FAULT],1,[Define to 1 if you need DOORS fault checking macros, else 0.])
  ;;
 esac
],[AC_DEFINE([_CF_FAULT],1,[Define to 1 if you need DOORS fault checking macros, else 0.])])

dnl Check for --enable-final
AC_ARG_ENABLE(final,[  --enable-final          build size optimised libraries [default=no]],
[
 case "$enableval" in
  no)
   AC_DEFINE([WITH_FINAL],0,[Define to 1 if optimised libraries are to be created, else 0.])
  ;;
  yes)
   AC_DEFINE([WITH_FINAL],1,[Define to 1 if optimised libraries are to be created, else 0..])
  ;;
 esac
],[AC_DEFINE([WITH_FINAL],0,[Define to 1 if optimised libraries are to be created, else 0.])])

dnl Check for --enable-error
AC_ARG_ENABLE(error,[  --enable-error          turn on library user use error checking [default=yes]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_ERROR],0,[Define to 1 if you need DOORS error checking macros, else 0.])
  ;;
  yes)
   AC_DEFINE([_CF_ERROR],1,[Define to 1 if you need DOORS error checking macros, else 0.])
  ;;
 esac
],[AC_DEFINE([_CF_ERROR],1,[Define to 1 if you need DOORS error checking macros, else 0.])])

dnl Check for --enable-mem
MEMLIBS=
AC_ARG_ENABLE(mem,[  --enable-mem            turn on memory manager in library building [default=yes]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_MEM],0,[Define to 1 if memory manager library is needed, else 0.])
  ;;
  yes)
   MEMLIBS='$(top_builddir)/base/mem/libmem.la'
   BASESUBS="${BASESUBS} mem"
   AC_DEFINE([_CF_MEM],1,[Define to 1 if memory manager library is needed, else 0.])
 esac
],[
MEMLIBS='$(top_builddir)/base/mem/libmem.la'
BASESUBS="${BASESUBS} mem"
AC_DEFINE([_CF_MEM],1,[Define to 1 if memory manager library is needed, else 0.])
])
AC_SUBST(MEMLIBS)

dnl Check for --enable-explicit
AC_ARG_ENABLE(explicit,[  --enable-explicit       turn on use of explicit templates [default=no]],
[
 case "$enableval" in
  no)
   AC_DEFINE([_CF_EXPLICIT_TEMPLATE],0,[Define to 0 if explicit templates are not to be used.])
  ;;
  yes)
   AC_DEFINE([_CF_EXPLICIT_TEMPLATE],1,[Define to 1 if explicit templates are to be used.])
 esac
],[AC_DEFINE([_CF_EXPLICIT_TEMPLATE],0,[Define to 0 if explicit templates are not to be used.])])

dnl Enable use of flags in Makefile.am files
AM_CONDITIONAL(_CF_PROFILE, test x"$enable_profile" = xyes)
AM_CONDITIONAL(_CF_DEBUG, test x"$enable_debugmsg" = xyes)
AM_CONDITIONAL(_CF_TRACE, test x"$enable_trace" = xyes)
AM_CONDITIONAL(_CF_FAULT, test x"$enable_fault" = xyes)
AM_CONDITIONAL(_CF_ERROR, test x"$enable_error" = xyes)
AM_CONDITIONAL(_CF_MEM, test x"$enable_mem" = xyes)
AM_CONDITIONAL(_CF_EXPLICIT_TEMPLATE, test x"$enable_explicit" = xyes)
AM_CONDITIONAL(WITH_FINAL, test x"$enable_final" = xyes)
AM_CONDITIONAL(WITH_AVSTREAMS, test x"$_slpfound" = xyes && test x"$_sdlfound" = xyes)

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h ctype.h stdlib.h string.h sys/file.h sys/ioctl.h sys/socket.h time.h sys/time.h unistd.h sysent.h sys/types.h ansidecl.h dlfcn.h errno.h float.h fp_class.h ieeefp.h malloc.h math.h nan.h ndir.h signal.h stdarg.h sys/dir.h sys/mman.h sys/select.h sys/stat.h sys/timeb.h zlib.h])

dnl Check for IPv6
_ipv6_compiles=no
AC_MSG_CHECKING([whether IPv6 source compiles])
AC_TRY_COMPILE([
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
],[
struct sockaddr_storage ss;
struct sockaddr_in6* iaddr6 = (struct sockaddr_in6*)&ss;
(void)IN6_IS_ADDR_MULTICAST(&iaddr6->sin6_addr);
(void)IN6_ARE_ADDR_EQUAL(&iaddr6->sin6_addr,&iaddr6->sin6_addr);
(void)getaddrinfo(0, 0, 0, 0);
],[
AC_MSG_RESULT([yes])
_ipv6_compiles=yes
],[
AC_MSG_RESULT([no])
])
_ipv6=no
AC_ARG_ENABLE(ipv6,[  --enable-ipv6       enable IPv6 [default=yes]],
[
 case "$enableval" in
  no)
  ;;
  yes)
   _ipv6=yes
  ;;
 esac
],[_ipv6=yes])
UDP6LIB=
SLP6LIBS=
if test x"${_ipv6}" = xyes && test x"${_ipv6_compiles}" = xyes; then
  APPSUBS="${APPSUBS} udp6 slp6"
  PROTOSUBS="${PROTOSUBS} udp6 slp6"
  UDP6LIB='$(top_builddir)/protocols/udp6/libudp6.la'
  SLP6LIBS='$(top_builddir)/protocols/slp6/impl/da/libslp6_da.la $(top_builddir)/protocols/slp6/interface/libslp6.la $(top_builddir)/protocols/slp6/impl/common/libslp6_r.la $(top_builddir)/protocols/slp6/impl/sa/libslp6_sa.la $(top_builddir)/protocols/slp6/impl/ua/libslp6_ua.la $(top_builddir)/protocols/slp6/impl/aa/libslp6_aa.la'
  AC_DEFINE([HAVE_IPV6], 1, [Define if IPv6 support is provided.])
  AM_CONDITIONAL(HAVE_IPV6, true)
  _ipv6=yes
else
  AC_DEFINE(HAVE_IPV6, 0, [Define if IPv6 support is not provided.])
  AM_CONDITIONAL(HAVE_IPV6, false)
  _ipv6=no
  AC_MSG_WARN([IPv6 disabled])
fi
AC_SUBST(UDP6LIB)
AC_SUBST(SLP6LIBS)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPE(socklen_t,AC_DEFINE([HAVE_SOCKLEN_T],1,[Define if socklen_t is available.]),AC_DEFINE([HAVE_SOCKLEN_T],0,[Define if socklen_t is not available.]),[
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
#if HAVE_SOCKET_H
  #include <socket.h>
#endif
])
AC_HEADER_TIME
AC_STRUCT_TM

# Checking for the third getpeername parameter
_peername_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third getpeername argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_SOCKLEN_T_PEERNAME],1,[Define if getpeername() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_peername_type_found=yes])
fi
if test x"$_peername_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_SIZE_T_PEERNAME],1,[Define if getpeername() uses size_t.])
AC_MSG_RESULT([size_t])
_peername_type_found=yes])
fi
if test x"$_peername_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)getpeername(0,0,&i);],[
AC_DEFINE([HAVE_INT_PEERNAME],1,[Define if getpeername() uses int.])
AC_MSG_RESULT([int])
_peername_type_found=yes])
fi

if test x"$_peername_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getpeername type found])
fi

# Checking for the third getsockname parameter
_sockname_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third getsockname argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_SOCKLEN_T_SOCKNAME],1,[Define if getsockname() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_sockname_type_found=yes])
fi
if test x"$_sockname_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_SIZE_T_SOCKNAME],1,[Define if getsockname() uses size_t.])
AC_MSG_RESULT([size_t])
_sockname_type_found=yes])
fi
if test x"$_sockname_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)getsockname(0,0,&i);],[
AC_DEFINE([HAVE_INT_SOCKNAME],1,[Define if getsockname() uses int.])
AC_MSG_RESULT([int])
_sockname_type_found=yes])
fi

if test x"$_sockname_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getsockname type found])
fi

_getsockopt_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the fourth getsockopt argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_SOCKLEN_T_GETSOCKOPT],1,[Define if getsockopt() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_getsockopt_type_found=yes])
fi
if test x"$_getsockopt_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_SIZE_T_GETSOCKOPT],1,[Define if getsockopt() uses size_t.])
AC_MSG_RESULT([size_t])
_getsockopt_type_found=yes])
fi
if test x"$_getsockopt_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)getsockopt(0,0,0,0,&i);],[
AC_DEFINE([HAVE_INT_GETSOCKOPT],1,[Define if getsockopt() uses int.])
AC_MSG_RESULT([int])
_getsockopt_type_found=yes])
fi

if test x"$_getsockopt_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known getsockopt type found])
fi

# Checking for the third accept parameter
_accept_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third accept argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_SOCKLEN_T_ACCEPT],1,[Define if accept() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_accept_type_found=yes])
fi
if test x"$_accept_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_SIZE_T_ACCEPT],1,[Define if accept() uses size_t.])
AC_MSG_RESULT([size_t])
_accept_type_found=yes])
fi
if test x"$_accept_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)accept(0,0,&i);],[
AC_DEFINE([HAVE_INT_ACCEPT],1,[Define if accept() uses int.])
AC_MSG_RESULT([int])
_accept_type_found=yes])
fi

if test x"$_accept_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known accept type found])
fi

# Checking for the third bind parameter
_bind_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the third bind argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)bind(0,0,i);],[
AC_DEFINE([HAVE_SOCKLEN_T_BIND],1,[Define if bind() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_bind_type_found=yes])
fi
if test x"$_bind_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)bind(0,0,i);],[
AC_DEFINE([HAVE_SIZE_T_BIND],1,[Define if bind() uses size_t.])
AC_MSG_RESULT([size_t])
_bind_type_found=yes])
fi
if test x"$_bind_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)bind(0,0,i);],[
AC_DEFINE([HAVE_INT_BIND],1,[Define if bind() uses int.])
AC_MSG_RESULT([int])
_bind_type_found=yes])
fi

if test x"$_bind_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known bind type found])
fi

# Checking for the sixth recvfrom parameter
_recvfrom_type_found=no
if test x"$ac_cv_type_socklen_t" = xyes; then
AC_MSG_CHECKING([for the sixth recvfrom argument])
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
socklen_t i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_SOCKLEN_T_RECVFROM],1,[Define if recvfrom() uses socklen_t.])
AC_MSG_RESULT([socklen_t])
_recvfrom_type_found=yes])
fi
if test x"$_recvfrom_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
size_t i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_SIZE_T_RECVFROM],1,[Define if recvfrom() uses size_t.])
AC_MSG_RESULT([size_t])
_recvfrom_type_found=yes])
fi
if test x"$_recvfrom_type_found" != xyes; then
AC_TRY_COMPILE([
#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if HAVE_SYS_SOCKET_H
  #include <sys/socket.h>
#endif
],[
int i;
(void)recvfrom(0,0,0,0,0,&i);],[
AC_DEFINE([HAVE_INT_RECVFROM],1,[Define if recvfrom() uses int.])
AC_MSG_RESULT([int])
_recvfrom_type_found=yes])
fi

if test x"$_recvfrom_type_found" != xyes; then
  AC_MSG_RESULT([cannot resolve])
  AC_MSG_ERROR([No known recvfrom type found])
fi

# Sizeofs
AC_CHECK_SIZEOF(unsigned char)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(unsigned int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(unsigned long long)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(unsigned short)
AC_CHECK_SIZEOF(float)
AC_CHECK_SIZEOF(double)
AC_CHECK_SIZEOF(long double)

# Byteorder
AC_C_BIGENDIAN

# Checks for library functions.
AC_PROG_MAKE_SET
AC_FUNC_ERROR_AT_LINE
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([getcwd gethostbyaddr gethostbyname gethostname gettimeofday inet_ntoa memset select socket sqrt strchr strerror strstr])

AH_BOTTOM([
#ifndef DOORSCONF_H_INCLUDED
#define DOORSCONF_H_INCLUDED

#if HAVE_SYS_TYPES_H
  #include <sys/types.h>
#endif
#if TIME_WITH_SYS_TIME
  #include <sys/time.h>
  #include <time.h>
#else
  #if HAVE_SYS_TIME_H
  #include <sys/time.h>
#else
  #include <time.h>
  #endif
#endif
#if HAVE_UNISTD_H
  #include <unistd.h>
#endif
#if HAVE_SELECT_H
  #include <select.h>
#endif
// Uncomment if needed:
//#if HAVE_SYSENT_H
//  #include <sysent.h>
//#endif
#if HAVE_SOCKLEN_T
#else
typedef int socklen_t;
#endif

#define SELECT(wi,r,w,e,t) select(wi,(fd_set*)r,(fd_set*)w,(fd_set*)e,t)

#endif // DOORSCONF_H_INCLUDED
])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 include/doors/Makefile
                 base/Makefile
                 base/mem/Makefile
                 base/hsi/Makefile
                 base/sys/Makefile
                 base/ptb/Makefile
                 base/dev/Makefile
                 base/util/Makefile
		 tools-online/Makefile
		 tools-online/lemon/Makefile
                 tools-online/demon/Makefile
                 tools-online/demon/agent/Makefile
                 tools-online/demon/manager/Makefile
                 tools-online/demon/manager/qt/Makefile
		 tools-offline/Makefile
		 tools-offline/xcg/Makefile
		 tools-offline/xcg/common/Makefile
		 tools-offline/xcg/common/parser/Makefile
		 tools-offline/xcg/dsapg/Makefile
		 tools-offline/xcg/dpeerg/Makefile
		 tools-offline/xcg/dsmg/Makefile
		 protocols/Makefile
		 protocols/rtcp/Makefile
		 protocols/rtcp/interface/Makefile
		 protocols/rtcp/impl/Makefile
		 protocols/rtp/Makefile
		 protocols/rtp/interface/Makefile
		 protocols/rtp/impl/Makefile
		 protocols/udp/Makefile
		 protocols/udp6/Makefile
		 protocols/phys_virtual/Makefile
		 protocols/phys_virtual/interface/Makefile
		 protocols/phys_virtual/impl/Makefile
		 protocols/link/Makefile
		 protocols/link/interface/Makefile
		 protocols/link/impl/Makefile
		 protocols/sap/Makefile
		 protocols/sap/interface/Makefile
		 protocols/sap/impl/Makefile
		 protocols/altbit/Makefile
		 protocols/altbit/interface/Makefile
		 protocols/altbit/impl/Makefile
		 protocols/tcpmux/Makefile
		 protocols/tcpmux/interface/Makefile
		 protocols/tcpmux/impl/Makefile
		 protocols/tcp6mux/Makefile
		 protocols/tcp6mux/interface/Makefile
		 protocols/tcp6mux/impl/Makefile
		 protocols/slp6/Makefile
                 protocols/slp6/impl/Makefile
		 protocols/slp6/impl/common/Makefile
		 protocols/slp6/impl/da/Makefile
		 protocols/slp6/impl/sa/Makefile
		 protocols/slp6/impl/ua/Makefile
		 protocols/slp6/impl/aa/Makefile
		 protocols/slp6/interface/Makefile
                 corba/Makefile
                 corba/broker/Makefile
                 corba/services/Makefile
                 corba/services/propertyservice/Makefile
                 corba/services/namingservice/Makefile
                 apps/Makefile
                 apps/browser/Makefile
                 apps/chatservice/Makefile
                 apps/rtp/Makefile
                 apps/sap/Makefile
                 apps/udp/Makefile
                 apps/udp6/Makefile
                 apps/slp6/Makefile
		 apps/link/Makefile
		 apps/tcpmux/Makefile
		 apps/tcp6mux/Makefile
                 apps/altbit/Makefile
		 apps/propertyd/Makefile
		 apps/namingd/Makefile])

AC_OUTPUT

echo ""
echo "DOORS configuration:"
echo ""
echo "        Compiler: ${CXX}"
echo "          Linker: ${LD}"
echo "  Compiler flags: ${CXXFLAGS}"
echo "       With Mico: ${_micofound}."
echo "         With Qt: ${_qtfound}."
echo "        With SLP: ${_slpfound}."
echo "       With LDAP: ${_ldapfound}."
echo "   Subroutines to compile: ${ROOTSUBS}"
echo "  Applications to compile: ${APPSUBS}"
echo "     Protocols to compile: ${PROTOSUBS}"
echo ""
