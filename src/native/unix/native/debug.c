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

#include "jsvc.h"
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

/* Wether debug is enabled or not */
bool log_debug_flag = false;

/* Wether SYSLOG logging (for stderr) is enable or not. */
bool log_stderr_syslog_flag = false;

/* Wether SYSLOG logging (for stdout) is enable or not. */
bool log_stdout_syslog_flag = false;

/* The name of the jsvc binary. */
char *log_prog = "jsvc";

/* Dump a debug trace message to stderr */
void log_debug(const char *fmt, ...)
{
    va_list ap;
    time_t now;
    struct tm *nowtm;
    char buff[80];

    if (log_debug_flag == false)
        return;
    if (fmt == NULL)
        return;

    now   = time(NULL);
    nowtm = localtime(&now);
    strftime(buff, sizeof(buff), "%Y-%m-%d %T", nowtm);
    va_start(ap, fmt);
    if (log_stderr_syslog_flag)
        fprintf(stderr, "%s %d %s debug: ", buff, getpid(), log_prog);
#if defined(DEBUG) || defined(_DEBUG)
    else
        fprintf(stderr, "[debug] %s %d ", buff, getpid());
#endif
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);
    va_end(ap);
}

/* Dump an error message to stderr */
void log_error(const char *fmt, ...)
{
    va_list ap;
    time_t now;
    struct tm *nowtm;
    char buff[80];

    if (fmt == NULL)
        return;

    va_start(ap, fmt);
    if (log_stderr_syslog_flag) {
        now   = time(NULL);
        nowtm = localtime(&now);
        strftime(buff, sizeof(buff), "%Y-%m-%d %T", nowtm);
        fprintf(stderr, "%s %d %s error: ", buff, getpid(), log_prog);
    }
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);
    va_end(ap);
}

