CFLAGS := -std=c++17
LDFLAGS := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -Isrc/
CPP_FILES := $(shell find ./src/*.cpp)
VERT_FILES := $(shell find ./shaders/*.vert)
FRAG_FILES := $(shell find ./shaders/*.frag)
VERT_OBJ_FILES := $(patsubst %.vert,%.vert.spv, $(VERT_FILES))
FRAG_OBJ_FILES := $(patsubst %.frag,%.frag.spv, $(FRAG_FILES))

.PHONY: all test buildShaders clean build

all: buildShaders test clean

test:
	g++ $(CFLAGS) -o build/main $(CPP_FILES) $(LDFLAGS)
	build/main

buildShaders:
	glslc $(VERT_FILES) -o $(VERT_OBJ_FILES)
	glslc $(FRAG_FILES) -o $(FRAG_OBJ_FILES)

clean:
	rm build/main
	rm $(VERT_OBJ_FILES)
	rm $(FRAG_OBJ_FILES)

# production build
build: buildShaders
	g++ $(CFLAGS) -O3 -o build/main $(CPP_FILES) $(LDFLAGS)