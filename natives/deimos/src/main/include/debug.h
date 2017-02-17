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

#ifndef __DEIMOS_DEBUG_H__
#define __DEIMOS_DEBUG_H__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Wether debugging is enabled or not.
 */
extern bool log_debug_flag;

/* Wether SYSLOG logging (for stderr) is enable or not. */
extern bool log_stderr_syslog_flag;

/* Wether SYSLOG logging (for stdout) is enable or not. */
extern bool log_stdout_syslog_flag;

/**
 * The name of the deimos binary.
 */
extern char *log_prog;

/**
 * Helper macro to avoid NPEs in printf.
 */
#define PRINT_NULL(x) ((x) == NULL ? "null" : (x))

/**
 * Dump a debug message.
 *
 * @param fmt The printf style message format.
 * @param ... Any optional parameter for the message.
 */
void log_debug(const char *fmt, ...);

/**
 * Dump an error message.
 *
 * @param fmt The printf style message format.
 * @param ... Any optional parameter for the message.
 */
void log_error(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif                          /* ifndef __DEIMOS_DEBUG_H__ */
