
set(Render_SRC
  "RenderFunction/RenderFunction.h" "RenderFunction/RenderFunction.cc")

add_library(VisualTextRender STATIC ${Render_SRC})


set(Render_Require
  SDL3-static
  VisualTextCore
  VisualTextAsset
  VisualTextTexture
)

target_link_libraries(VisualTextRender PUBLIC ${Render_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()