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
package io.zatarox.hall.maven;

import io.zatarox.hall.*;
import java.rmi.RemoteException;
import org.junit.*;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.*;
import static org.junit.Assert.*;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public final class BackgroundProcessRemoteProxyTest {

    @Mock
    private BackgroundProcess instance;
    @InjectMocks
    private BackgroundProcessRemoteProxy proxy;

    @Test
    public void initSuccess() throws Exception {
        final BackgroundContext context = mock(BackgroundContext.class);
        proxy.init(context);
        verify(instance, times(1)).init(eq(context));
    }

    @Test(expected = RemoteException.class)
    public void initError() throws Exception {
        final BackgroundContext context = mock(BackgroundContext.class);
        doThrow(new BackgroundException(null)).when(instance).init(same(context));
        proxy.init(context);
        fail();
    }

    @Test
    public void startSuccess() throws Exception {
        proxy.start();
        verify(instance, times(1)).start();
    }

    @Test(expected = RemoteException.class)
    public void startError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(instance).start();
        proxy.start();
    }

    @Test
    public void stopSuccess() throws Exception {
        proxy.stop();
        verify(instance, times(1)).stop();
    }

    @Test(expected = RemoteException.class)
    public void stopError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(instance).stop();
        proxy.stop();
    }

    @Test
    public void destroySuccess() throws Exception {
        proxy.destroy();
        verify(instance, times(1)).destroy();
    }

    @Test(expected = UnsupportedOperationException.class)
    public void destroyError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(instance).destroy();
        proxy.destroy();
    }

}
