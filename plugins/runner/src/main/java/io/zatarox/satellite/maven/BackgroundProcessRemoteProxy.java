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
package io.zatarox.satellite.maven;

import io.zatarox.satellite.BackgroundContext;
import io.zatarox.satellite.BackgroundProcess;
import java.rmi.RemoteException;

public final class BackgroundProcessRemoteProxy implements BackgroundProcessRemote {

    private final BackgroundProcess instance;

    public BackgroundProcessRemoteProxy(BackgroundProcess instance) {
        this.instance = instance;
    }

    @Override
    public void init(BackgroundContext dc) throws RemoteException {
        try {
            instance.initialize(dc);
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void start() throws RemoteException {
        try {
            instance.resume();
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void stop() throws RemoteException {
        try {
            instance.pause();
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void destroy() {
        instance.shutdown();
    }

}
