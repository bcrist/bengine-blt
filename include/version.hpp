#pragma once
#ifndef BE_BLT_VERSION_HPP_
#define BE_BLT_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_BLT_VERSION_MAJOR 0
#define BE_BLT_VERSION_MINOR 1
#define BE_BLT_VERSION_REV 6

/*!! include('common/version', 'be::blt') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_BLT_VERSION (BE_BLT_VERSION_MAJOR * 100000 + BE_BLT_VERSION_MINOR * 1000 + BE_BLT_VERSION_REV)
#define BE_BLT_VERSION_STRING "be::blt " BE_STRINGIFY(BE_BLT_VERSION_MAJOR) "." BE_STRINGIFY(BE_BLT_VERSION_MINOR) "." BE_STRINGIFY(BE_BLT_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
