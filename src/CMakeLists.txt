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
  # Qt DLLs
  set (QT_BIN_DIR "${_qt5Core_install_prefix}/bin/")
  file(
    COPY "${QT_BIN_DIR}/Qt5Core.dll"
    DESTINATION ${WIN_BIN_DIR}
    )
  file(
    COPY "${QT_BIN_DIR}/Qt5Gui.dll"
    DESTINATION ${WIN_BIN_DIR}
    )

  # Qt plugins
  set(QT_IMAGEFORMATS "${WIN_BIN_DIR}/imageformats")
  file(MAKE_DIRECTORY ${QT_IMAGEFORMATS})
  file(
    COPY "${QT_PLUGINS}/imageformats/qjpeg.dll"
    DESTINATION ${QT_IMAGEFORMATS}
    )
  file(
    COPY "${QT_PLUGINS}/imageformats/qgif.dll"
    DESTINATION ${QT_IMAGEFORMATS}
    )
  file(
    COPY "${QT_PLUGINS}/imageformats/qtiff.dll"
    DESTINATION ${QT_IMAGEFORMATS}
    )

  # ICU DLLs
  if (ICU_BIN_DIR)
    file(
      COPY "${ICU_BIN_DIR}/icuin53.dll"
      DESTINATION ${WIN_BIN_DIR}
      )
    file(
      COPY "${ICU_BIN_DIR}/icuuc53.dll"
      DESTINATION ${WIN_BIN_DIR}
      )
    file(
      COPY "${ICU_BIN_DIR}/icudt53.dll"
      DESTINATION ${WIN_BIN_DIR}
      )
  endif()

  # OpenCV DLLs
  set(OpenCV_BIN_DIR "${_OpenCV_LIB_PATH}")
  file(
    COPY "${OpenCV_BIN_DIR}/opencv_core2411.dll"
    DESTINATION ${WIN_BIN_DIR}
    )
  set(OpenCV_BIN_DIR "${_OpenCV_LIB_PATH}")
  file(
    COPY "${OpenCV_BIN_DIR}/opencv_highgui2411.dll"
    DESTINATION ${WIN_BIN_DIR}
    )
  set(OpenCV_BIN_DIR "${_OpenCV_LIB_PATH}")
  file(
    COPY "${OpenCV_BIN_DIR}/opencv_imgproc2411.dll"
    DESTINATION ${WIN_BIN_DIR}
    )
  set(OpenCV_BIN_DIR "${_OpenCV_LIB_PATH}")
  file(
    COPY "${OpenCV_BIN_DIR}/opencv_objdetect2411.dll"
    DESTINATION ${WIN_BIN_DIR}
    )
endif()