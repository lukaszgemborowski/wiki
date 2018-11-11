cmake_minimum_required (VERSION 3.2)
project (wiki)
include(ExternalProject)

ExternalProject_Add(
  toolbox_git

  PREFIX ${CMAKE_BINARY_DIR}
  BUILD_IN_SOURCE 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}

  GIT_REPOSITORY "https://github.com/lukaszgemborowski/cpptoolbox.git"
  GIT_TAG "master"
)

ExternalProject_Add(
  fmt_git

  PREFIX ${CMAKE_BINARY_DIR}
  BUILD_IN_SOURCE 1
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}

  GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
  GIT_TAG "5.2.1"

  CMAKE_ARGS -DFMT_TEST=OFF
)

add_executable(
    wiki
    src/main.cpp
    src/show_wiki_page.cpp
)

target_link_libraries(
    wiki
    curl
    ${CMAKE_BINARY_DIR}/lib/libfmt.a
)

add_dependencies(wiki toolbox_git fmt_git)

target_include_directories(
    wiki PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/include
)