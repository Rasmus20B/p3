set(SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/main.cc
)

add_library(engine)
target_sources(engine
  PUBLIC
  FILE_SET
  CXX_MODULES FILES
  components.cc
  p3.cc 
  types.cc
)

target_link_libraries(engine ecstasy raylib jcast)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
