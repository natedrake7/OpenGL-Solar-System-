#ifndef  MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	unsigned int TexID;
	string Type;
	string path;
};


class Mesh
{
public:
	//Mesh Data
	vector<Vertex> Vertices;
	vector<unsigned int> Indices;
	vector<Texture> Textures;
	Mesh(vector<Vertex> Vertices, vector<unsigned int> Indices, vector<Texture> Textures);
	void Draw(Shader& Shader);
private:
	//render data
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};


















#endif // ! MESH_H
