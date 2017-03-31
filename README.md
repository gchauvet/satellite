# Satellite Project (Work in Progress)

Satellite is a set of utilities and Java support classes for running Java applications as server processes.
Commonly known as 'daemon' processes in Unix terminology, on Windows they are called 'services'.

[![Build Status](https://travis-ci.org/gchauvet/satellite.png)](https://travis-ci.org/gchauvet/satellite)
[![Coverage Status](https://coveralls.io/repos/gchauvet/satellite/badge.png?branch=master)](https://coveralls.io/r/gchauvet/satellite?branch=master)

## Requirements
* Satellite project requires Maven 3.x
* Java Development Kit >= 7

### To compile Windows frontend (phobos) :
* [tdm-gcc](http://tdm-gcc.tdragon.net)

### To compile Unix frontend (deimos & phobos) :
* gcc >= 5.x
* gcc-mingw-w64-x86-64
* gcc-mingw-w64
* libc6-dev-i386

## Satellite in action
Writing an independent OS background process become pretty simple. The first step is to implement a java interface who manage background process behaviors. The second step require `Phobos` or `Deimos` depending if you are under Windows or Linux

### Dependency
In your Maven POM project, add the Satellite API dependency (check latest version):

```xml
<dependency>
    <groupId>io.zatarox.satellite</groupId>
    <artifactId>satellite-api</artifactId>
    <version>1.0.0-SNAPSHOT</version>
    <scope>provided</scope>
</dependency>
```

### Core of the background process
This is the facade of your service. Attention : calls on these methods are synchronous.

```java
import io.zatarox.satellite.*;

public final class FooBackgroundProcess implements BackgroundProcess {

    @Override
    public void init(BackgroundContext dc) throws BackgroundException, Exception {
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
    public void destroy() {
        System.err.println("Destroyed");
    }
}
```

### Usage

#### Linux
* To install
```
TODO
```
* To start :
```sh
sudo deimos {filename}.jar
```
* To stop :
```sh
sudo deimos {filename}.jar -stop
```
* To pause the background process (stop method) :
```sh
sudo kill -SIGUSR2 {pid}
```
* To resume the background process (start method) :
```sh
sudo kill -SIGUSR1 {pid}
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

## Inspiration
This project is based on Apache Commons Daemon.