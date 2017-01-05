#pragma once
#include "GL/glew.h"
#include <glm/glm.hpp>

#include <vector>

struct vertex {
    glm::vec3 position;
};

enum class MeshType {
    Triangles = GL_TRIANGLES,
    LineStrip = GL_LINE_STRIP,
};

class Mesh {
    public:
         Mesh() { glGenVertexArrays(1, &_vertex_array); }
        ~Mesh() { glDeleteVertexArrays(1, &_vertex_array); }

         Mesh(const std::vector<vertex> &vertices, const std::vector<GLuint> &indices, MeshType = MeshType::Triangles);
        void draw();
   private:
        std::vector<vertex> _vertices;
        std::vector<GLuint> _indices;
        GLuint _vertex_array;
        GLuint _vertex_buffer;
        GLuint _element_buffer;
        MeshType _type;
};