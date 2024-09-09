:: run with mingw32 from msys64 ::::::::::::::::::::::::::::::::::::::::::
C:\msys64\mingw32\bin\g++ -std=c++17 -w -L./lib -I./include ^
main.cpp ^
clip_plane.cpp ^
shapes/plane.cpp ^
model.cpp ^
texture.cpp ^
texture_picking.cpp ^
cam.cpp ^
extrude_point.cpp ^
shell_texture.cpp ^
shapes/point.cpp ^
shapes/sphere.cpp ^
shapes/cube.cpp ^
shapes/line.cpp ^
shaders/shader.cpp ^
vertex_array.cpp -o ./build/main.exe -lglext -lglfw3dll -lassimp
.\build\main
