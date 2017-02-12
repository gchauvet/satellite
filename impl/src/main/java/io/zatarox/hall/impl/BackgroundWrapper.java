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
package io.zatarox.hall.impl;

import io.zatarox.hall.BackgroundException;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.Manifest;
import java.util.zip.ZipFile;
import io.zatarox.hall.BackgroundProcess;
import io.zatarox.hall.SignalListener;
import io.zatarox.hall.BackgroundContext;
import io.zatarox.hall.BackgroundController;

/**
 * Used by jsvc for BackgroundProcess management.
 */
public final class BackgroundWrapper {

    // N.B. These static mutable variables need to be accessed using synch.
    private Controller controller = null;
    private Object daemon = null;
    private final ClassLoader loader;

    public BackgroundWrapper(ClassLoader loader) {
        if (loader == null) {
            throw new IllegalArgumentException("No embedded classloader provided");
        }
        this.loader = loader;
    }

    public void version() {
        System.out.println(String.format("java version \"%s\"", System.getProperty("java.version")));
        System.out.println(String.format("%s (build %s)", System.getProperty("java.runtime.name"), System.getProperty("java.runtime.version")));
        System.out.println(String.format("%s (build %s, %s)", System.getProperty("java.vm.name"), System.getProperty("java.vm.version"), System.getProperty("java.vm.info")));
        System.out.println(String.format("commons daemon version \"%s\"", System.getProperty("commons.daemon.version")));
        System.out.println(String.format("commons daemon process (id: %s, parent: %s)", System.getProperty("commons.daemon.process.id"), System.getProperty("commons.daemon.process.parent")));
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
            if (daemon instanceof SignalListener) {
                ((SignalListener) daemon).signal();
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
     * Load the BackgroundProcess class entry-point.
     *
     * @param jarName Define explicitely then jar who contains the entry point
 defined in the MANIFEST.MF (BackgroundProcess-Class property)
     * @param args Arguments to provide to the BackgroundContext
     * @return true if loaded.
     */
    public boolean load(final String jarName, final String args[]) {
        try {
            if (jarName == null) {
                throw new IllegalArgumentException("No main jar provided");
            }

            final File jar = new File(jarName);
            final ZipFile archive = new ZipFile(jar);
            final List<URL> urls = new LinkedList<URL>();
            final Manifest manifest;
            try {
                manifest = new Manifest(archive.getInputStream(archive.getEntry("META-INF/MANIFEST.MF")));
                final String paths = manifest.getMainAttributes().getValue("Class-Path");
                urls.add(new File(jarName).toURI().toURL());
                if (paths != null) {
                    for (String path : paths.split("\\s+")) {
                        urls.add(new File(jar.getParentFile(), path).toURI().toURL());
                    }
                }
            } finally {
                try {
                    archive.close();
                } catch (IOException ex) {
                }
            }

            final URLClassLoader loader = new URLClassLoader(urls.toArray(new URL[0]), this.loader);
            final Class<?> c = Class.forName(manifest.getMainAttributes().getValue("Background-Process-Class"), true, loader);
            daemon = c.newInstance();
            controller = new Controller();
            /* Set the availability flag in the controller */
            controller.setAvailable(false);

            /* Create context */
            final Context context = new Context();
            context.setArguments(args != null ? args : new String[0]);
            context.setController(controller);

            ((BackgroundProcess) daemon).init(context);
        } catch (InvocationTargetException e) {
            Throwable thrown = e.getTargetException();
            /* DaemonInitExceptions can fail with a nicer message */
            if (thrown instanceof BackgroundException) {
                failed(((BackgroundException) thrown).getMessageWithCause());
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
            ((BackgroundProcess) daemon).start();

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
            ((BackgroundProcess) daemon).stop();
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
            ((BackgroundProcess) daemon).destroy();

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

    private final class Controller implements BackgroundController {

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
                BackgroundWrapper.this.shutdown(false);
            }
        }

        public void reload()
                throws IllegalStateException {
            synchronized (this) {
                if (!this.isAvailable()) {
                    throw new IllegalStateException();
                }
                this.setAvailable(false);
                BackgroundWrapper.this.shutdown(true);
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
                BackgroundWrapper.this.failed(msg);
            }
        }

    }

    private final class Context implements BackgroundContext {

        private BackgroundController daemonController = null;

        private String[] args = null;

        public BackgroundController getController() {
            return daemonController;
        }

        public void setController(BackgroundController controller) {
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
