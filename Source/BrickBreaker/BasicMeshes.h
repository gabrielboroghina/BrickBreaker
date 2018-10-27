#pragma once
#include "Core/GPU/Mesh.h"

Mesh *CreateMeshWithVertices(const char *name, const std::vector<VertexFormat> &vertices,
                             const std::vector<unsigned short> &indices);

Mesh *CreateCircle(float radius);

Mesh *CreateRect(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill);

Mesh *CreateHeart();
