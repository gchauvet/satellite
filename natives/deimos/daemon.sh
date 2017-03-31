# Your service name
SERVICE_NAME=
# Your daemon base directory
SERVICE_DIRECTORY=/opt/Portal/bin
# Your main Jar
MAIN_JAR=MyDaemon.jar
# Your Java home
DAEMON_JAVA_HOME=%JAVA_HOME%
# Associated pid file
PID_FILE=/var/run/${SERVICE_NAME}.pid
 
usage()
{
        echo "-----------------------"
        echo "Usage: $0 (shutdown|start|pause|resume|restart)"
        echo "-----------------------"
}
 
if [ -z $1 ]; then
        usage
fi
 
service_start()
{
        echo "Starting service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	deimos -cp ${DAEMON_JAVA_HOME} -pidfile ${PID_FILE} ${MAIN_JAR}
        cd $OWD
        echo "Service '${SERVICE_NAME}' started successfully"
}

service_pause()
{
        echo "Pause service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	deimos -cp ${DAEMON_JAVA_HOME} -pause -pidfile ${PID_FILE}
        cd $OWD
        echo "Service '${SERVICE_NAME}' paused"
}

service_resume()
{
        echo "Resume service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	deimos -cp ${DAEMON_JAVA_HOME} -resume -pidfile ${PID_FILE}
        cd $OWD
        echo "Service '${SERVICE_NAME}' resumed"
}
 
service_shutdown()
{
        echo "Stopping service '${SERVICE_NAME}'..."
        OWD=`pwd`
        cd ${SERVICE_DIRECTORY}
	deimos -cp ${DAEMON_JAVA_HOME} -stop -pidfile ${PID_FILE}
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