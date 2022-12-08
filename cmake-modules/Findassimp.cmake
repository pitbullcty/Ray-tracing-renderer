
set(ASSIMP_ROOT_DIR "D:/assimp-master/build")
# Find path of each library
find_path(ASSIMP_INCLUDE_DIR
	NAMES
		assimp/anim.h
	HINTS
		${ASSIMP_ROOT_DIR}/../include
)

set(ASSIMP_MSVC_VERSION "vc143")

find_library(ASSIMP_LIBRARY_RELEASE				assimp-${ASSIMP_MSVC_VERSION}-mt.lib			PATHS ${ASSIMP_ROOT_DIR}/lib/Release)
find_library(ASSIMP_LIBRARY_DEBUG				assimp-${ASSIMP_MSVC_VERSION}-mtd.lib			PATHS ${ASSIMP_ROOT_DIR}/lib/Debug)
		

		
	
