#pragma once

#include<glad/glad.h>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	unsigned int ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(unsigned int* indices, GLsizeiptr size);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};