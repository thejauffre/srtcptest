project(SRtcpTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_INCLUDE_DIR include)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
find_package(Threads REQUIRED)

include_directories(${PROJECT_INCLUDE_DIR})

set(SOURCE_LIST
  "sources/main.cpp"
  )

add_executable(${PROJECT_NAME} ${SOURCE_LIST})