g++ -std=c++17 -I./include \
main.cpp \
clip_plane.cpp \
shapes/plane.cpp \
model.cpp \
texture.cpp \
texture_picking.cpp \
cam.cpp \
extrude_point.cpp \
shell_texture.cpp \
shapes/point.cpp \
shapes/sphere.cpp \
shapes/cube.cpp \
shapes/line.cpp \
shaders/shader.cpp \
include/imgui/ImGuizmo.cpp \
include/imgui/imgui.cpp \
include/imgui/imgui_draw.cpp \
include/imgui/imgui_tables.cpp \
include/imgui/imgui_widgets.cpp \
include/imgui/imgui_impl_glfw.cpp \
include/imgui/imgui_impl_opengl3.cpp \
vertex_array.cpp -o ./main -lGLEW -lglfw -lassimp -lGL -ldart-external-imgui
./main