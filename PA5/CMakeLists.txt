PROJECT(Tutorial)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLM REQUIRED)
FIND_PACKAGE(assimp REQUIRED)
SET(CXX11_FLAGS -std=gnu++11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
SET(TARGET_LIBRARIES "${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${ASSIMP_LIBRARIES}")

IF(UNIX)
  ADD_DEFINITIONS(-DUNIX)
ENDIF(UNIX)

IF(NOT APPLE)
  IF(assimp_FOUND)
      INCLUDE_DIRECTORIES(${assimp_INCLUDE_DIRS})
      LINK_LIBRARIES(${assimp_LIBRARIES})
  ENDIF(assimp_FOUND)
  IF(GLEW_FOUND)
      INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIRS})
      LINK_LIBRARIES(${GLEW_LIBRARIES})
  ENDIF(GLEW_FOUND)
ENDIF(NOT APPLE)

INCLUDE_DIRECTORIES(
  "${PROJECT_SOURCE_DIR}/include"
  ${SDL2_INCLUDE_DIR}
  ${GLM_INCLUDE_DIRS}
  ${assimp_INCLUDE_DIRS}
)


# Set Includes
SET(INCLUDES ${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${INCLUDES})

# Set sources
FILE(GLOB_RECURSE SOURCES "src/*.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES})

add_custom_target("${PROJECT_NAME}_SUCCESSFUL" ALL
                  DEPENDS ${PROJECT_NAME}
                  COMMAND ${CMAKE_COMMAND} -E echo ""
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                 )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${ASSIMP_LIBRARY})
