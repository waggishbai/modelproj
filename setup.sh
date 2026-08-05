git clone https://github.com/glfw/glfw
git clone https://github.com/g-truc/glm
mkdir objloader
cd objloader
git clone https://github.com/tinyobjloader/tinyobjloader
cp tinyobjloader/tiny_obj_loader.h ./
rm -r tinyobjloader/*
rmdir tinyobjloader
