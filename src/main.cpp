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
void setPixel(float posX, float posY)
{
	float vertices[] = { NormalizeCoordinate(posX, 0, 800), NormalizeCoordinate(posY, 0, 800), 0.0f, 0, 0, 1 };

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

void Midpoint_EDA(int xCenter, int yCenter, int rx, int ry)
{
	float x = 0;
	float y = ry;//(0,ry) ---
	float p1 = ry * ry - (rx * rx) * ry + (rx * rx) * (0.25);
	//slope
	float dx = 2 * (ry * ry) * x;
	float dy = 2 * (rx * rx) * y;
	while (dx < dy)
	{
		//plot (x,y)
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y);
		if (p1 < 0)
		{
			x = x + 1;
			dx = 2 * (ry * ry) * x;
			p1 = p1 + dx + (ry * ry);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dx = 2 * (ry * ry) * x;
			dy = 2 * (rx * rx) * y;
			p1 = p1 + dx - dy + (ry * ry);
		}
	}
	//ploting for 2nd region of 1st quardant and the slope will be > -1
	//----------------------Region-2------------------------//
	float p2 = (ry * ry) * (x + 0.5) * (x + 0.5) + (rx * rx) * (y - 1) * (y - 1) - (rx *
		rx) * (ry * ry);
	while (y > 0)
	{
		//plot (x,y)
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y); //glEnd();
		if (p2 > 0)
		{
			x = x;
			y = y - 1;
			dy = 2 * (rx * rx) * y;
			//dy = 2 * rx * rx *y;
			p2 = p2 - dy + (rx * rx);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dy = dy - 2 * (rx * rx);
			dx = dx + 2 * (ry * ry);
			p2 = p2 + dx -
				dy + (rx * rx);
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

	float rx,ry;
	std::cout << "Enter radiusX and radiusY the circle - r : ";
	std::cin >> rx >> ry;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		
		// left mouse button increases radius
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
			rx += 1;														   
		// right mouse button decreases radius
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			ry += 1;
		// left mouse button increases radius
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS)
			rx -= 1;
		// right mouse button decreases radius
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS)
			ry -= 1;

		Midpoint_EDA(0,0,rx,ry);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shaderProgram.~Shader();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}