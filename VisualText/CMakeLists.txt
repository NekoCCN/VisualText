cmake_minimum_required (VERSION 3.8)

set (VisualTextRequire
  SDL3-static
  imgui
  SDL_image
  SDL3_ttf-static
  VisualTextCore
  VisualTextAsset
  VisualTextTexture
)

add_executable (VisualText "VisualText.cpp" "VisualText.h")
target_link_libraries(VisualText ${VisualTextRequire})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualText PROPERTY CXX_STANDARD 20)
endif()

