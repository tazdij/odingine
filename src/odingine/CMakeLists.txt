#
#  Author: Shervin Aflatooni
#

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

file(GLOB_RECURSE TARGET_INC ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB_RECURSE TARGET_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

SET(TARGET_SRC ${TARGET_SRC} ${TARGET_INC} ${TARGET_INC_COMPONENTS})

IF (ANDROID)
  add_library (odingine STATIC ${TARGET_SRC})
ELSE ()
  add_library (odingine SHARED ${TARGET_SRC})
ENDIF ()

target_link_libraries (odingine ${ENGINE_LIBS})

# These might be needed
#install (TARGETS odingine DESTINATION lib)
#install (FILES ${TARGET_INC} DESTINATION include/odingine)

# TODO: ADD THIS BACK WHEN NEEDED
# IF (NOT TARGET uninstall)
#   # add make uninstall capability
#   configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
#   add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
# ENDIF()
