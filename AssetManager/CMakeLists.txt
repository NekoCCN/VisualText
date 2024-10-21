
set(ASSET_SRC
  "DataStream/DataStream.cc"
  "DataStream/DataStream.h"
)

add_library(VisualTextAsset STATIC ${Asset_SRC})


set(Asset_Require
  SDL3-static
)

target_link_libraries(VisualTextCore PUBLIC ${Asset_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()