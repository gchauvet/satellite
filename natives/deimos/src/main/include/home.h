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

#ifndef __DEIMOS_HOME_H__
#define __DEIMOS_HOME_H__

typedef struct home_jvm home_jvm;
typedef struct home_data home_data;

struct home_jvm
{
    char *name;
    char *libr;
};

struct home_data
{
    char *path;
    char *cfgf;
    home_jvm **jvms;
    int jnum;
};

/**
 * Attempt to locate a Java Home directory and build its structure.
 *
 * @param path The java home path specified on the command line.
 * @return A home_data structure containing all informations related to
 *         the Java environment, or NULL if no home was found.
 */
home_data *home(char *path);

#endif /* ifndef __DEIMOS_HOME_H__ */
