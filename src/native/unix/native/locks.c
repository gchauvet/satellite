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

/*
 * as Cygwin does not support lockf, jsvc uses fcntl to emulate it.
 */
#ifdef OS_CYGWIN
#include "jsvc.h"
#include <sys/fcntl.h>

/*
 * File locking routine
 */
int lockf(int fildes, int function, off_t size)
{
    struct flock buf;

    switch (function) {
        case F_LOCK:
            buf.l_type = F_WRLCK;
        break;
        case F_ULOCK:
            buf.l_type = F_UNLCK;
        break;
        default:
        return -1;
    }
    buf.l_whence = 0;
    buf.l_start = 0;
    buf.l_len = size;

    return fcntl(fildes, F_SETLK, &buf);
}
#else
const char __unused_locks_c[] = __FILE__;
#endif

