# From main target folder :
# cmake -G "MinGW Makefiles"  ..\src\native\windows -DCMAKE_BUILD_TYPE=MINSIZEREL -DCMAKE_TOOLCHAIN_FILE=..\src\native\toolchains\Windows64.cmake
# mingw32-make install/strip

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(procrun C)
SET(CMAKE_C_STANDARD 99)

ADD_DEFINITIONS(
    -D_UNICODE
    -DUNICODE
    -DPRG_VERSION_MAJOR=2
    -DPRG_VERSION_MINOR=0
    -DPRG_VERSION_PATCH=0
)

INCLUDE_DIRECTORIES (./include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(apps/prunmgr)
ADD_SUBDIRECTORY(apps/prunsrv)
