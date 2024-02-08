#pragma once

#include<iostream>

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "Texture.h"

using namespace std;

class OpenGLWindow
{
public:
	OpenGLWindow();
	OpenGLWindow(GLint width, GLint height);

	int Initialise();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHiehgt() { return bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* GetKeys() { return keys; }
	bool GetMouseButton() { return mouseButton; }
	pair<GLfloat,GLfloat> GetMousePos();

	void SwapBuffer() { glfwSwapBuffers(mainWindow); }


	~OpenGLWindow();
private:
	GLint width, height, bufferWidth, bufferHeight;

	GLFWwindow* mainWindow;
	GLFWcursor* pointer;
	GLFWcursor* pointerClicked;

	bool keys[1024];
	bool mouseButton;

	GLfloat curX, curY;

	bool mouseFirstMoved;

	void CreateCallBacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double posX, double posY);
	static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mods);
};

