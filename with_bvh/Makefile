CXX = g++
CXXFLAGS = -std=c++11 -O3 -fopenmp

SRC = main.cpp vec3.cpp aabb.cpp material.cpp hittable.cpp camera.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = ray_tracer

all: with_bvh

with_bvh: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)
	@echo "Running ray tracer..."
	./$(TARGET) > image1.ppm
	@echo "Output saved to image1.ppm"

clean:
	rm -f $(OBJ) $(TARGET) image1.ppm
	@echo "Cleaned up build files."
