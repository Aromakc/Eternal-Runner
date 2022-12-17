#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shader.h"
#include"VAO.h"
#include"VBO.h"

float NormalizeCoordinate(float input, float min, float max)
{
	//normalize the coordiante of the viewport between 0 and 1
	return (input - min) / (max - min); 
}
void setPixel(float posX, float posY, float k)
{
	float vertices[] = { NormalizeCoordinate(posX, 0, 800), NormalizeCoordinate(posY, 0, 800), 0.0f, k * 0.1, 0, 0 };

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

// Algorithm of Digital Differential Line Drawing 
void DDA_LDA(float xi, float yi, float xj, float yj)
{
	float dy, dx, steps, x, y, k;
	float Xinc, Yinc;
	dx = xj - xi;
	dy = yj - yi;
	if (abs(dy) > abs(dx))
		steps = abs(dy);
	else
		steps = abs(dy);
	Xinc = dx / steps;
	Yinc = dy / steps;
	x = xi;
	y = yi;
	setPixel(x, y,0);
	for (k = 1; k <= steps; k++)
	{
		x += Xinc;
		y += Yinc;
		setPixel(round(x), round(y),0);
	}
}

// Algorithm of Bresenham Line Drawing
void Bresenham_LDA(float xi, float yi, float xj, float yj)
{
	float dy, dx, x,y;
	dx = xj - xi;
	dy = yj - yi;


	if (abs(dy) < abs(dx)) // |m| < 1.0
	{
		float p = 2 * dy - dx;

		//Determine which endpoint to start as start position
		if (xi > xj) {
			x = xj;
			y = yj;
			xj = xi;
		}
		else {
			x = xi;
			y = yi;
		}
		setPixel(x, y, 0);
		while (x < xj) {
			x++;
			if (p < 0) {
				p += (2 * dy);
			}
			else {
				y++;
				p += (2 * (dy - dx));
			}
			setPixel(x, y, 0);
		}
	}
	else // |m| > 1.0
	{
		float p = 2 * dx - dy;

		//Determine which endpoint to start as start position
		if (yi > yj) {
			x = xj;
			y = yj;
			yj = yi;
		}
		else {
			x = xi;
			y = yi;
		}
		setPixel(x, y, 0);
		while (y < yj) {
			y++;
			if (p < 0) {
				p += (2 * dx);
			}
			else {
				x++;
				p += (2 * (dx - dy));
			}
			setPixel(x, y, 0);
		}
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

	Shader shaderProgram("resources/Shader/vertexShader.vs", "resources/Shader/fragmentShader.fs");

	float xi, xj, yi, yj;
	std::cout << "Enter starting coordinate xi yi : ";
	std::cin >> xi >> yi;
	std::cout << "Enter ending coordinate xj yj : ";
	std::cin >> xj >> yj;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();

		//DDA_LDA(xi, yi, xj, yj);
		Bresenham_LDA(xi, yi, xj, yj); //function to draw line
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shaderProgram.~Shader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}