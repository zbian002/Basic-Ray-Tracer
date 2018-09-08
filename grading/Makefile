CXX = g++
CXXFLAGS = -Wall -g -O3 -std=c++11
LIBS = -lpng
GLLIBS = -lGL -lGLU -lglut -lX11 -lpng
SRCS = src/boolean.cpp src/camera.cpp src/dump_png.cpp src/flat_shader.cpp src/parse.cpp src/phong_shader.cpp src/plane.cpp src/bounded_plane.cpp src/reflective_shader.cpp src/refractive_shader.cpp src/render_world.cpp src/sphere.cpp
SRCS = src/boolean.cpp src/camera.cpp src/dump_png.cpp src/flat_shader.cpp src/parse.cpp src/phong_shader.cpp src/plane.cpp src/bounded_plane.cpp src/reflective_shader.cpp src/refractive_shader.cpp src/render_world.cpp src/sphere.cpp
OBJS := ${SRCS:.cpp=.o} 
OBJS_NOGL := src/main.o
OBJS_GL := src/main-gl.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LIBS = -framework OpenGL -framework GLUT -lpng
	CXXFLAGS += -Wno-deprecated-declarations
endif

TARGET = ray_tracer 

.PHONY: all clean

all: $(TARGET) ray_tracer_preview

$(TARGET): $(OBJS) ${OBJS_NOGL}
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(OBJS_NOGL) $(LIBS)
ray_tracer_preview: $(OBJS) ${OBJS_GL}
	$(CXX) $(CXXFLAGS) -o ray_tracer_preview $(OBJS) $(OBJS_GL) $(LIBS) ${GLLIBS}

$(OBJS) $(OBJS_NOGL): %.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -DNO_OPENGL -o $@ $<
$(OBJS_GL): %-gl.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o src/*.d src/*~ src/*.bak $(TARGET) ray_tracer_preview
	rm -rf grading


# At the end of the makefile
# Include the list of dependancies generated for each object file
# unless make was called with target clean
ifneq "$(MAKECMDGOALS)" "clean"
-include ${SRCS:.cpp=.d}
endif

