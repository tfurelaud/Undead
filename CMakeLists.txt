cmake_minimum_required(VERSION 2.6)

project(undead)

enable_testing()

include(CTest)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMG REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} ${SDL2_IMG_LIBRARIES} ${SDL2_TTF_LIBRARIES})
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMG_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

include_directories(${SDL2_ALL_INC})

add_library(game game.c game_io.c)
add_library(toolbox toolbox.c game.c game_io.c game_alea.c model.c)

set_target_properties(game PROPERTIES LINKER_LANGUAGE C)

add_executable(undead_text undead_text.c)
add_executable(undead_solve undead_solve.c)
add_executable(undead_sdl undead_sdl.c)
add_executable(undead_alea undead_alea.c)

target_link_libraries(undead_text game toolbox)
target_link_libraries(undead_solve game toolbox)
target_link_libraries(undead_alea game toolbox)
target_link_libraries(undead_sdl game toolbox ${SDL2_ALL_LIBS} m)

add_executable(tests test.c)

target_link_libraries(tests game toolbox ${SDL2_ALL_LIBS})

option(ENABLE_DEBUG "debug options" ON)
if(ENABLE_DEBUG)
set(CMAKE_C_FLAGS "-std=c99 -g -fprofile-arcs -ftest-coverage -Wall")
set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
else()
set(CMAKE_C_FLAGS "-std=c99 -O3 -Wall")
endif()

add_test(test1 tests)

option(ENABLE_DEBUG "debug options" ON)
if(ENABLE_DEBUG)
  set(CMAKE_C_FLAGS "-std=c99 -g -fprofile-arcs -ftest-coverage -Wall")
  set(CMAKE_LD_FLAGS "-fprofile-arcs -ftest-coverage")
else()
  set(CMAKE_C_FLAGS "-std=c99 -O3 -Wall")
endif()
