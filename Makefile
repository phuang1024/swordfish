.PHONY: build debug release

BUILD_TYPE ?= Release
BUILD_SYSTEM ?= Unix Makefiles

release:
	make build BUILD_TYPE=Release

debug:
	make build BUILD_TYPE=Debug

build:
	mkdir -p ./build
	cd ./build; \
	cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -G="$(BUILD_SYSTEM)" ../src; \
	cmake --build .
