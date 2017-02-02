ADD_EXECUTABLE (prunmgr WIN32 prunmgr.c prunmgr.rc)
TARGET_LINK_LIBRARIES (prunmgr daemon netapi32 comctl32)
INSTALL(TARGETS prunmgr RUNTIME DESTINATION ../../bin)
