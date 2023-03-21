#ifndef SHADER_H
#define SHADER_H



#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

using namespace std;

class Shader
{

public:
	//Program ID
	unsigned int ProgramID;

	//ShaderConstructor
	Shader(const char* VertexPath, const char* FragmentPath);
	//Activate the Shader Program
	void Use();
	/*Delete the Shader Program*/
	void Delete();
	//Utility Functions
	void SetBool(const string& Name, bool Value) const;
	void SetInt(const string& Name, int Value) const;
	void SetFloat(const string& Name, float Value) const;
	void SetVec2(const string& Name, glm::vec2& Value) const;
	void SetVec2(const string& Name, float X, float Y) const;
	void SetVec3(const string& Name, const glm::vec3& Value) const;
	void SetVec3(const string& Name, float X, float Y, float Z) const;
	void SetVec4(const string& Name, const glm::vec4& Value) const;
	void SetVec4(const string& Name, float X, float Y, float Z, float W) const;
	void SetMat2(const string& Name, const glm::mat2& Mat) const;
	void SetMat3(const string& Name, const glm::mat3& Mat)const;
	void SetMat4(const string& Name, const glm::mat4& Mat) const;
private:
	void CheckCompilerErrors(GLuint Shader, string Type);
};

#endif
