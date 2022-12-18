#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"EBO.h"
#include"VAO.h"
#include"VBO.h"


float normalizeCoordinate(float input, float min, float max)
{
	//normalizeCoordinate the coordiant of the viewport between 0 and 1
	return (input - min) / (max - min); 
}
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Aromakc", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("resources/Shader/vertexShader.vs", "resources/Shader/fragmentShader.fs");

	float vertices[] = {
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
		-0.5f, 0.0f, -0.5f,     0.73f, 0.70f, 0.54f,
		 0.5f, 0.0f, -0.5f,     0.63f, 0.70f, 0.64f,
		 0.5f, 0.0f,  0.5f,     0.53f, 0.70f, 0.74f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	VAO VAO1;
	VAO1.Bind(); 

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));


	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Variables that help the rotation of the pyramid
	float rotation = 0.0f;
	float scale = 0.9f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);
	
	// Random Number Between 0 and 1
	//srand(time(0));
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.use();
		
		//Simple timer
		double currentTime = glfwGetTime();
		if (currentTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = currentTime;
		}

		// Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.1f, 0.0f));
		//proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		


		//model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale));
		 
		
		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shaderProgram.~Shader();
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}