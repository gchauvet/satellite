/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package javax.daemon;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.LinkedList;
import java.util.List;
import java.util.jar.Manifest;
import java.util.zip.ZipFile;

/**
 * Wrapper to manage the BackgroundProcess instance.
 */
final class BackgroundWrapper {

    private Controller controller = null;
    private Object instance = null;

    public boolean check(String cn) {
        /* Check the class name */
        if (cn == null) {
            throw new NullPointerException("Null class name specified");
        }

        try {
            /* Find the required class */
            Class<?> c = Class.forName(cn, true, this.getClass().getClassLoader());

            /* This should _never_ happen, but doublechecking doesn't harm */
            if (c == null) {
                throw new ClassNotFoundException(cn);
            }

            /* Create a new instance of the background process */
            c.newInstance();

        } catch (Exception ex) {
            return false;
        }
        /* The class was loaded and instantiated correctly, we can return
         */
        return true;
    }

    /**
     * Load the BackgroundProcess class entry-point.
     *
     * @param jarName Define explicitely then jar who contains the entry point
     * defined in the MANIFEST.MF (BackgroundProcess-Class property)
     * @param args Arguments to provide to the BackgroundContext
     * @return true if loaded.
     */
    public boolean load(final String jarName, final String args[]) {
        boolean result = false;
        try {
            if (jarName == null) {
                throw new IllegalArgumentException("No main jar provided");
            }

            controller = new Controller();
            /* Set the availability flag in the controller */
            controller.setAvailable(false);

            final Context context = new Context();
            context.setArguments(args != null ? args : new String[0]);
            context.setController(controller);

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

            final URLClassLoader loader = new URLClassLoader(urls.toArray(new URL[0]), this.getClass().getClassLoader());
            final Class<?> c = Class.forName(manifest.getMainAttributes().getValue("Background-Process-Class"), true, loader);
            instance = c.newInstance();
            ((BackgroundProcess) instance).initialize(context);
            result = true;
        } catch (InvocationTargetException e) {
            final Throwable thrown = e.getTargetException();
            /* BackgroundExceptions can fail with a nicer message */
            if (thrown instanceof BackgroundException) {
                failed(((BackgroundException) thrown).getMessageWithCause());
            } else {
                controller.fail(e);
            }
        } catch (Throwable ex) {
            /* In case we encounter ANY error, we dump the stack trace and
            * return false (load, resume and pause won't be called).
             */
            controller.fail(ex);
        }
        /* The class was loaded and instantiated correctly, we can return */
        return result;
    }

    public boolean resume() {
        try {
            /* Attempt to resume the background process */
            if (instance != null) {
                ((BackgroundProcess) instance).resume();
            }
            /* Set the availability flag in the controller */
            if (controller != null) {
                controller.setAvailable(true);
            }
        } catch (Throwable ex) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, resume and pause won't be called).
             */
            controller.fail(ex);
            return false;
        }
        return true;
    }

    public boolean pause() {
        try {
            /* Set the availability flag in the controller */
            if (controller != null) {
                controller.setAvailable(false);
            }
            /* Attempt to pause the background process */
            if (instance != null) {
                ((BackgroundProcess) instance).pause();
            }
        } catch (Throwable ex) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, resume and pause won't be called).
             */
            controller.fail(ex);
            return false;
        }
        return true;
    }

    public boolean shutdown() {
        try {
            /* Attempt to shutdown the background process */
            if (instance != null) {
                ((BackgroundProcess) instance).shutdown();
            }
            instance = null;
            controller = null;
        } catch (Throwable ex) {
            /* In case we encounter ANY error, we dump the stack trace and
             * return false (load, resume and pause won't be called).
             */
            controller.fail(ex);
            return false;
        }
        return true;
    }

    private native void shutdown(boolean reload);

    private native void failed(String message);

    private final class Controller implements BackgroundController {

        private boolean available = false;

        private Controller() {
        }

        private synchronized boolean isAvailable() {
            return this.available;
        }

        private synchronized void setAvailable(boolean available) {
            this.available = available;
        }

        public synchronized void shutdown() throws IllegalStateException {
            if (!this.isAvailable()) {
                throw new IllegalStateException();
            }
            this.setAvailable(false);
            BackgroundWrapper.this.shutdown(false);
        }

        public synchronized void reload() throws IllegalStateException {
            if (!this.isAvailable()) {
                throw new IllegalStateException();
            }
            this.setAvailable(false);
            BackgroundWrapper.this.shutdown(true);
        }

        public void fail() {
            fail(null, null);
        }

        public void fail(String message) {
            fail(message, null);
        }

        public void fail(Throwable exception) {
            fail(null, exception);
        }

        public synchronized void fail(String message, Throwable exception) {
            this.setAvailable(false);
            BackgroundWrapper.this.failed(new BackgroundException(message, exception).getMessageWithCause());
        }

    }

    private final class Context implements BackgroundContext {

        private BackgroundController controller = null;

        private String[] args = null;

        private Context() {
        }

        public BackgroundController getController() {
            return controller;
        }

        public void setController(BackgroundController controller) {
            this.controller = controller;
        }

        public String[] getArguments() {
            return args;
        }

        public void setArguments(String[] args) {
            this.args = args;
        }
    }
}
