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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Properties;
import java.text.ParseException;

/**
 * Define Hall configuration settings.
 * <p>
 * Configuration is read from properties file. If no properties file is given
 * the <code>hall.properties</code> is used from the current directory.
 * </p>
 * <p>
 * The properties file can have property values expanded at runtime by using
 * System properties or execution environment. The part of the property value
 * between <code>${</code> and <code>}</code> will be used as System property or
 * environment key. If found then the entire <code>${foo}</code> will be
 * replaced by the value of either system property or environment variable named
 * <code>foo</code>.
 * </p>
 * <p>
 * If no variable is found the <code>${foo}</code> will be passed as is. In case
 * of <code>$${foo}</code> this will be unescaped and resulting value will be
 * <code>${foo}</code>.
 * </p>
 */
public final class BackgroundConfiguration {

    /**
     * Default configuration file name.
     */
    protected final static String DEFAULT_CONFIG = "halll.properties";
    /**
     * Property prefix
     */
    protected final static String PREFIX = "hall.";
    private final static String BTOKEN = "${";
    private final static String ETOKEN = "}";

    private final Properties configurationProperties;
    private final Properties systemProperties;

    /**
     * Default constructor
     */
    public BackgroundConfiguration() {
        configurationProperties = new Properties();
        systemProperties = System.getProperties();
    }

    /**
     * Loads the configuration properties file.
     *
     * @param fileName The properties file to load.
     * @return <code>true</code> if the file was loaded.
     */
    public boolean load(File fileName) {
        boolean ok = false;
        FileInputStream file = null;
        try {
            if (fileName == null) {
                fileName = new File(DEFAULT_CONFIG);
            }
            file = new FileInputStream(fileName);
            configurationProperties.clear();
            configurationProperties.load(file);
            ok = true;
        } catch (FileNotFoundException ex) {
            // fileName does not exist
        } catch (IOException ex) {
            // Error reading properties file
        } finally {
            try {
                if (file != null) {
                    file.close();
                }
            } catch (IOException ex) {
            }
        }
        return ok;
    }

    private String expandProperty(String propValue)
            throws ParseException {
        StringBuffer expanded;
        int btoken;
        int ctoken = 0;

        if (propValue == null) {
            return null;
        }
        expanded = new StringBuffer();
        btoken = propValue.indexOf(BTOKEN);
        while (btoken != -1) {
            if (btoken > 0 && propValue.charAt(btoken - 1) == BTOKEN.charAt(0)) {
                // Skip and unquote.
                expanded.append(propValue.substring(ctoken, btoken));
                ctoken = btoken + 1;
                btoken = propValue.indexOf(BTOKEN, btoken + BTOKEN.length());
                continue;
            }
            int etoken = propValue.indexOf(ETOKEN, btoken);
            if (etoken != -1) {
                String variable = propValue.substring(btoken + BTOKEN.length(), etoken);
                String sysvalue = systemProperties.getProperty(variable);
                if (sysvalue == null) {
                    // Try with the environment if there was no
                    // property by that name.
                    sysvalue = System.getenv(variable); // N.B. Deprecated in Java 1.3/1.4, but re-instated in Java 1.5+
                }
                if (sysvalue != null) {
                    String strtoken = propValue.substring(ctoken, btoken);
                    expanded.append(strtoken);
                    expanded.append(sysvalue);
                    ctoken = etoken + ETOKEN.length();
                }
            } else {
                // We have "${" without "}"
                throw new ParseException("Error while looking for teminating '"
                        + ETOKEN + "'", btoken);
            }
            btoken = propValue.indexOf(BTOKEN, etoken + ETOKEN.length());
        }
        // Add what's left.
        expanded.append(propValue.substring(ctoken, propValue.length()));
        return expanded.toString();
    }

    /**
     * Gets the configuration property.
     *
     * @param name The name of the property to get.
     * @return
     *
     * @throws ParseException if the property is wrongly formatted.
     */
    public String getProperty(String name) throws ParseException {
        if (name == null) {
            return null;
        }
        return expandProperty(configurationProperties.getProperty(PREFIX + name));
    }

    /**
     * Gets the configuration property array.
     * <p>
     * Property array is constructed form the list of properties which end with
     * <code>[index]</code>
     * </p>
     * <pre>
     * hall.arg[0] = argument 1
     * hall.arg[1] = argument 2
     * hall.arg[2] = argument 3
     * </pre>
     *
     * @param name The name of the property array to get.
     * @return
     *
     * @throws ParseException if the property is wrongly formatted.
     */
    public String[] getPropertyArray(String name)
            throws ParseException {
        ArrayList<String> list = new ArrayList<String>();
        String args;

        // Load hall.arg[0] ... hall.arg[n] into the String array.
        while ((args = getProperty(name + "[" + list.size() + "]")) != null) {
            list.add(args);
        }
        return list.toArray(new String[list.size()]);
    }
}
