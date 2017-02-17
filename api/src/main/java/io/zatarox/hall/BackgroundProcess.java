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
package io.zatarox.hall;

/**
 * Provides support for native background process invocation. Using a platform dependant
 * helper program, classes that implement the <code>BackgroundProcess</code> interface can
 * be initialized, started and stopped according to the conventions of the
 * underlying operating system.
 * <p>
 * Implementors of this interface must also provide a public constructor with no
 * arguments so that instances can be created in an automated fashion.
 * </p>
 */
public interface BackgroundProcess {

    /**
     * Initializes this <code>BackgroundProcess</code> instance.
     * <p>
     * This method gets called once the JVM process is created and the
     * <code>BackgroundProcess</code> instance is created thru its empty public
     * constructor.
     * </p>
     * <p>
     * Under certain operating systems (typically Unix based operating systems)
     * and if the native invocation framework is configured to do so, this
     * method might be called with <i>super-user</i> privileges.
     * </p>
     * <p>
     * For example, it might be wise to create <code>ServerSocket</code>
     * instances within the scope of this method, and perform all operations
     * requiring <i>super-user</i> privileges in the underlying operating
     * system.
     * </p>
     * <p>
     * Apart from set up and allocation of native resources, this method must
     * not start the actual operation of the <code>BackgroundProcess</code> (such as
     * starting threads calling the <code>ServerSocket.accept()</code> method)
     * as this would impose some serious security hazards. The start of
     * operation must be performed in the <code>start()</code> method.
     * </p>
     *
     * @param context A <code>BackgroundContext</code> object used to communicate
     * with the container.
     * @throws BackgroundException An exception that prevented initialization
     * where you want to display a nice message to the user, rather than a stack
     * trace.
     * @throws Exception Any exception preventing a successful initialization.
     */
    public void init(BackgroundContext context)
            throws BackgroundException, Exception;

    /**
     * Starts the operation of this <code>BackgroundProcess</code> instance. This method is
     * to be invoked by the environment after the init() method has been
     * successfully invoked and possibly the security level of the JVM has been
     * dropped. Implementors of this method are free to start any number of
     * threads, but need to return control after having done that to enable
     * invocation of the stop()-method.
     */
    public void start()
            throws Exception;

    /**
     * Stops the operation of this <code>BackgroundProcess</code> instance. Note that the
 proper place to free any allocated resources such as sockets or file
 descriptors is in the destroy method, as the container may restart the
 BackgroundProcess by calling start() after stop().
     */
    public void stop()
            throws Exception;

    /**
     * Frees any resources allocated by this background process such as file descriptors or
     * sockets. This method gets called by the container after stop() has been
 called and before the JVM exits. The BackgroundProcess can not be restarted after this
 method has been called without a new call to the init() method.
     */
    public void destroy();
}
