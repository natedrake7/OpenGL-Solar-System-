
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>
#include "Shader.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

void FrameBuffer_Size_Callback(GLFWwindow* Window, int Width, int Height);
void ProcessInput(GLFWwindow* Window);
void Mouse_CallBack(GLFWwindow* Window, double Xpos, double Ypos);
void Scroll_CallBack(GLFWwindow* Window, double XoffSet, double YoffSet);
unsigned int LoadTexture(const char* Path);

/*Screen Resolution*/ 
const float SCR_LENGTH = 800.0f;
const float SCR_WIDTH = 600.0f;

/*Camera Components*/
Camera camera;
float LastX = 400.0f;
float LastY = 300.0f;
bool FirstMouse = true;

/*Keep track of time*/
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

/*Light Source*/
glm::vec3 LightPos(0.0f, 0.0f, 0.0f);

/*Cube Rotation Speed(Around Planet)*/
float CubeRotationDegrees = 0.0f;
float CubeRotationDegreesTemp = 0.0f;

/*Cubes Rotation Speed around themselves*/
float CubeSelfRotationDegrees = 0.0f;
float CubeSelfRotationDegreesTemp = 0.0f;

/*Planet Rotation Speed Around itself*/
float PlanetRotationDegrees = 0.0f;
float PlanetRotationDegreesTemp = 0.0f;

/*Planet (with the cubes around it) Rotation Speed*/
float Degrees = 0.0f;
float DegreesTemp = 0.0f;

/*Variable to hold the current time*/
float Time = 0.0f;

/*Pause/Unpause*/
bool Pause = false;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* Window = glfwCreateWindow(SCR_LENGTH, SCR_WIDTH, "LearnOpenGL", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to Create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);
	glfwSetFramebufferSizeCallback(Window, FrameBuffer_Size_Callback);
	glfwSetCursorPosCallback(Window, Mouse_CallBack);
	glfwSetScrollCallback(Window, Scroll_CallBack);

	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to Initialize GLAD" << endl;
		return -1;
	}
	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	/*Container Shader*/
	Shader ContainerShader("Shaders/ContainerVertexShader.txt","Shaders/ContainerFragmentShader.txt");
	/*Create a Planet shader*/
	Shader ModelShader("Shaders/ModelVertexShader.txt", "Shaders/ModelFragmentShader.txt");
	/*Create A Light Cube to be placed inside the Planet so it exerts light*/
	Shader LightCubeShader("Shaders/LightCubeVertexShader.txt","Shaders/LightCubeFragmentShader.txt");

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	unsigned int VBO, CubeVAO, lightVAO;

	glGenVertexArrays(1,&CubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*Position Attribute*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*Color Attribute*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int Texture = LoadTexture("Textures/Container.png"); //Load the Container texture
	unsigned int TextureSpecular = LoadTexture("Textures/container2.png"); //Load the Container Specular Map

	Model model("Textures/planet.obj");
	ContainerShader.Use();
	ContainerShader.SetInt("material.Diffuse", 0);
	ContainerShader.SetInt("material.Specular", 1);

	while (!glfwWindowShouldClose(Window))
	{
		//Calcuate Current Frame;
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		/*Check if user wants to use a command or wander around the world*/
		ProcessInput(Window);

		/*Background Color*/
		glClearColor(0.047f, 0.049f, 0.051f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*Use the Shaders*/
		ContainerShader.Use();
		ContainerShader.SetVec3("light.Position", glm::vec3(10 * cos(Degrees), 0.0f, 10 * sin(Degrees)));
		ContainerShader.SetVec3("ViewPos", camera.Position);

		ContainerShader.SetVec3("light.Ambient", 0.2f, 0.2f, 0.2f);
		ContainerShader.SetVec3("light.Diffuse", 0.5f, 0.5f, 0.5f);
		ContainerShader.SetVec3("light.Specular", 1.0f, 1.0f, 1.0f);

		ContainerShader.SetFloat("light.Constant", 1.0f);
		ContainerShader.SetFloat("light.Linear", 0.09f);
		ContainerShader.SetFloat("light.Quadratic", 0.032f);

		/*Bind the Color/Light to the Container Shaders*/
		ContainerShader.SetFloat("material.Shininess", 32.0f);

		/*Zoom Matrix*/
		glm::mat4 Projection = glm::perspective(glm::radians(camera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		/*Declare the View Matrix*/
		glm::mat4 View = camera.GetViewMatrix();
		/*Add it to the main shader*/
		ContainerShader.SetMat4("Projection", Projection);
		ContainerShader.SetMat4("View", View);

		glm::mat4 Model = glm::mat4(1.0f);
		ContainerShader.SetMat4("Model", Model);

		/*Bind Cube Textures and activate the corresponding Textures*/
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureSpecular);

		if (Pause == false)
		{
			Degrees += DegreesTemp + 0.01f;
			CubeRotationDegrees += CubeRotationDegreesTemp + 0.01f;
			CubeSelfRotationDegrees += CubeSelfRotationDegreesTemp + 0.01f;
			PlanetRotationDegrees += PlanetRotationDegreesTemp + 0.01f;
			Time = (float)glfwGetTime();
		}

		glBindVertexArray(CubeVAO);
		/*Standard Cube Positions to form a circle (sort of) around the planet*/
		glm::vec3 cubePositions[] = {
			glm::vec3(5 * cos(0.000f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(0.000f + CubeRotationDegrees * (float)2)),
			glm::vec3(5 * cos(60.00f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(60.00f + CubeRotationDegrees * (float)2)),
			glm::vec3(5 * cos(120.0f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(120.0f + CubeRotationDegrees * (float)2)),
			glm::vec3(5 * cos(180.0f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(180.0f + CubeRotationDegrees * (float)2)),
			glm::vec3(5 * cos(240.0f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(240.0f + CubeRotationDegrees * (float)2)),
			glm::vec3(5 * cos(300.0f + CubeRotationDegrees * (float)2), 0.0f,5 * sin(300.0f + CubeRotationDegrees * (float)2)),
		};

		for (int i = 0; i < 7; i++)
		{
			glm::mat4 Model = glm::mat4(1.0f);
			Model = glm::translate(Model, cubePositions[i]); //Move the Cubes according to their positions
			Model = glm::translate(Model, glm::vec3(10 * cos(Degrees), 0.0f, 10 * sin(Degrees))); //Move the Cubes in a circle
				/*Rotate each cube differently on the 3 basic axes*/
				if (i == 0)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
				else if (i == 1)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(-1.0f, 0.0f, 0.0f));
				else if (i == 2)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
				else if (i == 3)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(0.0f, -1.0f, 0.0f));
				else if (i == 4)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
				else if (i == 5)
					Model = glm::rotate(Model, Time * glm::radians(CubeSelfRotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));
			/*Draw the cubes*/
			ContainerShader.SetMat4("Model", Model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/*Activate the Light Cube Shader and set the Projectio  and view matrices to it*/
		LightCubeShader.Use();
		LightCubeShader.SetMat4("Projection", Projection);
		LightCubeShader.SetMat4("View", View);

		/*Move the Light Source in a circle*/
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(10 * cos(Degrees), 0.0f, 10 * sin(Degrees)));
		LightCubeShader.SetMat4("Model", Model);

		/*Set the Projection and View matrices to the Model Shader*/
		ModelShader.Use();
		ModelShader.SetMat4("Projection", Projection);
		ModelShader.SetMat4("View", View);

		/*Move the planet in a circle and rotate it on the y axis*/
		Model = glm::mat4(1.0f);
		Model = glm::translate(Model, glm::vec3(10 * cos(Degrees), 0.0f, 10 * sin(Degrees)));
		Model = glm::rotate(Model, Time * glm::radians(PlanetRotationDegrees), glm::vec3(0.0f, -1.0f, 0.0f));
		ModelShader.SetMat4("Model", Model);
		model.Draw(ModelShader);

		/*Draw the Light Cube*/
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*Wait for Vsync*/
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
	/*Delete the buffers*/
	glDeleteVertexArrays(1, &CubeVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &lightVAO);

	/*Delete the Shaders*/
	ContainerShader.Delete();
	ModelShader.Delete();
	LightCubeShader.Delete();

	/*Close the Window*/
	glfwTerminate();
	return 0;
}

void FrameBuffer_Size_Callback(GLFWwindow* Window, int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* Window)
{
	float CameraSpeed = 2.5 * DeltaTime;
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //Check if user wants to close the window
		glfwSetWindowShouldClose(Window, true); //close the window

	if (glfwGetKey(Window, GLFW_KEY_SPACE) == GLFW_PRESS) //Check if user wants to pause
	{
		if (Pause == false)
			Pause = true;
		else
			Pause = false;
	}
	/*change the speed of the cubes rotation around the planet*/
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		CubeSelfRotationDegreesTemp -= 0.001f;
	else if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
		CubeRotationDegreesTemp -= 0.001f;

	if(glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		CubeSelfRotationDegreesTemp += 0.001f;
	else if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		CubeRotationDegreesTemp += 0.001f;

	/*Change the Speed of the Planet and cubes rotation*/
	if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		PlanetRotationDegreesTemp -= 0.001f;
	else if (glfwGetKey(Window, GLFW_KEY_DOWN) == GLFW_PRESS)
		DegreesTemp -= 0.001f;

	if(glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		PlanetRotationDegreesTemp += 0.001f;
	else if (glfwGetKey(Window, GLFW_KEY_UP) == GLFW_PRESS)
		DegreesTemp += 0.001f;
	//Move Forward
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //Sprint Mechanism
		camera.Process_Keyboard(FORWARD, DeltaTime,true);
	else if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Process_Keyboard(FORWARD, DeltaTime,false);
	//Move Backwards
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //Sprint Mechanism
		camera.Process_Keyboard(BACKWARD, DeltaTime, true);
	else if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Process_Keyboard(BACKWARD, DeltaTime,false);
	//Move to the Left
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //Sprint Mechanism
		camera.Process_Keyboard(LEFT, DeltaTime,true);
	else if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Process_Keyboard(LEFT, DeltaTime, false);
	//Move to the Right
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //Sprint Mechanism
		camera.Process_Keyboard(RIGHT, DeltaTime, true);
	else if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Process_Keyboard(RIGHT, DeltaTime,false);
}

void Mouse_CallBack(GLFWwindow* Window, double Xpos, double Ypos)
{
	if (FirstMouse)
	{
		LastX = Xpos;
		LastY = Ypos;
		FirstMouse = false;
	}

	float XoffSet = Xpos - LastX;
	float YoffSet = LastY - Ypos;
	LastX = Xpos;
	LastY = Ypos;
	camera.Process_MouseMovement(XoffSet, YoffSet);
}

void Scroll_CallBack(GLFWwindow* Window, double XoffSet, double YoffSet)
{
	camera.ProcessMouseScroll(YoffSet);
}

unsigned int LoadTexture(const char* Path)
{
	unsigned int TextureID;
	glGenTextures(1, &TextureID);

	int Width, Height, NR_Components;
	unsigned char* Data = stbi_load(Path, &Width, &Height, &NR_Components, 0);
	if (Data)
	{
		GLenum Format;
		if (NR_Components == 1)
			Format = GL_RED;
		else if (NR_Components == 3)
			Format = GL_RGB;
		else if (NR_Components == 4)
			Format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(Data);
	}
	else
	{
		cout << "Texture Failed to load at path:" << Path << endl;
		stbi_image_free(Data);
	}

	return TextureID;
}
