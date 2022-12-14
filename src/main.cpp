#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shaderClass.h"

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Aromac", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, 800, 800);

	//Normalized coordinates for  for Nepal Flag
	float positions[] = {

		// Triangle1-Outer  (vertex + color)
		-1.0f,  -1.0f,  0.5f,	0.0f, 0.21f, 0.58f,		//bottom left
		 0.68f, -1.0f,  0.5f,	0.0f, 0.21f, 0.58f,		//bottom right
		-1.0f,   0.7f,  0.5f,	0.0f, 0.21f, 0.58f,		//mid-top left
								
		-1.0f,   0.0,   0.5f,	0.0f, 0.21f, 0.58f,		//mid left
		 0.72f,  0.0f,  0.5f,	0.0f, 0.21f, 0.58f,		//mid right 
		-1.0f,   1.0f,  0.5f,	0.0f, 0.21f, 0.58f,		//top left

		// Triangle2-Inner	
		-0.95f,	-0.95f,	0.5f,	0.87f, 0.04f, 0.2f,		//bottom left
		 0.55f, -0.95f,	0.5f,	0.87f, 0.04f, 0.2f,		//bottom right
		-0.95f,	 0.6f,	0.5f,	0.87f, 0.04f, 0.2f,		//mid top left
							
		-0.95f,	 0.05f,  0.5f,	0.87f, 0.04f, 0.2f,		//mid left
		 0.52f,  0.05f,  0.5f,	0.87f, 0.04f, 0.2f,		//mid right 
		-0.95f,  0.9f,  0.5f,	0.87f, 0.04f, 0.2f,		//top left
	};
	
	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO); //1 buffer	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shaderProgram("resources/Shader/vertexShader.vs", "resources/Shader/fragmentShader.fs");

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT); //clear back buffer and assign new color
		
		shaderProgram.use();
		
		glDrawArrays(GL_TRIANGLES, 0, 12);
		
		glfwSwapBuffers(window); //swap back buffer with front buffer
		glfwPollEvents(); //window responding: appering resizing
	}

	shaderProgram.~Shader();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
