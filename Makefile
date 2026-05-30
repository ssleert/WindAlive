CC  = clang
CXX = clang++

BUILD_TYPE=Debug
USE_ANGLE_FROM_CHROMIUM=OFF
USE_STDCXX=OFF

generate:
	CC=${CC} CXX=${CXX} cmake -B build \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DUSE_ANGLE_FROM_CHROMIUM=${USE_ANGLE_FROM_CHROMIUM} \
		-DUSE_STDCXX=${USE_STDCXX} \
		-GNinja 

build:
	cmake --build ./build --parallel

clean:
	rm -rf build

.PHONY: generate build clean
