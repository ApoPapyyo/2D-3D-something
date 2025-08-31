cmake_minimum_required(VERSION 3.10)
project(3dg)

# C++17を使用
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2の設定

if(APPLE)
    message(STATUS "Building on macOS")
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    # macOS専用の設定を書く
    include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
        /opt/homebrew/opt/sdl2_image/include
    )
elseif(UNIX)
    message(STATUS "Building on Linux")
    # Linux専用の設定を書く
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)

    include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${SDL2_INCLUDE_DIRS}
    )
endif()


# srcディレクトリのソースファイルを集める
file(GLOB LIB_SOURCES src/*.cpp)
list(REMOVE_ITEM LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)


# ライブラリとしてビルド
target_link_libraries(lib3dg PUBLIC
    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
)


add_executable(3dg src/main.cpp)
target_link_libraries(3dg PRIVATE lib3dg)

# テスト用バイナリ
file(GLOB_RECURSE TEST_FILES tests/*.cpp)

enable_testing()  # CTestを有効化

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)
    # ターゲット名にプレフィックスを追加
    set(target_name ${test_name})

    add_executable(${target_name} ${test_file})
    target_link_libraries(${target_name} PRIVATE lib3dg)

    # CTest にテストとして登録
    add_test(NAME ${target_name} COMMAND ${target_name})
endforeach()
