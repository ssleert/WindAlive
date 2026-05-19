CC  := clang
CXX := clang++

BUILD_TYPE=Debug

generate:
	CC=${CC} CXX=${CXX} cmake -B build \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DOPENGL_VERSION=4.3 \
		-GNinja 

build:
	cmake --build ./build --parallel

clean:
	rm -rf build

.PHONY: generate build clean
