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

#ifdef DSO_DLFCN
#include <dlfcn.h>
#ifdef OS_LINUX
bool ld_library_path_set = false;
#endif /* ifdef OS_LINUX */

/* Initialize all DSO stuff */
bool dso_init(void)
{
    return true;
}

/* Attempt to link a library from a specified filename */
dso_handle dso_link(const char *path)
{
    log_debug("Attemtping to load library %s", path);

    return ((void *)dlopen(path, RTLD_GLOBAL | RTLD_NOW));
}

/* Attempt to unload a library */
bool dso_unlink(dso_handle libr)
{
    if (dlclose(libr) == 0)
        return true;
    else
        return false;
}

/* Get the address for a specifed symbol */
void *dso_symbol(dso_handle hdl, const char *nam)
{
    return dlsym(hdl, nam);
}

/* Return the error message from dlopen */
char *dso_error(void)
{
    return (dlerror());
}

#endif /* ifdef DSO_DLFCN */

