#include "Shader.h"


Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	//1.Retrieve the vertex/fragment source code from the file
	string VertexCode, FragmentCode;
	ifstream VShaderFile, FShaderFile;
	//ensure ifstream objects can throw exceptions
	VShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	FShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{

		//open the files
		VShaderFile.open(VertexPath);
		FShaderFile.open(FragmentPath);
		stringstream VShaderStream, FShaderStream;
		
		//read the file's buffer contents into the streams

		VShaderStream << VShaderFile.rdbuf();
		FShaderStream << FShaderFile.rdbuf();

		//close the file handlers
		VShaderFile.close();
		FShaderFile.close();
		//convert stream into string
		VertexCode = VShaderStream.str();
		FragmentCode = FShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const char* VShaderCode = VertexCode.c_str();
	const char* FShaderCode = FragmentCode.c_str();

	//2. Compile the Shaders
	unsigned int VertexShader, FragmentShader;

	//The Vertex Shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VShaderCode, NULL);
	glCompileShader(VertexShader);

	//print any errors
	CheckCompilerErrors(VertexShader, "VERTEX");

	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FShaderCode, NULL);
	glCompileShader(FragmentShader);

	//print any errors
	CheckCompilerErrors(FragmentShader, "FRAGMENT");

	//Shader Program
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShader);
	glAttachShader(ProgramID, FragmentShader);
	glLinkProgram(ProgramID);

	//print any errors that occur
	CheckCompilerErrors(ProgramID, "PROGRAM");

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::Delete()
{
	glDeleteProgram(ProgramID);
}

void Shader::SetBool(const string& Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), (int)Value);
}

void Shader::SetInt(const string& Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()),Value);
}

void Shader::SetFloat(const string& Name, float Value) const
{
	glUniform1f(glGetUniformLocation(ProgramID, Name.c_str()), Value);
}

void Shader::SetVec2(const string& Name, glm::vec2& Value) const
{
	glUniform2fv(glGetUniformLocation(ProgramID, Name.c_str()),1,&Value[0]);
}

void Shader::SetVec2(const string& Name, float X, float Y) const
{
	glUniform2f(glGetUniformLocation(ProgramID, Name.c_str()), X, Y);
}

void Shader::SetVec3(const string& Name, const glm::vec3& Value) const
{
	glUniform3fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, &Value[0]);
}

void Shader::SetVec3(const string& Name, float X, float Y, float Z) const
{
	glUniform3f(glGetUniformLocation(ProgramID, Name.c_str()), X, Y, Z);
}

void Shader::SetVec4(const string& Name, const glm::vec4& Value) const
{
	glUniform4fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, &Value[0]);
}

void Shader::SetVec4(const string& Name, float X, float Y, float Z, float W) const
{
	glUniform4f(glGetUniformLocation(ProgramID, Name.c_str()), X, Y, Z, W);
}

void Shader::SetMat2(const string& Name, const glm::mat2& Mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void Shader::SetMat3(const string& Name, const glm::mat3& Mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void Shader::SetMat4(const string& Name, const glm::mat4& Mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void Shader::CheckCompilerErrors(GLuint Shader, string Type)
{
	GLint Success;
	GLchar InfoLog[1024];
	if (Type != "PROGRAM")
	{
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(Shader, 1024, NULL, InfoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type " << Type << "\n" << InfoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(Shader, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(Shader, 1024, NULL, InfoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type " << Type << "\n" << InfoLog << endl;
		}
	}
}
