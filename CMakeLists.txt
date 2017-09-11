CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
include(RezBuild)


if(UNIX)
add_compile_options(-std=c++11)
#add_definitions(-D_LINUX -DLINUX -DNO_RTM=TRUE)
SET(CMAKE_CXX_FLAGS "-ggdb -O3 -fPIC -std=c++0x -march=native -mno-avx -DOPENVDB_3_ABI_COMPATIBLE -g")
endif()


rez_find_packages(PREFIX pkgs AUTO)

include_directories( sources )

ADD_HOUDINI_PLUGIN (vdb_deformer "src/vdb_deformer.C" "src/vdb_deformer.h")

TARGET_LINK_LIBRARIES ( vdb_deformer ${pkgs_LIBRARIES} )
INSTALL ( TARGETS vdb_deformer DESTINATION dso )
