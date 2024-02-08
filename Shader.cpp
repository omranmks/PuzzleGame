#include "Shader.h"

Shader::Shader() {
    shaderID = 0;
    uniModel = 0;
    uniProjection = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation) {
    string vertexString = ReadFile(vertexLocation);
    string fragmentString = ReadFile(fragmentLocation);

    CompileShader(vertexString.c_str(), fragmentString.c_str());
}

string Shader::ReadFile(const char* filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);

    if (!fileStream.is_open()) {
        cout << "file did not open";
        return "";
    }

    string line;
    while (!fileStream.eof()) {
        getline(fileStream, line);
        content += line + '\n';
    }

    fileStream.close();
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {

    shaderID = glCreateProgram();

    if (!shaderID) {
        cout << "Error";
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        cout << eLog;
        return;
    }

    glValidateProgram(shaderID);

    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        cout << eLog;
        return;
    }

    uniModel = glGetUniformLocation(shaderID, "model");
    uniView = glGetUniformLocation(shaderID, "view");
    uniProjection = glGetUniformLocation(shaderID, "projection");
    uniColor = glGetUniformLocation(shaderID, "fColor");
    toggle = glGetUniformLocation(shaderID, "toggle");
    
}

GLuint Shader::GetModelLocation() {
    return uniModel;
}
GLuint Shader::GetProjectionLocation() {
    return uniProjection;
}
GLuint Shader::GetViewLocation() {
    return uniView;
}
GLuint Shader::GetColorLocation() {
    return uniColor;
}
GLuint Shader::GetToggleLocation() {
    return toggle;
}

void Shader::UseShader() {
    glUseProgram(shaderID);
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        cout << eLog << shaderType;
        return;
    }

    glAttachShader(theProgram, theShader);
}

void Shader::ClearShader() {
    if (shaderID != 0) {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    uniModel = 0;
    uniProjection = 0;
}

Shader::~Shader() {
    ClearShader();
}