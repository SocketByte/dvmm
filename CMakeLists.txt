cmake_minimum_required(VERSION 4.2)
project(dvmm C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED ayatana-appindicator3-0.1)

add_executable(${PROJECT_NAME} main.c process.c)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${GTK_INCLUDE_DIRS}
        ${APPINDICATOR_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        ${GTK_LIBRARIES}
        ${APPINDICATOR_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
        ${GTK_CFLAGS_OTHER}
        ${APPINDICATOR_CFLAGS_OTHER}
)