#include "BasicMeshes.h"

#include "Core/GPU/Mesh.h"
#include <GL/glew.h>
#include "include/gl.h"

Mesh *CreateMeshWithVertices(const char *name, const std::vector<VertexFormat> &vertices,
                             const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
	                      (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh *mesh = new Mesh(name);
	mesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return mesh;
}

Mesh *CreateCircle(float radius, glm::vec3 color, glm::vec3 marginColor)
{
	// Create circle
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	// Insert center
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(radius, 0, 0), marginColor));

	for (int u = 1; u <= 360; u++) {
		vertices.push_back(VertexFormat(glm::vec3(radius * cos(u), radius * sin(u), 0), marginColor));
		indices.push_back(0);
		indices.push_back(u);
		indices.push_back(u + 1);
	}

	// Create a new mesh from the buffer data
	return CreateMeshWithVertices("circle", vertices, indices);
}

Mesh *CreateHeart()
{
	std::vector<VertexFormat> vertices;
	glm::vec3 magenta = glm::vec3(0.93, 0.05, 0.33);
	glm::vec3 gray = glm::vec3(0.64f, 0.23f, 0.36f); // gray
	float radius = 3;

	// bottom triangle
	vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), gray));
	vertices.push_back(VertexFormat(glm::vec3(6, 9, 0), magenta));
	vertices.push_back(VertexFormat(glm::vec3(-6, 9, 0), magenta));
	std::vector<unsigned short> indices = {0, 1, 2};

	for (int x = -3; x <= 3; x += 6) {
		unsigned short centerIndex = (unsigned short)vertices.size();
		vertices.push_back(VertexFormat(glm::vec3(x, 9, 0), magenta)); // center
		vertices.push_back(VertexFormat(glm::vec3(x + radius, 9, 0), magenta));

		for (int u = 1; u <= 180; u++) {
			vertices.push_back(VertexFormat(glm::vec3(x + radius * cos(u), 9 + radius * sin(u), 0), magenta));
			indices.push_back(centerIndex);
			indices.push_back(centerIndex + u);
			indices.push_back(centerIndex + u + 1);
		}
	}

	// Create a new mesh from the buffer data
	return CreateMeshWithVertices("circle", vertices, indices);
}

Mesh *CreateRect(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill)
{
	height /= 2;
	width /= 2;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(center + glm::vec3(-width, -height, 0), color),
		VertexFormat(center + glm::vec3(width, -height, 0), color),
		VertexFormat(center + glm::vec3(width, height, 0), color),
		VertexFormat(center + glm::vec3(-width, height, 0), color)
	};

	Mesh *rect = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 2, 3};

	if (!fill) {
		rect->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rect->InitFromData(vertices, indices);
	return rect;
}
