CC  = clang
CXX = clang++

BUILD_TYPE=Debug
DEBUGGER=OFF
USE_ANGLE_FROM_CHROMIUM=OFF
USE_STDCXX=OFF
USE_MIMALLOC=OFF

generate:
	CC=${CC} CXX=${CXX} cmake -B build \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DUSE_ANGLE_FROM_CHROMIUM=${USE_ANGLE_FROM_CHROMIUM} \
		-DUSE_STDCXX=${USE_STDCXX} \
		-DUSE_MIMALLOC=${USE_MIMALLOC} \
		-DDEBUGGER=${DEBUGGER} \
		-GNinja 

build:
	cmake --build ./build --parallel

clean:
	rm -rf build

.PHONY: generate build clean
