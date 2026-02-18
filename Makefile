SRC_FILES = src/fleece.cpp src/debug.cpp src/test.cpp

default: $(SRC_FILES)
	clang++ -o build/fleece $(SRC_FILES)

test: default
	build/fleece
