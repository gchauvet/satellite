INCLUDE(CMakeForceCompiler)
SET(CMAKE_SYSTEM_NAME Linux)
ENABLE_LANGUAGE(RC)
CMAKE_FORCE_C_COMPILER(i686-w64-mingw32-gcc GNU)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
