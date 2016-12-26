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
import java.util.jar.JarEntry;
import java.util.jar.JarInputStream;

public final class DaemonClassLoader extends ClassLoader {

    private final byte[] content;
    final ClassLoader parent;

    public DaemonClassLoader(final byte[] content, final ClassLoader parent) {
        super(DaemonClassLoader.class.getClassLoader());
        this.content = content;
        this.parent = parent;
    }

    @Override
    protected Class<?> loadClass(String name, boolean resolve) throws ClassNotFoundException {
        Class<?> clazz = parent.loadClass(name);
        if (clazz == null) {
            clazz = findLoadedClass(name);
        }
        if (clazz == null) {
            try {
                final InputStream in = getResourceAsStream(name.replace('.', '/') + ".class");
                final ByteArrayOutputStream out = new ByteArrayOutputStream();
                int n;
                byte[] buffer = new byte[4096];
                while ((n = in.read(buffer)) > 0) {
                    out.write(buffer, 0, n);
                }
                final byte[] bytes = out.toByteArray();
                clazz = defineClass(name, bytes, 0, bytes.length);
                if (resolve) {
                    resolveClass(clazz);
                }
            } catch (IOException ex) {
                throw new ClassNotFoundException(ex.getMessage(), ex);
            }
        }
        return clazz;
    }

    @Override
    public InputStream getResourceAsStream(String name) {
        try {
            final JarInputStream jis = new JarInputStream(new ByteArrayInputStream(content));
            JarEntry entry;
            while ((entry = jis.getNextJarEntry()) != null) {
                if (entry.getName().equals(name)) {
                    return jis;
                }
            }
        } catch (IOException e) {
        }
        return null;
    }

}
