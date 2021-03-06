#
#  Author: Shervin Aflatooni
#

cmake_minimum_required(VERSION 2.8)

project (Engine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Root dir")

set(CP_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)

include(${CP_CMAKE_DIR}/macros.cmake)

IF (EMSCRIPTEN)
  set ( BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin-emscripten )
ELSEIF (ANDROID)
  set ( BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin-android )
  include(${CP_CMAKE_DIR}/android.cmake)
ELSE ()
  set ( BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin )
ENDIF ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH         ${BIN_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
LINK_DIRECTORIES(${BIN_DIR}/lib)
set(CMAKE_BINARY_DIR ${BIN_DIR})

add_subdirectory(dependencies)

set ( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL/include )
set ( SDL2_LIBRARY SDL2 )

set ( GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew/include )
set ( GLEW_LIBRARIES libglew_shared )

set ( PHYSFS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/physfs )
set ( PHYSFS_LIBRARY physfs )


IF (EMSCRIPTEN)
  add_definitions( -DGLES2=1 )

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/src/example/assets/ -s TOTAL_MEMORY=38000000 -s USE_SDL=2")
  add_definitions(-DASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/example/assets/")

  #set(ENGINE_LIBS
  #  ${ASSIMP_LIBRARY}
  #)

  set(ENGINE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/odingine/
  )
ELSEIF (ANDROID)
  add_definitions( -DGLES2=1 )
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c11")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

  set(ENGINE_LIBS
    ${SDL2_LIBRARY}
    ${PHYSFS_LIBRARY}
  )

  set(ENGINE_INCLUDES
    ${SDL2_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/odingine/
  )
ELSE ()
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c11")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
  find_package(OpenGL REQUIRED)

  set(ENGINE_LIBS
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    ${PHYSFS_LIBRARY}
    ${GLEW_LIBRARIES}
  )

  set(ENGINE_INCLUDES
    ${SDL2_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/odingine/
  )
ENDIF ()

include_directories(${ENGINE_INCLUDES})

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/src )
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
