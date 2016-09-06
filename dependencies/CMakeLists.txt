execute_process( COMMAND git submodule update --init --recursive )
set_directory_properties( PROPERTIES CLEAN_NO_CUSTOM 1)

IF (EMSCRIPTEN)
ELSEIF (ANDROID)
  set( SDL_STATIC ON CACHE BOOL "Build the static SDL library" )
  set( SDL_SHARED OFF CACHE BOOL "Build the shared SDL library" )
  # set( SDL_FILESYSTEM FALSE )
  set( PTHREADS OFF CACHE BOOL "Pthread support" )
  add_subdirectory( SDL )

  set( PHYSFS_BUILD_STATIC ON CACHE BOOL "Build the static physfs library" )
  set( PHYSFS_BUILD_SHARED OFF CACHE BOOL "Build the shared physfs library" )
  set( PHYSFS_BUILD_TEST OFF CACHE BOOL "Build the test physfs program" )
  add_subdirectory( physfs )
ELSE ()

  set( SDL_STATIC OFF CACHE BOOL "Build the static SDL library" )
  set( SDL_SHARED ON CACHE BOOL "Build the shared SDL library" )
  add_subdirectory( SDL )

  set( glew-cmake_BUILD_SHARED ON CACHE BOOL "Build the shared glew library" )
  set( glew-cmake_BUILD_STATIC OFF CACHE BOOL "Build the static glew library" )
  set( glew-cmake_BUILD_SINGLE_CONTEXT ON CACHE BOOL "Build the single context glew library" )
  set( glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "Build the multi context glew library" )

  add_subdirectory( glew )


  set( PHYSFS_BUILD_STATIC OFF CACHE BOOL "Build the static physfs library" )
  set( PHYSFS_BUILD_SHARED ON CACHE BOOL "Build the shared physfs library" )
  set( PHYSFS_BUILD_TEST OFF CACHE BOOL "Build the test physfs program" )
  add_subdirectory( physfs )
ENDIF ()
