set(SDK_MIN "10.9")
set(SDK "10.9")
set(DEV_SDK "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${SDK}.sdk")

add_definitions(
  -DMAC
  -DGCC_VISIBILITY
  -mmacosx-version-min=${SDK_MIN}
  )

set(CMAKE_OSX_SYSROOT ${DEV_SDK})
