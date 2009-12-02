set( MODULE    Core )
set( LIB_NAME  ${MODULE_PREFIX}${MODULE_NAME} )

set( HEADERS 
  Cache.h
  CompoundTypes.h
  ConfigParser.h
  Debugging.h
  Exception.h
  Functors.h
  FundamentalTypes.h
  Log.h
  ProgressCallback.h
  Settings.h
  Stopwatch.h
  TerminationHandler.h
  Thread.h
  ThreadPool.h
  TypeDeduction.h
  VarArray.h
)

set( SOURCES
  Cache.cc
  ConfigParser.cc
  Debugging.cc
  Exception.cc
  Log.cc
  ProgressCallback.cc
  Settings.cc
  Stopwatch.cc
  TerminationHandler.cc
  Thread.cc
)

link_libraries( ${Boost_THREAD_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} )

add_library( ${LIB_NAME} ${VW_DEFAULT_LIB_TYPE}
  ${HEADERS}
  ${SOURCES}
)

#-----------------------------------------------------------
install_library( ${LIB_NAME} ) # see SetupInstall.cmake
install_files  ( /include/${MODULE_PREFIX}/${MODULE_NAME} FILES ${HEADERS} )

