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

public final class DaemonRemoteProxy implements DaemonRemote {

    private final Daemon instance;

    public DaemonRemoteProxy(Daemon instance) {
        this.instance = instance;
    }

    @Override
    public void init(DaemonContext dc) throws RemoteException {
        try {
            instance.init(dc);
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void start() throws RemoteException {
        try {
            instance.start();
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void stop() throws RemoteException {
        try {
            instance.stop();
        } catch (Exception ex) {
            throw new RemoteException(ex.getMessage(), ex);
        }
    }

    @Override
    public void destroy() {
        instance.destroy();
    }

}
