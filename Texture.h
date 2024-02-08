#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

using namespace std;

class Texture
{
public:
	Texture();

	void LoadTexture(const char* location, bool option);
	void UseTexture();
	static GLFWcursor* CreateCursor(const char* filePath);
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;

	int width, height, bitDepth;

	const char* fileLocation;

	bool option = 0;
};

