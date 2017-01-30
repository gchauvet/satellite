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
package org.apache.commons.daemon.support;

import org.apache.commons.daemon.DaemonContext;
import org.apache.commons.daemon.DaemonController;
import org.apache.commons.daemon.DaemonInitException;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Used by jsvc for Daemon management.
 *
 * @version $Id$
 */
public final class DaemonLoader {

    // N.B. These static mutable variables need to be accessed using synch.
    private Controller controller = null; //@GuardedBy("this")
    private Object daemon = null; //@GuardedBy("this")
    /* Methods to call */
    private Method init = null; //@GuardedBy("this")
    private Method start = null; //@GuardedBy("this")
    private Method stop = null; //@GuardedBy("this")
    private Method destroy = null; //@GuardedBy("this")
    private Method signal = null; //@GuardedBy("this")

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
            if (signal != null) {
                signal.invoke(daemon, new Object[0]);
                return true;
            }
            System.out.println("Daemon doesn't support signaling");
        } catch (Throwable ex) {
            System.err.println("Cannot send signal: " + ex);
            ex.printStackTrace(System.err);
        }
        return false;
    }

    public boolean load(String className, String args[]) {
        try {
            /* Check if the underlying library supplied a valid list of
               arguments */
            if (args == null) {
                args = new String[0];
            }

            /* Check the class name */
            if (className == null) {
                throw new NullPointerException("Null class name specified");
            }

            final Class<?> c = Class.forName(className, true, loader);

            /* Check interfaces */
            final boolean isdaemon = Class.forName("org.apache.commons.daemon.Daemon", true, loader).isAssignableFrom(c);
            /* Check methods */
            Class<?>[] myclass = new Class[1];
            if (isdaemon) {
                myclass[0] = DaemonContext.class;
            } else {
                myclass[0] = args.getClass();
            }

            init = c.getMethod("init", myclass);

            myclass = null;
            start = c.getMethod("start", myclass);
            stop = c.getMethod("stop", myclass);
            destroy = c.getMethod("destroy", myclass);

            try {
                signal = c.getMethod("signal", myclass);
            } catch (NoSuchMethodException e) {
                // Signalling will be disabled.
            }

            /* Create a new instance of the daemon */
            daemon = c.newInstance();

            if (isdaemon) {
                /* Create a new controller instance */
                controller = new Controller();

                /* Set the availability flag in the controller */
                controller.setAvailable(false);

                /* Create context */
                Context context = new Context();
                context.setArguments(args);
                context.setController(controller);

                /* Now we want to call the init method in the class */
                Object arg[] = new Object[1];
                arg[0] = context;
                init.invoke(daemon, arg);
            } else {
                Object arg[] = new Object[1];
                arg[0] = args;
                init.invoke(daemon, arg);
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
            Object arg[] = null;
            start.invoke(daemon, arg);

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
            Object arg[] = null;
            stop.invoke(daemon, arg);
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
            Object arg[] = null;
            destroy.invoke(daemon, arg);

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

    private class Controller
            implements DaemonController {

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

    private class Context
            implements DaemonContext {

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
