CXX = g++
CXXFLAGS = -std=c++11 -O3 -fopenmp -Iinclude

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(SRC_DIR)/aabb.cpp \
       $(SRC_DIR)/camera.cpp \
       $(SRC_DIR)/hittable.cpp \
       $(SRC_DIR)/material.cpp \
       $(SRC_DIR)/vec3.cpp \
       main.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = ray_tracer

all: with_bvh

with_bvh: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)
	@echo "Running ray tracer..."
	./$(TARGET) > image1.ppm
	@echo "Output saved to image1.ppm"

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm -f $(SRC_DIR)/*.o main.o $(TARGET) image1.ppm
	@echo "Cleaned up build files."
