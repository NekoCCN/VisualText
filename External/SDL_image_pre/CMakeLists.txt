add_library(SDL_image STATIC
  SDL_image/src/IMG.c
  SDL_image/src/IMG_WIC.c
  SDL_image/src/IMG_avif.c
  SDL_image/src/IMG_bmp.c
  SDL_image/src/IMG_gif.c
  SDL_image/src/IMG_jpg.c
  SDL_image/src/IMG_jxl.c
  SDL_image/src/IMG_lbm.c
  SDL_image/src/IMG_pcx.c
  SDL_image/src/IMG_png.c
  SDL_image/src/IMG_pnm.c
  SDL_image/src/IMG_qoi.c
  SDL_image/src/IMG_stb.c
  SDL_image/src/IMG_svg.c
  SDL_image/src/IMG_tga.c
  SDL_image/src/IMG_tif.c
  SDL_image/src/IMG_webp.c
  SDL_image/src/IMG_xcf.c
  SDL_image/src/IMG_xpm.c
  SDL_image/src/IMG_xv.c
)

target_include_directories(SDL_image PUBLIC SDL_image/include)
target_compile_definitions(SDL_image PRIVATE
  USE_STBIMAGE
  LOAD_PNG
  LOAD_JPG
)