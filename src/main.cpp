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
void setPixel(float posX, float posY)
{
	float vertices[] = { normalizeCoordinate(posX, 0, 800), normalizeCoordinate(posY, 0, 800), 0.0f, 0, 0, 1 };

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	VAO1.Unbind();
	VBO1.Unbind();

	VAO1.Bind();
	glPointSize(5); //specify the diameter of rasterized points
	glDrawArrays(GL_POINTS, 0, 1); // point as primitive, start, size

	VAO1.Unbind();
	VAO1.Delete();
	VBO1.Delete();	
}
void Midpoint_CDA(float r)
{
	float x = 0;
	float y = r;
	float p = 1 - r; // p parameter is integer
	while (y > x)
	{
		x++;
		if (p < 0)
		{
			p += (2 * x + 1);
		}
		else
		{
			y--;
			p += (2 * (x - y) + 1);
		}
		// ploting 8-octants 
		setPixel(x, y); // start from mid-top
		setPixel(-x, y);
		setPixel(x, -y);
		setPixel(-x, -y);
		setPixel(y, x);
		setPixel(-y, x);
		setPixel(y, -x);
		setPixel(-y, -x);
	}
}
void displayRotation(Shader& shaderProgram, VAO& VAO1)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void displayScale(Shader& shaderProgram, float scaleX, float scaleY, VAO& VAO1)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void displayTranslation(Shader& shaderProgram, float& x, float& y, float& speedX, float& speedY, float deltaTime)
{
	x += speedX * deltaTime;
	y += speedY * deltaTime;
	if (x + 25 > 200 || x - 25 < -200) speedX *= -1;
	if (y + 25 > 200 || y - 25 < -200) speedY *= -1;

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(normalizeCoordinate(x, 0, 800), normalizeCoordinate(y, 0, 800), 0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	Midpoint_CDA(50);
}
void displayShear(Shader& shaderProgram, float x, VAO& VAO1)
{
	glm::mat4 trans = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		x, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void displayReflection(GLFWwindow* window, Shader& shaderProgram, VAO VAO1, float x, float y)
{
	glm::mat4 trans(1.0f);
	glm::mat4 ReflectX(
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	glm::mat4 ReflectY(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		trans = ReflectX * trans;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		trans = ReflectY * trans;

	trans = glm::translate(trans, glm::vec3(normalizeCoordinate(x, 0, 800), normalizeCoordinate(y, 0, 800), 0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	VAO1.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void takeInput(GLFWwindow* window, float& x, float& y)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x += 1;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x -= 1;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y += 1;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y -= 1;
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
		-0.25f, 0.0, 0.0f,	1,0,0,
		0.25f, 0.0f, 0.0f,	1,0,0,
	   -0.25f, 0.5f, 0.0f,	1,0,0,
		0.25f, 0.5f, 0.0f,	1,0,0
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
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
	glm::mat4 reflectX(
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	glm::mat4 reflectY(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		//initialize matrix to identity matrix first
		glm::mat4 transform	(1.0f);
		//transform = glm::translate(transform, glm::vec3(0.25f, 0.25f, 1.0f));
		//transform = glm::rotate(transform, (float)glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(2.0f, 1.0f, 1.0f));
		transform = transform * reflectX;
		transform = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.2f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);


		// get matrix's uniform location and set matrix
		shaderProgram.use();
		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// render the new vertices
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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