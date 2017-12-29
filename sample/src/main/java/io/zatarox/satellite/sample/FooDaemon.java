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
package io.zatarox.satellite.sample;

import fi.iki.elonen.NanoHTTPD;
import static fi.iki.elonen.NanoHTTPD.*;
import io.zatarox.satellite.*;
import java.util.List;
import java.util.Map;

public final class FooDaemon implements BackgroundProcess {

    private NanoHTTPD instance;

    @Override
    public void initialize(BackgroundContext dc) throws BackgroundException, Exception {
        instance = new NanoHTTPD(8085) {
            @Override
            public Response serve(IHTTPSession session) {
                String msg = "<html><body><h1>Hello server</h1>\n";
                final Map<String, List<String>> parms = session.getParameters();
                if (parms.get("username") == null) {
                    msg += "<form action='?' method='get'>\n  <p>Your name: <input type='text' name='username'></p>\n" + "</form>\n";
                } else {
                    msg += "<p>Hello, " + parms.get("username").get(0) + "!</p>";
                }
                return newFixedLengthResponse(msg + "</body></html>\n");
            }
        };
    }

    @Override
    public void resume() throws Exception {
        instance.start();
    }

    @Override
    public void pause() throws Exception {
        instance.stop();
    }

    @Override
    public void shutdown() {
        instance.closeAllConnections();
        instance.stop();
    }

}
