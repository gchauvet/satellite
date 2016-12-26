/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* @version $Id$ */
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

