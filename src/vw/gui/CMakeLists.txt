set(TARGET vwv)

set( MOC_HEADERS
  GlPreviewWidget.h
  MainWindow.h
  TileGenerator.h
)

set(HEADERS
  ${MOC_HEADERS}
  TextureCache.h
)

set(SOURCES 
  TextureCache.cc
  vwv.cc
)

qt4_wrap_cpp(MOC_FILES ${MOC_HEADERS})

# note that Qt convention is not to <QtModule/foo.h>, just to have <foo.h>. 
# but the source include <QtModule/foo>, so we have to add a /.. entry
include_directories( 
  ${QT_QTCORE_INCLUDE_DIR}/.. 
  ${QT_QTGUI_INCLUDE_DIR}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTOPENGL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
)

link_libraries( 
  vwCore 
  ${QT_QTGUI_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTOPENGL_LIBRARY}
  ${OPENGL_LIBRARIES}
)

add_executable(${TARGET}
  ${SOURCES}
  ${HEADERS}
  ${MOC_FILES}
)

## install 
########################################
install_targets( /bin ${TARGET} )

