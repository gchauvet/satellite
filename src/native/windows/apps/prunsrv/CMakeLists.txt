ADD_EXECUTABLE (prunsrv prunsrv.c prunsrv.rc)
TARGET_LINK_LIBRARIES (prunsrv daemon)
INSTALL(TARGETS prunsrv RUNTIME DESTINATION ${CMAKE_HOME_DIRECTORY}/../../../target/bin)
