ADD_EXECUTABLE (prunsrv prunsrv.c prunsrv.rc)
TARGET_LINK_LIBRARIES (prunsrv daemon)
INSTALL(TARGETS prunsrv RUNTIME DESTINATION ../../bin)
