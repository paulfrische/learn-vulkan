CFLAGS = -std=c++17 -O2
LDFLAGS = `pkg-config --cflags --libs vulkan glfw3`

VulkanTest: src/main.cpp
	g++ $(CFLAGS) -o VulkanTest src/main.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
