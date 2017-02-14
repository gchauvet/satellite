/*
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __VERSION_H__
#define __VERSION_H__

/**
 * Major API changes that could cause compatibility problems for older
 * programs such as structure size changes.  No binary compatibility is
 * possible across a change in the major version.
 */
#define DEIMOS_MAJOR_VERSION      2

/**
 * Minor API changes that do not cause binary compatibility problems.
 * Should be reset to 0 when upgrading DEIMOS_MAJOR_VERSION
 */
#define DEIMOS_MINOR_VERSION      0

/** patch level */
#define DEIMOS_PATCH_VERSION      0

/**
 *  This symbol is defined for internal, "development" copies of DEIMOS.
 *  This symbol will be #undef'd for releases.
 */
#define DEIMOS_IS_DEV_VERSION     1

/** Properly quote a value as a string in the C preprocessor */
#define DEIMOS_STRINGIFY(n) DEIMOS_STRINGIFY_HELPER(n)
/** Helper macro for DEIMOS_STRINGIFY */
#define DEIMOS_STRINGIFY_HELPER(n) #n


/** The formatted string of APU's version */
#define DEIMOS_VERSION_STRING \
     DEIMOS_STRINGIFY(DEIMOS_MAJOR_VERSION) "."   \
     DEIMOS_STRINGIFY(DEIMOS_MINOR_VERSION) "."   \
     DEIMOS_STRINGIFY(DEIMOS_PATCH_VERSION)       \
     DEIMOS_IS_DEV_STRING

/** Internal: string form of the "is dev" flag */
#if DEIMOS_IS_DEV_VERSION
#define DEIMOS_IS_DEV_STRING "-dev"
#else
#define DEIMOS_IS_DEV_STRING ""
#endif

#endif /* __VERSION_H__ */

