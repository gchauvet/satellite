/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package org.apache.commons.daemon.maven;

import java.io.IOException;
import java.net.ServerSocket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.apache.commons.daemon.*;
import static org.junit.Assert.*;

public final class FakeDaemon implements Daemon {

    private final ExecutorService executor = Executors.newSingleThreadExecutor();
    private ServerSocket socket;

    @Override
    public void init(DaemonContext dc) throws DaemonInitException, Exception {
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
