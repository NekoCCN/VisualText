
set(Asset_SRC
  "BinaryPackGen/BinaryPackGen.h"
  "BinaryPackGen/BinaryPackGen.cc"
)

add_library(VisualTextAsset STATIC ${Asset_SRC})


set(Asset_Require
  SDL3-static
  VisualTextCore
)

target_link_libraries(VisualTextAsset PUBLIC ${Asset_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()