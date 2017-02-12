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

#ifndef __JSVC_ARGUMENTS_H__
#define __JSVC_ARGUMENTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * The structure holding all parsed command line options.
 */
typedef struct {
    /** The name of the PID file. */
    char *pidf;
    /** The name of the user. */
    char *user;
    /** The name of the JVM to use. */
    char *name;
    /** The JDK or JRE installation path (JAVA_HOME). */
    char *home;
    /** Working directory (defaults to /). */
    char *cwd;
    /** Options used to invoke the JVM. */
    char **opts;
    /** Number of JVM options. */
    int onum;
    /** The name of the class to invoke. */
    char *jar;
    /** Command line arguments to the class. */
    char **args;
    /** Number of class command line arguments. */
    int anum;
    /** Wether to detach from parent process or not. */
    bool dtch;
    /** Wether to print the VM version number or not. */
    bool vers;
    /** Show the VM version and continue. */
    bool vershow;
    /** Wether to display the help page or not. */
    bool help;
    /** Only check environment without running the service. */
    bool chck;
    /** Stop running jsvc */
    bool stop;
    /** number of seconds to until service started */
    int wait;
    /** Install as a service (win32) */
    bool install;
    /** Remove when installed as a service (win32) */
    bool remove;
    /** Run as a service (win32) */
    bool service;
    /** Destination for stdout */
    char *outfile;
    /** Destination for stderr */
    char *errfile;
    /** Program name **/
    char *procname;
    /** Whether to redirect stdin to /dev/null or not. Defaults to true **/
    bool redirectstdin;
    /** What umask to use **/
    int umask;
} arg_data;

/**
 * Parse command line arguments.
 *
 * @param argc The number of command line arguments.
 * @param argv Pointers to the different arguments.
 * @return A pointer to a arg_data structure containing the parsed command
 *         line arguments, or NULL if an error was detected.
 */
arg_data *arguments(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
#endif                          /* ifndef __JSVC_ARGUMENTS_H__ */

