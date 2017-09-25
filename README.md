<p align="center">
  <img src="https://github.com/gchauvet/satellite/blob/jsr-96/satellite.png"/>
</p>

# JSR-96 Implementation
Satellite is a set of utilities and Java support classes for running Java applications as server processes.
Commonly known as 'daemon' processes in Unix terminology, on Windows they are called 'services'.

[![Build Status](https://travis-ci.org/gchauvet/satellite.png)](https://travis-ci.org/gchauvet/satellite)
[![Coverage Status](https://coveralls.io/repos/github/gchauvet/satellite/badge.svg?branch=jsr-96)](https://coveralls.io/github/gchauvet/satellite?branch=jsr-96)

## Requirements
Compilation process is performed under Linux. Cross-compilation fanboy :-)

* Maven 3.x
* CMake 3.x
* Java Development Kit >= 7
* gcc 5.x
* gcc-mingw-w64-x86-64 (gcc 5.x)
* gcc-mingw-w64 (gcc 5.x)
* libc6-dev-i386

## JSR-96 in action
Writing an independent OS background process become pretty simple. The first step is to implement a java interface who manage background process behaviors. The second step require `Phobos` or `Deimos` depending if you are under Windows or Linux. The Daemon behavior has been designed as the same manner as Windows service. Under Windows, only certain service state transitions are valid. The following diagram shows the valid transitions (credits to Microsoft) :
<p align="center">
  <img src="https://i-msdn.sec.s-msft.com/dynimg/IC498931.png"/>
</p>

### Core of the background process
This is the facade of your service. Attention : calls on these methods are synchronous.

```java
import java.daemon.*;

@BackgroundProcessDescription(name = "Foo", description = "A background process sample")
public final class FooBackgroundProcess implements BackgroundProcess {

    @Override
    public void initialize(BackgroundContext dc) throws BackgroundException, Exception {
        System.err.println("Initialized");
    }

    @Override
    public void resume() throws Exception {
        System.err.println("Started...");
    }

    @Override
    public void pause() throws Exception {
        System.err.println("Stopped !");
    }

    @Override
    public void shutdown() {
        System.err.println("Destroyed");
    }
}
```

### Main jar assembly

Finally, add a manifest entry in your main jar (and dependency jars OFC).
```xml
<plugin>
  <artifactId>maven-jar-plugin</artifactId>
    <version>3.0.2</version>
        <configuration>
        <archive>
            <index>true</index>
            <manifestEntries>
                <Background-Process-Class>io.zatarox.satellite.FooBackgroundProcess</Background-Process-Class>
            </manifestEntries>
        </archive>
    </configuration>
</plugin>
```

### Sample
To a look to the sample project submodule to see how to get binary frontends from Maven

## Inspiration
This project is based on Apache Commons Daemon.
