INCLUDE(CMakeForceCompiler)
SET(CMAKE_SYSTEM_NAME Windows)
CMAKE_FORCE_C_COMPILER(x86_64-w64-mingw32-gcc GNU)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
