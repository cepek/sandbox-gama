cmake_minimum_required(VERSION 3.5)

project(geng3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (GAMALIB "../../../lib")

include_directories(${GAMALIB})

add_executable(geng3
    main.cpp geng3.h geng3.cpp
    ${GAMALIB}/gnu_gama/ellipsoid.cpp
    ${GAMALIB}/gnu_gama/ellipsoid.h
    ${GAMALIB}/gnu_gama/radian.h
    ${GAMALIB}/gnu_gama/intfloat.h
    ${GAMALIB}/gnu_gama/gon2deg.h
    ${GAMALIB}/gnu_gama/gon2deg.cpp
)
#target_link_libraries(geng3demo ${CMAKE_SOURCE_DIR}/gama/lib/libgama.a)

install(TARGETS geng3
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
