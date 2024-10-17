
set(Core_SRC
  "LogSystem/LogSystem.cc"
  "LogSystem/LogSystem.h"
  "Exception/Exception.cc"
  "Exception/Exception.h"
  "Window/Window.h"
  "Window/Window.cc")

add_library(VisualTextCore STATIC ${Core_SRC})


set(Core_Require
  SDL3-static
)

target_link_libraries(VisualTextCore PUBLIC ${Core_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()