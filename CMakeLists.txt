CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
cmake_policy(SET CMP0015 NEW)

include( RezBuild OPTIONAL RESULT_VARIABLE REZ_FOUND )

if( REZ_FOUND STREQUAL "NOTFOUND" )
	set( REZ_FOUND 0 )
	message( "REZ not found" )
else()
	set( REZ_FOUND 1 )
	message( "REZ found" )
endif()


set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DOPENVDB_3_ABI_COMPATIBLE" )


if( REZ_FOUND )

	rez_find_packages( PREFIX pkgs AUTO )

	ADD_HOUDINI_PLUGIN ( vdb_deformer "src/vdb_deformer.C" "src/vdb_deformer.h" )

	target_link_libraries ( vdb_deformer ${pkgs_LIBRARIES} )

	install ( TARGETS vdb_deformer DESTINATION dso )

else()

	if( NOT DEFINED ENV{HFS} )
		message( FATAL_ERROR "$HFS environment variable was not found, make sure you have initialized your houdini environment by sourcing /opt/hfsXX.X.XXX/houdini_setup" )
	endif()

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DVERSION=\"16.0.633\" -D_GNU_SOURCE -DLINUX -DAMD64 -m64 -fPIC -DSIZEOF_VOID_P=8 -DFBX_ENABLED=1 -DOPENCL_ENABLED=1 -DOPENVDB_ENABLED=1 -DSESI_LITTLE_ENDIAN -DENABLE_THREADS -DUSE_PTHREADS -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DGCC4 -DGCC3 -Wno-deprecated -Wall -W -Wno-parentheses -Wno-sign-compare -Wno-reorder -Wno-uninitialized -Wunused -Wno-unused-parameter -Wno-unused-local-typedefs -O2 -fno-strict-aliasing -DMAKING_DSO -Wno-unused-variable" )

	set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lpthread -lHoudiniUI -lHoudiniOPZ -lHoudiniOP3 -lHoudiniOP2 -lHoudiniOP1 -lHoudiniSIM -lHoudiniGEO -lHoudiniPRM -lHoudiniUT -lboost_system -L/usr/X11R6/lib64 -L/usr/X11R6/lib -lGLU -lGL -lX11 -lXext -lXi -ldl" )

	include_directories( $ENV{HT}/include )
	link_directories( $ENV{HDSO} )
	set( LINK_LIBS tbb Half openvdb_sesi )

	add_library( vdb_deformer SHARED "src/vdb_deformer.C" "src/vdb_deformer.h" )
	set_target_properties( vdb_deformer PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE )
	set_target_properties( vdb_deformer PROPERTIES PREFIX "" )
	target_link_libraries( vdb_deformer ${LINK_LIBS} )
	install( TARGETS vdb_deformer DESTINATION ${CMAKE_SOURCE_DIR}/dso )

endif()



