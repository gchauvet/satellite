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

import java.io.*;
import java.lang.reflect.Constructor;
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

public final class EmbeddedClassLoader extends ClassLoader {

    private final byte[] content;

    private EmbeddedClassLoader(final byte[] content) {
        this.content = content;
    }

    @Override
    protected Class<?> findClass(String name) throws ClassNotFoundException {
        Class<?> result = null;
        try {
            final InputStream in = getResourceAsStream(name.replace('.', '/') + ".class");
            final ByteArrayOutputStream out = new ByteArrayOutputStream();
            final byte[] buffer = new byte[4096];
            int n;
            while ((n = in.read(buffer)) > 0) {
                out.write(buffer, 0, n);
            }
            final byte[] bytes = out.toByteArray();
            result = defineClass(name, bytes, 0, bytes.length);
        } catch (IOException ex) {
        }
        return result;
    }

    @Override
    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        Class<?> result = null;
        try {
            result = findClass(name);
            if (result != null) {
                if (resolve) {
                    resolveClass(result);
                }
            }

        } catch (Throwable ex) {
            result = super.loadClass(name, resolve);
        }
        return result;
    }

    @Override
    public InputStream getResourceAsStream(String name) {
        InputStream result = super.getResourceAsStream(name);
        if (result == null) {
            try {
                final JarInputStream jis = new JarInputStream(new ByteArrayInputStream(content));
                JarEntry entry;
                while ((entry = jis.getNextJarEntry()) != null) {
                    if (entry.getName().equals(name)) {
                        result = jis;
                        break;
                    }
                }
            } catch (IOException e) {
            }
        }
        return result;
    }

    /**
     * Instanciate a DaemonLoader object
     *
     * @param content Bytecodes of embedded jar
     * @return a DaemonLoader as an Object
     * @throws Exception
     */
    static Object createBootstrap(final byte[] content) throws Exception {
        final ClassLoader main = new EmbeddedClassLoader(content);
        final Class<?> clazz = Class.forName("org.apache.commons.daemon.support.DaemonLoader", true, main);
        final Constructor<?> constructor = clazz.getConstructor(ClassLoader.class);
        return constructor.newInstance(main);
    }

}
