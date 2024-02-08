#pragma once

#include<iostream>
#include<fstream>
#include <string>
#include<GL/glew.h>	

using namespace std;

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);

	string ReadFile(const char* filePath);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetColorLocation();
	GLuint GetToggleLocation();

	void UseShader();
	void ClearShader();

	~Shader();
private:
	GLuint shaderID, uniModel, uniProjection, uniView, uniColor, toggle;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

