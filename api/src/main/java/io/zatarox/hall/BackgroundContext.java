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
package io.zatarox.hall;

/**
 * Defines a set of methods that a BackgroundProcess instance can use to communicate with
 * the container.
 */
public interface BackgroundContext {

    /**
     * Returns <code>BackgroundController</code> object that can be used to control
     * the instance that this <code>BackgroundContext</code> is
     * passed to.
     */
    public BackgroundController getController();

    /**
     * Returns an array of <code>String</code> arguments supplied by the
     * environment. corresponding to the array of arguments given in the
     * <code>public static void main()</code> method used as an entry point to
     * most other java programs.
     */
    public String[] getArguments();

}
