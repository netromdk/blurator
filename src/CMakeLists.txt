add_executable(
  ${PROJECT_NAME}
  main.cpp

  Util.h
  Util.cpp

  Detector.h
  Detector.cpp

  res.qrc
  )

set_blurator_links(${PROJECT_NAME})

# Windows: Copy shared libs to output folder.
if (WIN32)
  include(misc)

  # Qt DLLs
  set (QT_BIN_DIR "${_qt5Core_install_prefix}/bin/")
  COPY_FILE_GLOB(${QT_BIN_DIR} "Qt.*Core.dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${QT_BIN_DIR} "Qt.*Gui.dll" ${WIN_BIN_DIR})

  # Qt plugins
  set(QT_IMAGEFORMATS "${WIN_BIN_DIR}/imageformats")
  file(MAKE_DIRECTORY ${QT_IMAGEFORMATS})
  COPY_FILE_GLOB(${QT_PLUGINS} "qjpeg.dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${QT_PLUGINS} "qgif.dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${QT_PLUGINS} "qtiff.dll" ${WIN_BIN_DIR})

  # ICU DLLs
  if (ICU_BIN_DIR)
    COPY_FILE_GLOB(${ICU_BIN_DIR} "icuin.*.dll" ${WIN_BIN_DIR})
    COPY_FILE_GLOB(${ICU_BIN_DIR} "icuuc.*.dll" ${WIN_BIN_DIR})
    COPY_FILE_GLOB(${ICU_BIN_DIR} "icudt.*.dll" ${WIN_BIN_DIR})
  endif()

  # OpenCV DLLs
  set(OpenCV_BIN_DIR "${_OpenCV_LIB_PATH}")
  COPY_FILE_GLOB(${OpenCV_BIN_DIR} "opencv_core(.*[^d]).dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${OpenCV_BIN_DIR} "opencv_highgui(.*[^d]).dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${OpenCV_BIN_DIR} "opencv_imgproc(.*[^d]).dll" ${WIN_BIN_DIR})
  COPY_FILE_GLOB(${OpenCV_BIN_DIR} "opencv_objdetect(.*[^d]).dll" ${WIN_BIN_DIR})
endif()