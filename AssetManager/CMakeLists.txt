
set(Asset_SRC
  "BinaryPackGenerate/BinaryPackGenerate.h"
  "BinaryPackGenerate/BinaryPackGenerate.cc"
  "BinaryPackGenerate/ThemeTemplateGenerate/ThemeTemplateGenerate.h"
  "BinaryPackGenerate/ThemeTemplateGenerate/ThemeTemplateGenerate.cc"
  "BinaryPack/BinaryPack.h"
  "BinaryPack/BinaryPack.cc"
  "ThemeTemplate/ThemeTemplate.h"
  "AssetPackStream/AssetPackStream.h" 
  "AssetPackStream/AssetPackStream.cc"
  "AssetPack/AssetPack.h" 
  "AssetPack/AssetPack.cc"
  "AssetFormat/AssetFormat.h"
  "MemoryBuffer/MemoryBuffer.h" "EntryPoint.h" "EntryPoint.cc")

add_library(VisualTextAsset STATIC ${Asset_SRC})


set(Asset_Require
  SDL3-static
  VisualTextCore
)

target_link_libraries(VisualTextAsset PUBLIC ${Asset_Require})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VisualTextCore PROPERTY CXX_STANDARD 20)
endif()