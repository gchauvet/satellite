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
package io.zatarox.satellite;

import java.io.*;

/**
 * Throw this during init if you can't initialise yourself for some expected
 * reason. Using this exception will cause the exception's message to come out
 * on stdout, rather than a dirty great stacktrace.
 */
public final class BackgroundException extends Exception {

    public BackgroundException(String message) {
        super(message);
    }

    public BackgroundException(String message, Throwable cause) {
        super(message, cause);
    }

    public String getMessageWithCause() {
        String result = getMessage();
        if (getCause() != null) {
            final ByteArrayOutputStream bos = new ByteArrayOutputStream();
            getCause().printStackTrace(new PrintStream(bos));
            result += ": " + new String(bos.toByteArray());
        }
        return result;
    }

}
