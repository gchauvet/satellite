#!/usr/bin/sh
# Your service name
SERVICE_NAME=test
# Your daemon base directory
SERVICE_DIRECTORY=/opt/to/your/project
# Your main Jar
MAIN_JAR=.jar
# Your Java home
DAEMON_JAVA_HOME=/usr/lib/jvm/default-java
# Associated pid file
PID_FILE=/var/run/${SERVICE_NAME}.pid
 
usage()
{
        echo "Usage: $0 (shutdown|start|pause|resume|restart)"
}
 
service_start()
{
        echo "Starting service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	./deimos -cwd ${SERVICE_DIRECTORY} -home ${DAEMON_JAVA_HOME} -pidfile ${PID_FILE} ${MAIN_JAR}
        cd $OWD
        echo "Service '${SERVICE_NAME}' started successfully"
}

service_pause()
{
        echo "Pause service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	./deimos -cwd ${SERVICE_DIRECTORY} -home ${DAEMON_JAVA_HOME} pause -pidfile ${PID_FILE}
        cd $OWD
        echo "Service '${SERVICE_NAME}' paused"
}

service_resume()
{
        echo "Resume service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	./deimos -cwd ${SERVICE_DIRECTORY} -home ${DAEMON_JAVA_HOME} resume -pidfile ${PID_FILE}
        cd $OWD
        echo "Service '${SERVICE_NAME}' resumed"
}
 
service_shutdown()
{
        echo "Stopping service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	./deimos -cwd ${SERVICE_DIRECTORY} -home ${DAEMON_JAVA_HOME} shutdown -pidfile ${PID_FILE}
        cd $OWD
        echo "Service '${SERVICE_NAME}' stopped"
}
 
case $1 in
        shutdown)
                service_shutdown
        ;;
        start)
                service_start
        ;;
        pause)
                service_pause
        ;;
        resume)
                service_resume
        ;;
        restart)
                service_stop
                service_start
        ;;
        *)
                usage
esac
exit 0
