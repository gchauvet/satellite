SET_SOURCE_FILES_PROPERTIES(prunmgr.rc PROPERTIES LANGUAGE RC)
ADD_EXECUTABLE (prunmgr prunmgr.c prunmgr.rc)
TARGET_LINK_LIBRARIES (prunmgr daemon netapi32 comctl32)