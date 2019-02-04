
//Mark Meritt
//100692252
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86
//PLEASE COMPILE IN x86

#include <glad/glad.h>
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <GLM/glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height),	
	 mouse_callback(GLFWwindow* window, double xpos, double ypos),			
	 scroll_callback(GLFWwindow* window, double xoffset, double yoffset),	
	 processInput(GLFWwindow *window); 

bool firstMouse = true;
float deltaTime = 0.0f, lastFrame = 0.0f;
const unsigned int SCR_WIDTH = 1600, SCR_HEIGHT = 900;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

glm::vec3 ObjectPosition1 = glm::vec3(3.0f, 1.0f, 3.0f);
glm::vec3 ObjectPosition2 = glm::vec3(3.0f, 1.0f, -3.0f);
glm::vec3 ObjectPosition3 = glm::vec3(-3.0f, 1.0f, 3.0f);
glm::vec3 ObjectPosition4 = glm::vec3(-3.0f, 1.0f, -3.0f);

glm::vec3 ObjectScale1 = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 ObjectScale2 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ObjectScale3 = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 ObjectScale4 = glm::vec3(1.0f, 1.0f, 1.0f);

glm::vec3 levelPos = glm::vec3(0.0f, 0.45f, 0.0f);
glm::vec3 levelScale = glm::vec3(0.1f, 0.2f, 0.1f);

Shader ShaderProgram;
Camera camera(glm::vec3(0.0f, 2.6f, 7.0f)); 

glm::vec3 pointLightPositions[] = {
  ObjectPosition1,
   ObjectPosition2,
   ObjectPosition3,
   ObjectPosition4
};

int main() 
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment 1 - Lighting & Color Correction", NULL, NULL);
	if (window == NULL) {
		std::cout << "> Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1; 
	}

	glfwMakeContextCurrent(window);										
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	
	glfwSetCursorPosCallback(window, mouse_callback);					
	glfwSetScrollCallback(window, scroll_callback);						

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "> Failed to initialize GLAD" << std::endl;
		system("pause");
		return -1; 
	}

	glEnable(GL_DEPTH_TEST);								
	glEnable(GL_BLEND);										
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		

	ShaderProgram = Shader("./Assets/Shaders/Light.vert", "./Assets/Shaders/Light.frag");
	Shader lightShader("./Assets/Shaders/shaderLight.vert", "./Assets/Shaders/shaderLight.frag");

	Model object1("./Assets/Models/nanosuit.obj"),		
		  object2("./Assets/Models/nanosuit.obj"),		
		  object3("./Assets/Models/nanosuit.obj"),	
		  object4("./Assets/Models/nanosuit.obj"),	
		  Level("./Assets/Level/Level.obj");
	
	glm::mat4 LevelOne, o1, o2, o3, o4;

	LevelOne = glm::translate(LevelOne, levelPos);
	o1 = glm::translate(o1, ObjectPosition1);
	o2 = glm::translate(o2, ObjectPosition2);
	o3 = glm::translate(o3, ObjectPosition3);
	o4 = glm::translate(o4, ObjectPosition4);

	LevelOne = glm::scale(LevelOne, levelScale);
	o1 = glm::scale(o1, ObjectScale1);
	o2 = glm::scale(o2, ObjectScale1);
	o3 = glm::scale(o3, ObjectScale1);
	o4 = glm::scale(o4, ObjectScale1);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

		ShaderProgram.use();
		ShaderProgram.setInt("material.diffuse", 0);
		ShaderProgram.setInt("material.specular", 2);
		ShaderProgram.setVec3("viewPos", camera.Position);
		ShaderProgram.setFloat("material.shininess", 80);

		ShaderProgram.setVec3("spotLight.position", camera.Position);
		ShaderProgram.setVec3("spotLight.direction", camera.Front);
		ShaderProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setFloat("spotLight.constant", 0.0f);
		ShaderProgram.setFloat("spotLight.linear", 0.00);
		ShaderProgram.setFloat("spotLight.quadratic", 0.032);
		ShaderProgram.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		ShaderProgram.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		ShaderProgram.setMat4("projection", projection);
		ShaderProgram.setMat4("view", camera.GetViewMatrix());

		ShaderProgram.setMat4("model", LevelOne);
		Level.Draw(ShaderProgram);

		ShaderProgram.setMat4("model", o1);
		object1.Draw(ShaderProgram);

		ShaderProgram.setMat4("model", o2);
		object2.Draw(ShaderProgram);

		//glDisable(GL_DEPTH_TEST);
		//glEnable(GL_BLEND);

		lightShader.setMat4("model", o3);
		object3.Draw(lightShader);

		lightShader.setMat4("model", o4);
		object4.Draw(lightShader);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);	

		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	glfwTerminate(); 
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.Position.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.Position.y -= 0.1f; 

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); 
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		//No Lighting

		cout << "No Lighting" << endl;

		ShaderProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
		ShaderProgram.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setFloat("pointLights[0].constant", 0.0f);
		ShaderProgram.setFloat("pointLights[0].linear", 0.00);
		ShaderProgram.setFloat("pointLights[0].quadratic", 0.0);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		//Ambient Lighting Only

		cout << "Ambient Lighting" << endl;

		ShaderProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
		ShaderProgram.setVec3("pointLights[0].ambient", 2.8f, 2.6f, 2.6f);
		ShaderProgram.setVec3("pointLights[0].diffuse", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("pointLights[0].specular", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setFloat("pointLights[0].constant", 1.0f);
		ShaderProgram.setFloat("pointLights[0].linear", 0.09);
		ShaderProgram.setFloat("pointLights[0].quadratic", 0.032);
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		//Specular Lighting Only

		cout << "Specular Lighting" << endl;

		ShaderProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
		ShaderProgram.setVec3("pointLights[0].ambient", 0.0f, 0.0f, 0.0f);
		ShaderProgram.setVec3("pointLights[0].diffuse", 5.5f, 5.5f, 5.5f);
		ShaderProgram.setVec3("pointLights[0].specular", 2.6f, 2.6f, 2.6f);
		ShaderProgram.setFloat("pointLights[0].constant", 1.0f);
		ShaderProgram.setFloat("pointLights[0].linear", 0.19);
		ShaderProgram.setFloat("pointLights[0].quadratic", 0.032);
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		//Specular + Rim Lighting
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		//Ambient + Specular + Rim
	}

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
		//TOGGLE diffuse warp/ramp
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		//TOGGLE specular warp/ramp
	}

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		//TOGGLE Color Grading Warm
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		//TOGGLE Color Grading Cool
	}

	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
		//TOGGLE Color Grading Custom Effect
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse == true) {
		lastX = xpos, lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos, lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
