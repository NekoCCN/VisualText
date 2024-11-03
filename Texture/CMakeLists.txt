
set(Texture_SRC
  "Texture.cc"
  "Texture.h"
  "Font/Font.h"	
  "Font/Font.cc"
)

add_library(VisualTextTexture STATIC ${Texture_SRC})


set(Texture_Require
  VisualTextCore
  VisualTextAsset
  SDL_image
  SDL3_ttf::SDL3_ttf
)

target_link_libraries(VisualTextTexture PUBLIC ${Texture_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()