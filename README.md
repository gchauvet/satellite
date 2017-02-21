#Hall Project (Work in Progress)

Hall is a set of utilities and Java support classes for running Java applications as server processes.
Commonly known as 'daemon' processes in Unix terminology, on Windows they are called 'services'.

[![Build Status](https://travis-ci.org/gchauvet/hall.png)](https://travis-ci.org/gchauvet/hall)
[![Coverage Status](https://coveralls.io/repos/gchauvet/hall/badge.png?branch=master)](https://coveralls.io/r/gchauvet/hall?branch=master)

## Requirements
* Hall project requires Maven 3.x
* Java Development Kit >= 7

### To compile Windows frontend (phobos) :
* [tdm-gcc](http://tdm-gcc.tdragon.net)

### To compile Unix frontend (deimos) :
* gcc >= 5.x
* gcc-mingw-w64-x86-64
* gcc-mingw-w64
* libc6-dev-i386

## Hall in action
Writing an independent OS background process become pretty simple. The first step is to implement a java interface who manage background process behaviors. The second step require `Phobos` or `Deimos` depending if you are under Windows or Linux

### Dependency
In your Maven POM project, add the Hall API dependency (check latest version):

```xml
<dependency>
    <groupId>io.zatarox.hall</groupId>
    <artifactId>hall-api</artifactId>
    <version>1.0.0-SNAPSHOT</version>
    <scope>provided</scope>
</dependency>
```

### Core of the background process
This is the facade of your service. Attention : calls on these methods are synchronous.

```java
import io.zatarox.hall.*;

public final class FooBackgroundProcess implements BackgroundProcess {

    @Override
    public void init(BackgroundContext dc) throws BackgroundException, Exception {
        System.err.println("Initialized");
    }

    @Override
    public void start() throws Exception {
        System.err.println("Started...");
    }

    @Override
    public void stop() throws Exception {
        System.err.println("Stopped !");
    }

    @Override
    public void destroy() {
        System.err.println("Destroyed");
    }
}
```
You can implements `SignalListener` if you want to be notified by a signal event (Unix) or by a LPC callback under Windows.

### Usage

#### Linux
* To install
```
TODO
```
* To start :
```sh
sudo deimos --MainJar={filename}.jar
```
* To stop :
```sh
sudo deimos --MainJar={filename}.jar --stop
```
* To send a signal :
```sh
sudo kill -SIGUSR2 {pid}
```

#### Windows
* To install
```batch
phobos.exe //IS/{service name} --MainJar={filname}.jar ...
```
* To start :
```sh
net start {service name}
```
* To pause :
```sh
net pause {service name}
```
* To continue :
```sh
net continue {service name}
```
* To stop :
```sh
net stop {service name}
```
*  To send a signal :
```
TODO
```

## Inspiration
This project is based on Apache Commons Daemon.

## License
The MIT License (MIT)

Copyright (c) 2017 Guillaume Chauvet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
