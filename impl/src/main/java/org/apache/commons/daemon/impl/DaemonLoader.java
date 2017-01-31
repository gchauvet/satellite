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
package org.apache.commons.daemon.impl;

import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.util.Collections;
import java.util.jar.Manifest;
import org.apache.commons.daemon.*;
import org.apache.commons.daemon.Daemon;

/**
 * Used by jsvc for Daemon management.
 *
 * @version $Id$
 */
public final class DaemonLoader {

    // N.B. These static mutable variables need to be accessed using synch.
    private Controller controller = null;
    private Object daemon = null;
    private final ClassLoader loader;

    public DaemonLoader(ClassLoader loader) {
        if (loader == null) {
            throw new IllegalArgumentException("No embedded classloader provided");
        }
        this.loader = loader;
    }

    public void version() {
        System.err.println("java version \""
                + System.getProperty("java.version") + "\"");
        System.err.println(System.getProperty("java.runtime.name")
                + " (build "
                + System.getProperty("java.runtime.version") + ")");
        System.err.println(System.getProperty("java.vm.name")
                + " (build "
                + System.getProperty("java.vm.version")
                + ", " + System.getProperty("java.vm.info") + ")");
        System.err.println("commons daemon version \""
                + System.getProperty("commons.daemon.version") + "\"");
        System.err.println("commons daemon process (id: "
                + System.getProperty("commons.daemon.process.id")
                + ", parent: "
                + System.getProperty("commons.daemon.process.parent") + ")");
    }

    public boolean check(String cn) {
        try {
            /* Check the class name */
            if (cn == null) {
                throw new NullPointerException("Null class name specified");
            }

            /* Find the required class */
            Class<?> c = Class.forName(cn, true, loader);

            /* This should _never_ happen, but doublechecking doesn't harm */
            if (c == null) {
                throw new ClassNotFoundException(cn);
            }

            /* Create a new instance of the daemon */
            c.newInstance();

        } catch (Throwable t) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, start and stop won't be called).
             */
            t.printStackTrace(System.err);
            return false;
        }
        /* The class was loaded and instantiated correctly, we can return
         */
        return true;
    }

    public boolean signal() {
        try {
            if (daemon instanceof DaemonUserSignal) {
                ((DaemonUserSignal) daemon).signal();
                return true;
            }
            System.out.println("Daemon doesn't support signaling");
        } catch (Throwable ex) {
            System.err.println("Cannot send signal: " + ex);
            ex.printStackTrace(System.err);
        }
        return false;
    }

    /**
     * Load the Daemon class entry-point.
     *
     * @param className Define explicitely then classname of entry point. If
     * null, search the first MANIFEST.MF who contains Daemon-Class property
     * @param args Arguments to provide to the DaemonContext
     * @return true if loaded.
     */
    public boolean load(String className, String args[]) {
        try {
            /* Check if the underlying library supplied a valid list of
               arguments */
            if (args == null) {
                args = new String[0];
            }

            /* Check the class name */
            if (className == null) {
                // Search if the class is defined in one of the MANIFEST.MF (first used)
                for (URL url : Collections.list(getClass().getClassLoader().getResources("META-INF/MANIFEST.MF"))) {
                    final Manifest manifest = new Manifest(url.openStream());
                    className = manifest.getMainAttributes().getValue("Daemon-Class");
                    if (className != null) {
                        final Class<?> c = Class.forName(className, true, loader);
                        daemon = c.newInstance();
                        controller = new Controller();
                        /* Set the availability flag in the controller */
                        controller.setAvailable(false);

                        /* Create context */
                        final Context context = new Context();
                        context.setArguments(args);
                        context.setController(controller);

                        ((Daemon) daemon).init(context);
                        break;
                    }
                }
                if (daemon == null) {
                    throw new IllegalArgumentException("Classname unspecified");
                }
            }
        } catch (InvocationTargetException e) {
            Throwable thrown = e.getTargetException();
            /* DaemonInitExceptions can fail with a nicer message */
            if (thrown instanceof DaemonInitException) {
                failed(((DaemonInitException) thrown).getMessageWithCause());
            } else {
                thrown.printStackTrace(System.err);
            }
            return false;
        } catch (Throwable t) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, start and stop won't be called).
             */
            t.printStackTrace(System.err);
            return false;
        }
        /* The class was loaded and instantiated correctly, we can return */
        return true;
    }

    public boolean start() {
        try {
            /* Attempt to start the daemon */
            ((Daemon) daemon).start();

            /* Set the availability flag in the controller */
            if (controller != null) {
                controller.setAvailable(true);
            }

        } catch (Throwable t) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, start and stop won't be called).
             */
            t.printStackTrace(System.err);
            return false;
        }
        return true;
    }

    public boolean stop() {
        try {
            /* Set the availability flag in the controller */
            if (controller != null) {
                controller.setAvailable(false);
            }

            /* Attempt to stop the daemon */
            ((Daemon) daemon).stop();
        } catch (Throwable t) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, start and stop won't be called).
             */
            t.printStackTrace(System.err);
            return false;
        }
        return true;
    }

    public boolean destroy() {
        try {
            /* Attempt to stop the daemon */
            ((Daemon) daemon).destroy();

            daemon = null;
            controller = null;
        } catch (Throwable t) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, start and stop won't be called).
             */
            t.printStackTrace(System.err);
            return false;
        }
        return true;
    }

    private native void shutdown(boolean reload);

    private native void failed(String message);

    private final class Controller implements DaemonController {

        private boolean available = false;

        private Controller() {
            super();
            this.setAvailable(false);
        }

        private boolean isAvailable() {
            synchronized (this) {
                return this.available;
            }
        }

        private void setAvailable(boolean available) {
            synchronized (this) {
                this.available = available;
            }
        }

        public void shutdown()
                throws IllegalStateException {
            synchronized (this) {
                if (!this.isAvailable()) {
                    throw new IllegalStateException();
                }
                this.setAvailable(false);
                DaemonLoader.this.shutdown(false);
            }
        }

        public void reload()
                throws IllegalStateException {
            synchronized (this) {
                if (!this.isAvailable()) {
                    throw new IllegalStateException();
                }
                this.setAvailable(false);
                DaemonLoader.this.shutdown(true);
            }
        }

        public void fail() {
            fail(null, null);
        }

        public void fail(String message) {
            fail(message, null);
        }

        public void fail(Exception exception) {
            fail(null, exception);
        }

        public void fail(String message, Exception exception) {
            synchronized (this) {
                this.setAvailable(false);
                String msg = message;
                if (exception != null) {
                    if (msg != null) {
                        msg = msg + ": " + exception.toString();
                    } else {
                        msg = exception.toString();
                    }
                }
                DaemonLoader.this.failed(msg);
            }
        }

    }

    private final class Context implements DaemonContext {

        private DaemonController daemonController = null;

        private String[] args = null;

        public DaemonController getController() {
            return daemonController;
        }

        public void setController(DaemonController controller) {
            this.daemonController = controller;
        }

        public String[] getArguments() {
            return args;
        }

        public void setArguments(String[] args) {
            this.args = args;
        }

    }
}
