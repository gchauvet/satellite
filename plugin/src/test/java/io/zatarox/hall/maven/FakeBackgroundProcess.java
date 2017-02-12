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
import java.io.IOException;
import java.net.ServerSocket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import static org.junit.Assert.*;

public final class FakeBackgroundProcess implements BackgroundProcess {

    private final ExecutorService executor = Executors.newSingleThreadExecutor();
    private ServerSocket socket;

    @Override
    public void init(BackgroundContext dc) throws BackgroundException, Exception {
        assertArrayEquals(new String[]{"test 1", "test 2"}, dc.getArguments());
        try {
            dc.getController();
            fail("need exception");
        } catch (UnsupportedOperationException ex) {
        }
        socket = new ServerSocket(0);
    }

    @Override
    public void start() throws Exception {
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    socket.accept();
                } catch (IOException ex) {
                    throw new RuntimeException(ex);
                }
            }
        });
    }

    @Override
    public void stop() throws Exception {
        socket.close();
        executor.shutdown();
    }

    @Override
    public void destroy() {
        socket = null;
    }

}
