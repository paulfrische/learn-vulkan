CFLAGS = -std=c++17 -O2
LDFLAGS = `pkg-config --cflags --libs vulkan glfw3`

VulkanTest: src/main.cpp shaders/shader.vert shaders/shader.frag
	glslc shaders/shader.vert -o shaders/vert.spv
	glslc shaders/shader.frag -o shaders/frag.spv
	g++ $(CFLAGS) -o VulkanTest src/main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
