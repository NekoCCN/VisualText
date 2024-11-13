
set(SemanticResolution_SRC
  "SemanticTable/SemanticTable.h"
   "SemanticResolution.h" "SemanticResolution.cc")

add_library(VisualTextSemanticResolution STATIC ${SemanticResolution_SRC})


set(SemanticResolution_Require
  SDL3-static
  VisualTextCore
  VisualTextAsset
)

target_link_libraries(VisualTextSemanticResolution PUBLIC ${SemanticResolution_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()