## Satellite Toolkit : Sample

## Requirements

* Maven 3.x
* Java Development Kit >= 7

## Building
This sample project demonstrate how to use our Satellite toolkit both on Windows and Linux for a simple HTTP server listening on port 8085. The first step is to build this sub-project :
```sh
cd sample
mvn clean package -Px86,x86_64 # if you want x86 and x86_64 frontends
```

Your Maven target folder contains :
* The main jar (satellite-sample-*.jar);
* A lib directory who containing required dependencies;
* one or more frontends depending your previous chose building time (see previous step)

### Usage

When service is running on your localhost, you can access to the service with this URL:  http://localhost:8085.
* Pausing the service freeze the HTTP server (don't accept connections).
* Resuming service reallow the HTTP server to serve static pages.
* Stopping service (service shutdown) exit gracefully the JVM.

#### Linux
Installation

The easy way is to adapt our shell script provided in the deimos root folder and edit them at your convenience.

The hard way can be done like this :
* To start the background process :
```sh
sudo deimos -home $JAVA_HOME {filename}.jar
```
* To pause the background process :
```sh
sudo deimos pause
```
* To resume the background process :
```sh
sudo deimos resume
```
* To stop the background process :
```sh
sudo deimos shutdown
```

To manage multiple services, you must provide your own pid filename.

#### Windows

* To install
```batch
phobos.exe install "service name" --Home %JAVA_HOME% --MainJar {filename}.jar
```
* To start the background process :
```sh
net start "service name"
```
* To pause the background process :
```sh
net pause "service name"
```
* To resume the background process :
```sh
net continue "service name"
```
* To stop the background process :
```sh
net stop "service name}"
```

### Maven
To launch your background process from Maven (initialize & resume methods) :
```sh
mvn io.zatarox.satellite:satellite-maven-plugin:start
```

To launch your background process from Maven (pause & shutdown methods) :
```sh
mvn io.zatarox.satellite:satellite-maven-plugin:stop
```