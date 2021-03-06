#include "mesh.hpp"

// based on https://github.com/Polytonic/Glitter/blob/master/Samples/mesh.cpp

Mesh::Mesh(const std::vector<vertex> &vertices,
        const std::vector<GLuint> &indices,
        DrawType drawType, MeshType meshType)
    : _vertices(vertices), _indices(indices), _drawType(drawType), _meshType(meshType) {
    // Bind a Vertex Array Object
    glGenVertexArrays(1, &_vertex_array);
    glBindVertexArray(_vertex_array);

    // Copy Vertex Buffer Data
    glGenBuffers(1, &_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            _vertices.size() * sizeof(vertex),
            &_vertices.front(), static_cast<GLuint>(meshType));

    // Copy Index Buffer Data
    glGenBuffers(1, & _element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            _indices.size() * sizeof(GLuint),
            &_indices.front(), GL_STATIC_DRAW);

    // Set Shader Attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *) offsetof(vertex, position));
    glEnableVertexAttribArray(0); // Vertex Positions

    // Cleanup Buffers
    // Apparently deleting these buffers is okay because they are still referenced by the VAO
    // and so will only get deleted when the VAO is deleted
    // --> One call to delete _vertex_array should be sufficient to free all the memory.
    glBindVertexArray(0);
    if(_meshType == MeshType::Static) {
        glDeleteBuffers(1, & _vertex_buffer);
        glDeleteBuffers(1, & _element_buffer);
    }
}

void Mesh::update(std::vector<vertex> &newVertices) {
    std::swap(newVertices, _vertices);
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            _vertices.size() * sizeof(vertex),
            nullptr, static_cast<GLuint>(_meshType));
    glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(vertex), &_vertices.front());
}

void Mesh::draw() {
    glBindVertexArray(_vertex_array);
    glDrawElements(static_cast<GLuint>(_drawType), _indices.size(), GL_UNSIGNED_INT, 0);
}
