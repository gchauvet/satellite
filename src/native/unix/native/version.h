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
#define JSVC_MAJOR_VERSION      2

/**
 * Minor API changes that do not cause binary compatibility problems.
 * Should be reset to 0 when upgrading JSVC_MAJOR_VERSION
 */
#define JSVC_MINOR_VERSION      0

/** patch level */
#define JSVC_PATCH_VERSION      0

/**
 *  This symbol is defined for internal, "development" copies of JSVC.
 *  This symbol will be #undef'd for releases.
 */
#define JSVC_IS_DEV_VERSION     1

/** Properly quote a value as a string in the C preprocessor */
#define JSVC_STRINGIFY(n) JSVC_STRINGIFY_HELPER(n)
/** Helper macro for JSVC_STRINGIFY */
#define JSVC_STRINGIFY_HELPER(n) #n


/** The formatted string of APU's version */
#define JSVC_VERSION_STRING \
     JSVC_STRINGIFY(JSVC_MAJOR_VERSION) "."   \
     JSVC_STRINGIFY(JSVC_MINOR_VERSION) "."   \
     JSVC_STRINGIFY(JSVC_PATCH_VERSION)       \
     JSVC_IS_DEV_STRING

/** Internal: string form of the "is dev" flag */
#if JSVC_IS_DEV_VERSION
#define JSVC_IS_DEV_STRING "-dev"
#else
#define JSVC_IS_DEV_STRING ""
#endif

#endif /* __VERSION_H__ */

