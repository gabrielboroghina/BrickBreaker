#pragma once
#include "Core/GPU/Mesh.h"
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat.hpp>

Mesh *CreateMeshWithVertices(const char *name, const std::vector<VertexFormat> &vertices,
                             const std::vector<unsigned short> &indices);

Mesh *CreateCircle(float radius, glm::vec3 color, glm::vec3 marginColor);

Mesh *CreateRect(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill);

Mesh *CreateHeart();
