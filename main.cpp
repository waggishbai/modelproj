#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> mats;
    std::string err;
    std::string warn;
    tinyobj::LoadObj(&attrib, &shapes, &mats, &err, &warn, "../cube_diorama.obj");

    int w, h, somethingidcabt;
    unsigned char* data = stbi_load("cube_diorama.mtl", &w, &h, &somethingidcabt, 0);

    for (int i = 0; i < sizeof(data) / 2000; i++) {
        std::cout << data[i] << std::endl;
    }

    std::vector<float> vertices;
    std::cout << shapes.size() << std::endl;
    for (int i = 0; i < shapes.size(); i++) {
        tinyobj::mesh_t mesh = shapes[i].mesh;
        for (int i = 0; i < mesh.indices.size(); i++ ) {
            //std::cout << attrib.vertices[i] << std::endl;
            int ind = mesh.indices[i].vertex_index;

            vertices.push_back(attrib.vertices[3 * ind + 0]);
            vertices.push_back(attrib.vertices[3 * ind + 1]);
            vertices.push_back(attrib.vertices[3 * ind + 2]);
        }
    }


    glfwInit();

    GLFWwindow* window = glfwCreateWindow(600, 600, "t", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGL();

    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0, 0, 0, 1);


    GLuint compileProgram(GLuint v, GLuint f);
    GLuint compileShader(const char* t, GLenum ty);
    std::string getFileText(std::string s);

    void mat4ToProgram(GLuint p, glm::mat4 mat, const char* c);

    GLuint program = compileProgram(compileShader(getFileText("../vertex_shader.glsl").c_str(), GL_VERTEX_SHADER),
                                    compileShader(getFileText("../fragment_shader.glsl").c_str(), GL_FRAGMENT_SHADER));

    glUseProgram(program);

    glm::mat4 proj = glm::perspective(glm::radians(45.f), 1.f, .1f, 100.f);
    glm::mat4 cam = glm::lookAt(glm::vec3(8, 8, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 model = glm::mat4(1);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    while(!glfwWindowShouldClose(window)) {
            model = glm::rotate(model, glm::radians(1.f), glm::vec3(0, 1, 0));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mat4ToProgram(program, proj, "proj");
            mat4ToProgram(program, cam, "view");
            mat4ToProgram(program, model, "model");
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());


            glfwPollEvents();
            glfwSwapBuffers(window);
    }
    return 0;
}

std::string getFileText(std::string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(const char* shaderText, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderText, nullptr);
    glCompileShader(shader);
    return shader;
}

GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}

void mat4ToProgram(GLuint program, glm::mat4 mat, const char* varName) {
    GLint uVar = glGetUniformLocation(program, varName);
    glUniformMatrix4fv(uVar, 1, GL_FALSE, glm::value_ptr(mat));
}
