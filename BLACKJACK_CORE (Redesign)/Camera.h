#pragma once
#include <glad/glad.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default global camera values
const float YAW = -90.0f,			//left and right
			PITCH = 0.0f,			//up and down
			ROLL = 0.0f,			//tilting rotations
			SPEED = 2.5f,			//camera movement speed
			SENSITIVITY = 0.035f,	//mouse sensitivity
			ZOOM = 45.0f;			//camera zoom speed

class Camera {

public:
	glm::vec3 Position, Front, Up, Right, WorldUp;	// Camera Attributes
	float Yaw, Pitch, Roll;							// Euler Angles
	float MovementSpeed, MouseSensitivity, Zoom;	// Camera control options

	
	// Constructor with vectors //
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		   float yaw = YAW, float pitch = PITCH, float roll = ROLL):
		   Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		   MovementSpeed(SPEED),  MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		Roll = roll;
		updateCameraVectors();
	}

	// Constructor with scalars //
	Camera(float posX, float posY, float posZ, 
		   float upX, float upY, float upZ, 
		   float yaw, float pitch): 
		   Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	// functions //

	// Returns view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}


	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		//float velocity = MovementSpeed * deltaTime;
		//if (direction == FORWARD) Position += Front * velocity;
		//if (direction == BACKWARD) Position -= Front * velocity;
		//if (direction == LEFT) Position -= Right * velocity;
		//if (direction == RIGHT) Position += Right * velocity;
	
		if (direction == FORWARD) Position += Front * 0.008f;
		if (direction == BACKWARD) Position -= Front * 0.008f;
		if (direction == LEFT) Position -= Right * 0.008f;
		if (direction == RIGHT) Position += Right * 0.008f;
	}


	// Processes input received from mouse movement. Expects offset value in x & y directions
	void ProcessMouseMovement(float xoffset, float yoffset) {
		Yaw += xoffset * MouseSensitivity;
		Pitch += yoffset * MouseSensitivity;

		// when pitch out of bounds, screen no flippy
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
		
		updateCameraVectors(); // Update Front, Right and Up Vectors using the updated Euler angles
	}


	// Zooms with scroll-wheel
	void ProcessMouseScroll(float yoffset) {
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}


private:
	// Calcs the front vector from the Camera's (updated) Euler Angles - yaw/pitch
	void updateCameraVectors() {
		// calc the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// recalc the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  //Normalize vectors bc length gets closer to 0 when looking up/down, makes camera movement slower
		Up = glm::normalize(glm::cross(Right, Front));
	}
};