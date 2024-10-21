# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VisualText")

# Visual Text SRC
include_directories( ./ )
add_subdirectory ("VisualText")
add_subdirectory ("Core")

# SDL
set (BUILD_SHARED_LIBS false)
set(SDL_STATIC on)
add_subdirectory( External/SDL )
include_directories( External/SDL/include )

# SDL Extension
add_subdirectory( External/SDL_image_pre )
include_directories( External/SDL_image_pre/SDL_image/include )
add_subdirectory( External/Truetype_loader/SDL_ttf )
include_directories( External/Truetype_loader/SDL_ttf/include )

# ImgUI
add_subdirectory( External/imgui_pre )
include_directories( External/imgui_pre )