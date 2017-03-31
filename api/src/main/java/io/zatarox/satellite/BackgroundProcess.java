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
package io.zatarox.satellite;

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
 Apart from set up and allocation of native resources, this method must
 not resume the actual operation of the <code>BackgroundProcess</code> (such as
     * starting threads calling the <code>ServerSocket.accept()</code> method)
 as this would impose some serious security hazards. The resume of
 operation must be performed in the <code>resume()</code> method.
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
 to be invoked by the environment after the init() method has been
 successfully invoked and possibly the security level of the JVM has been
 dropped. Implementors of this method are free to resume any number of
 threads, but need to return control after having done that to enable
 invocation of the pause()-method.
     */
    public void resume()
            throws Exception;

    /**
     * Stops the operation of this <code>BackgroundProcess</code> instance. Note that the
 proper place to free any allocated resources such as sockets or file
 descriptors is in the destroy method, as the container may restart the
 BackgroundProcess by calling resume() after pause().
     */
    public void pause()
            throws Exception;

    /**
     * Frees any resources allocated by this background process such as file descriptors or
     * sockets. This method gets called by the container after pause() has been
 called and before the JVM exits. The BackgroundProcess can not be restarted after this
 method has been called without a new call to the init() method.
     */
    public void destroy();
}