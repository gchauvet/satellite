# The MIT License
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(javad C)
SET(CMAKE_C_STANDARD 99)
FIND_PACKAGE(JNI)

ADD_DEFINITIONS(
    -D_UNICODE
    -DUNICODE
    -DDSO_DLFCN
    -DPRG_VERSION_MAJOR=${PRG_VERSION_MAJOR}
    -DPRG_VERSION_MINOR=${PRG_VERSION_MINOR}
    -DPRG_VERSION_PATCH=${PRG_VERSION_PATCH}
)

IF(JNI_FOUND)
  INCLUDE_DIRECTORIES(SYSTEM ${JNI_INCLUDE_DIRS})
ELSE()
  INCLUDE_DIRECTORIES(SYSTEM
    $ENV{JAVA_HOME}/include
    $ENV{JAVA_HOME}/include/win32
    $ENV{JAVA_HOME}/include/linux
  )
ENDIF()

INCLUDE_DIRECTORIES (
    ./include
    ${CMAKE_BINARY_DIR}/generated-sources/c
)
FILE(GLOB SOURCES ./c/*.c)
LIST(APPEND SOURCES ${CMAKE_BINARY_DIR}/generated-sources/c/embedded.c)

ADD_EXECUTABLE (javad ${SOURCES})
TARGET_LINK_LIBRARIES(javad dl)
SET_TARGET_PROPERTIES(javad PROPERTIES OUTPUT_NAME "javad-${SATELLITE_ARCH}")
INSTALL(TARGETS javad RUNTIME DESTINATION bin)
