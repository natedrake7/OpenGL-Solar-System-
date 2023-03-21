#include "Camera.h"

Camera::Camera(): Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovemementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(0.0f,4.0f,20.0f);
	WorldUp = glm::vec3(0.0f,1.0f,0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	UpdateCameraVectors();
}

Camera::Camera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovemementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(PosX, PosY, PosZ);
	WorldUp = glm::vec3(UpX, UpY, UpZ);
	Yaw = YAW;
	Pitch = PITCH;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Process_Keyboard(Camera_Movemement Direction, float DeltaTime,bool Sprint)
{
	float Velocity = MovemementSpeed * DeltaTime;
	float sprint = 2.0f;
	if (Direction == FORWARD && Sprint == true) //if user presses W  + LSHIFT
		Position += Front * Velocity * sprint; //double the movement speed in the direction he is moving
	else if (Direction == FORWARD) //else if he presses only W
		Position += Front * Velocity; //regular movement speed

	if (Direction == BACKWARD && Sprint == true) //if user presses S  + LSHIFT
		Position -= Front * Velocity * sprint; //double the movement speed in the direction he is moving
	else if (Direction == BACKWARD) //else if he presses only S
		Position -= Front * Velocity; //regular movement speed

	if (Direction == LEFT && Sprint == true) //if user presses A  + LSHIFT
		Position -= Right * Velocity * sprint; //double the movement speed in the direction he is moving
	else if (Direction == LEFT) //else if he presses only A
		Position -= Right * Velocity; //regular movement speed

	if (Direction == RIGHT && Sprint == true) //if user presses D  + LSHIFT
		Position += Right * Velocity * sprint; //double the movement speed in the direction he is moving
	else if (Direction == RIGHT) //else if he presses only D
		Position += Right * Velocity; //regular movement speed
}

void Camera::Process_MouseMovement(float XoffSet, float YoffSet)
{
	XoffSet *= MouseSensitivity;
	YoffSet *= MouseSensitivity;
	Yaw += XoffSet;
	Pitch += YoffSet;

	//make sure if pitch gets out of bound the screen doesnt get flipped
	if (Pitch > 89.0f)
			Pitch = 89.0f;
	if (Pitch < -89.0f)
			Pitch = -89.0f;

	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float YoffSet)
{
	Zoom -= (float)YoffSet;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 front;
	/*Update the Camera Vectors using the Euler Angles*/
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

