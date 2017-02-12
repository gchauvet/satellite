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

import java.rmi.RemoteException;
import org.apache.commons.daemon.Daemon;
import org.apache.commons.daemon.DaemonContext;
import org.apache.commons.daemon.DaemonInitException;
import org.junit.*;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.*;
import static org.junit.Assert.*;
import org.junit.runner.RunWith;
import org.mockito.runners.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public final class DaemonRemoteProxyTest {

    @Mock
    private Daemon daemon;
    @InjectMocks
    private DaemonRemoteProxy proxy;

    @Test
    public void initSuccess() throws Exception {
        final DaemonContext context = mock(DaemonContext.class);
        proxy.init(context);
        verify(daemon, times(1)).init(eq(context));
    }

    @Test(expected = RemoteException.class)
    public void initError() throws Exception {
        final DaemonContext context = mock(DaemonContext.class);
        doThrow(new DaemonInitException(null)).when(daemon).init(same(context));
        proxy.init(context);
        fail();
    }

    @Test
    public void startSuccess() throws Exception {
        proxy.start();
        verify(daemon, times(1)).start();
    }

    @Test(expected = RemoteException.class)
    public void startError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(daemon).start();
        proxy.start();
    }

    @Test
    public void stopSuccess() throws Exception {
        proxy.stop();
        verify(daemon, times(1)).stop();
    }

    @Test(expected = RemoteException.class)
    public void stopError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(daemon).stop();
        proxy.stop();
    }

    @Test
    public void destroySuccess() throws Exception {
        proxy.destroy();
        verify(daemon, times(1)).destroy();
    }

    @Test(expected = UnsupportedOperationException.class)
    public void destroyError() throws Exception {
        doThrow(UnsupportedOperationException.class).when(daemon).destroy();
        proxy.destroy();
    }

}
