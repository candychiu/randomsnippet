/*************************************************************************
USAGE:
~~~~~~

Before including this header you must define one or more of define the following macros:

AUTOLINK_LIB_NAME:		Required: A string containing the basename of the library, for example newmat.
AUTOLINK_LIB_VERSION:	Optional: A string containing the version the library, for example 1_0.
AUTOLINK_DIAGNOSTIC:	Optional: when set the header will print out the name

These macros will be undef'ed at the end of the header, further this header
has no include guards - so be sure to include it only once from your library!

Algorithm:
~~~~~~~~~~

The name of the lib is selected according to the following formula:

  LIB_NAME "-"
+ LIB_VERSION "-" (skip if not defined)
+ TOOLSET
+ PLATFORM
+ THREAD_OPT
+ RT_OPT

These are defined as:

LIB_NAME: The base name of the lib (for example newmat).
LIB_VERSION: The library version, in the form x_y, skipped if not defined
TOOLSET: The compiler toolset name (v100, v120, etc)
THREAD_OPT:	"-mt" for multithread builds, otherwise nothing.
RT_OPT: A suffix that indicates the runtime library used, contains one or more of the following letters after a hyphen:
	g      debug/diagnostic runtime (release if not present).
	d      debug build (release if not present).
	    Static runtime is not supported

REFERENCE:
~~~~~~~~~~
This utility is adopted from the boost library's <boost/config/auto_link.hpp>

***************************************************************************/

// version
#ifdef AUTOLINK_LIB_VERSION
#  define AUTOLINK_VERSION "-" AUTOLINK_LIB_VERSION
#else
#  define AUTOLINK_VERSION 
#endif

// select toolset:
#ifndef _MSC_VER
#  error "Microsoft compiler version not found"
#else
#  if (_MSC_VER < 1600)
#    error "unsupported Microsoft compiler"
#  elif (_MSC_VER == 1600)
#    define AUTOLINK_TOOLSET "-v100"
#  elif (_MSC_VER == 1700)
#    define AUTOLINK_TOOLSET "-v110"
#  elif (_MSC_VER == 1800)
#    define AUTOLINK_TOOLSET "-v120"
#  else
#    define AUTOLINK_TOOLSET "-v130"
#  endif
#endif

#ifdef _M_X64
#  define AUTOLINK_PLATFORM "-x64"
#else
#  define AUTOLINK_PLATFORM "-x86"
#endif

/*** libraries to be linked ***/

// select thread opt:
#ifdef _MT
#else
#  error "does not single threaded runtime"
#endif

// select linkage opt:
#ifdef _DLL
#  if defined(_DEBUG)
#    define AUTOLINK_RT_OPT "-gd"
#  else
#    define AUTOLINK_RT_OPT
#  endif
#else
#  error "does not support static runtime"
#endif

#define AUTOLINK_NAME AUTOLINK_LIB_NAME AUTOLINK_VERSION AUTOLINK_TOOLSET AUTOLINK_PLATFORM AUTOLINK_RT_OPT ".lib"

#pragma comment(lib, AUTOLINK_NAME)
#ifdef AUTOLINK_DIAGNOSTIC
#  pragma message("Will (need to) link to lib file: " AUTOLINK_NAME)
#endif

// clean up
#undef AUTOLINK_NAME
#undef AUTOLINK_VERSION
#undef AUTOLINK_TOOLSET
#undef AUTOLINK_PLATFORM
#undef AUTOLINK_RT_OPT

// clean up inputs
#undef AUTOLINK_LIB_NAME
#undef AUTOLINK_LIB_VERSION
#undef AUTOLINK_DIAGNOSTIC
