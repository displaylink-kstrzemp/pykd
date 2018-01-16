#pragma once

#define PYKDEXT_VERSION_MAJOR      2
#define PYKDEXT_VERSION_MINOR      0
#define PYKDEXT_VERSION_SUBVERSION 0
#define PYKDEXT_VERSION_BUILDNO    16

#define __VER_STR2__(x) #x
#define __VER_STR1__(x) __VER_STR2__(x)

#define PYKDEXT_VERSION_BUILD_COMMA    PYKDEXT_VERSION_MAJOR, PYKDEXT_VERSION_MINOR, PYKDEXT_VERSION_SUBVERSION, PYKDEXT_VERSION_BUILDNO
#define PYKDEXT_VERSION_BUILD          PYKDEXT_VERSION_MAJOR.PYKDEXT_VERSION_MINOR.PYKDEXT_VERSION_SUBVERSION.PYKDEXT_VERSION_BUILDNO

#define PYKDEXT_VERSION_BUILD_STR_COMMA    __VER_STR1__(PYKDEXT_VERSION_BUILD_COMMA)
#define PYKDEXT_VERSION_BUILD_STR          __VER_STR1__(PYKDEXT_VERSION_BUILD)
