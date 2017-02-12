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

#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static char *_find_file_in_java_home(char *java_home, const char *filename) {
    DIR *dp;
    struct dirent *entry;
    struct stat stat;
    char *result = NULL;
    
    if((dp = opendir(java_home)) != NULL) {
        chdir(java_home);
        while((entry = readdir(dp)) != NULL) {
            lstat(entry->d_name, &stat);
            if(S_ISDIR(stat.st_mode)) {
                if(strcmp(".", entry->d_name) == 0 ||
                   strcmp("..", entry->d_name) == 0)
                    continue;
                result = _find_file_in_java_home(entry->d_name, filename);
                if(result != NULL)
                    break;
            } else {
                if(strcmp(filename, entry->d_name) == 0) {
                    char buffer[PATH_MAX + 1];
                    result = realpath(entry->d_name, buffer);
                    if(result != NULL)
                        result = strdup(result);
                    break;
                }
            }
        }
        chdir("..");
        closedir(dp);
    }
    return result;
}

static char *find_file_in_java_home(char *java_home, const char *filename) {
    char buffer[PATH_MAX + 1];
    char *result = NULL;
    if(getcwd(buffer, sizeof(buffer)) != NULL) {
        result = _find_file_in_java_home(java_home, filename);
        chdir(buffer);
    }
    return result;
}

/* The jvm.cfg file defines the VMs available for invocation. So far, on all
 * all systems I've seen it's in $JAVA_HOME subdirectory. We iterate over
 * subfolders to reach this file.
 */
char *find_location_jvm_cfg(char *java_home) {
    return find_file_in_java_home(java_home, "jvm.cfg");
}

char *find_location_jvm_default(char *java_home) {
    static const char *filename =
#if defined(__APPLE__)
    "libjvm.dylib";
#elif defined(_WIN32)
    "jvm.dll";
#elif defined(__unix__)
    "libjvm.so";
#else
    "libgcj.so";
#endif
    return find_file_in_java_home(java_home, filename);
}

