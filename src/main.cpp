#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

float Normalize(float input, float min, float max)
{
	return (input - min) / (max - min);
}
void DrawPixel(int posX, int posY)
{
	//float vertices[] = {
	//Normalize(posX, 0, 800), Normalize(posY, 0, 800),0.0f
	//};
	////Drawing The initial coordinate
	//VAO VAO1;
	//VAO1.Bind();

	//VBO VBO1(vertices, sizeof(vertices));
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	//
	////VAO1.Unbind();
	////VBO1.Unbind();

	//glPointSize(15);
	//glDrawArrays(GL_POINTS, 0, 1);
	//VAO1.Unbind();
	//VAO1.Delete();
	//VBO1.Delete();

	glBegin(GL_POINTS);
	glVertex2i(posX, posY);
	glEnd();
	glFlush();
}

void DDADisplayLine(int xi, int yi, int xj, int yj)
{
	int dy, dx, steps, x, y, k;
	float Xinc, Yinc;
	dx = xj - xi;
	dy = yj - yi;
	if (abs(dy) > abs(dx))
		steps = abs(dy);
	else
		steps = abs(dy);
	Xinc = (float) (dx / steps);
	Yinc = (float) (dy / steps);
	x = xi;
	y = yi;
	DrawPixel(x, y);
	for (k = 1; k <= steps; k++)
	{
		x += Xinc;
		y += Yinc;
		DrawPixel(round(x), round(y));
	}
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

	//Shader shaderProgram("resources/Shader/vertexShader.vs", "resources/Shader/fragmentShader.vs");

	const int pixelSize = 15;
	int xi, xj, yi, yj;
	std::cout << "Enter starting coordinate xi yi : ";
	std::cin >> xi >> yi;
	std::cout << "Enter ending coordinate xj yj : ";
	std::cin >> xj >> yj;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//shaderProgram.use();
		DDADisplayLine(xi, yi, xj, yj);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//shaderProgram.~Shader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}