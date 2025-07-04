cmake_minimum_required(VERSION 3.28)

project(learn_redis)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

find_package(nlohmann_json 3.12.0 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)

add_library(redis_client INTERFACE)
target_include_directories(redis_client INTERFACE ${HIREDIS_INCLUDE_DIRS})
target_link_libraries(redis_client INTERFACE ${HIREDIS_LIBRARIES})
target_link_libraries(redis_client INTERFACE nlohmann_json::nlohmann_json)

function(add_redis_executable TARGET SOURCE)
    add_executable(${TARGET} ${SOURCE})
    target_link_libraries(${TARGET} PRIVATE redis_client)
endfunction()


add_redis_executable(test_connect test/test1.cpp)
add_redis_executable(redis_string test/redis-string.cpp)
add_redis_executable(redis_json test/redis-json.cpp)


