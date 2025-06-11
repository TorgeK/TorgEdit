# Compiler and flags
CXX = g++
CXXFLAGS = -Iimgui -Iimgui/backends -Wall -std=c++17
LDFLAGS = -lglfw -lGLEW -lGL

# ImGui source files
IMGUI_DIR = imgui
IMGUI_BACKENDS = $(IMGUI_DIR)/backends
IMGUI_SRC = \
	$(IMGUI_DIR)/imgui.cpp \
	$(IMGUI_DIR)/imgui_draw.cpp \
	$(IMGUI_DIR)/imgui_tables.cpp \
	$(IMGUI_DIR)/imgui_widgets.cpp \
	$(IMGUI_DIR)/imgui_demo.cpp \
	$(IMGUI_BACKENDS)/imgui_impl_glfw.cpp \
	$(IMGUI_BACKENDS)/imgui_impl_opengl3.cpp

# Object files
OBJS = main.o $(IMGUI_SRC:.cpp=.o)

# Target
all: image_editor

image_editor: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(IMGUI_DIR)/*.o $(IMGUI_BACKENDS)/*.o image_editor
