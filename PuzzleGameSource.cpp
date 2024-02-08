#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <unordered_set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bass.h"

#include "Game.h"
#include "Mesh.h"
#include "Shader.h"
#include "OpenGLWindow.h"


using namespace std;

vector<Mesh*> meshes;
vector<Shader*> shaders;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shader/vertex.shader";
static const char* fShader = "Shader/fragment.shader";

void CreateShaders() {
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vShader, fShader);
    shaders.push_back(shader1);
}

int main(int argc, char* numbers[]) {
    unordered_set<int> listValue;
    int count = 1;
    while (numbers[count] != NULL) {

        char* endPtr;
        int integerValue = strtol(numbers[count], &endPtr, 10);

        if (*endPtr == '\0' && 0 <= integerValue && integerValue <= 8) {
            listValue.insert(integerValue);
        }
        else {
            cout << "Invalid integer value" << endl;
        }
        count++;
    }
    if ((count != 10 && count != 1) || (listValue.size() != 9 && count != 1)) {
        cout << "Invalide Parameters" << endl;
        cout << "Game end!" << endl;
        return -1;
    }

    OpenGLWindow mainWindow = OpenGLWindow();
    mainWindow.Initialise();

    CreateShaders();

    if (BASS_Init(-1, 44100, 0, NULL, NULL) == FALSE) {
        std::cout << "Failed to initialize BASS" << std::endl;
        return -1;
    }

    GLuint uniModel = 0, uniProjection = 0, uniView = 0, uniColor = 0;

    glm::mat4 projection = glm::ortho(-960.0f, 960.0f, -540.0f, 540.0f, 1.0f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));

    uniColor = shaders[0]->GetColorLocation();

    uniProjection = shaders[0]->GetProjectionLocation();
    uniView = shaders[0]->GetViewLocation();
    uniModel = shaders[0]->GetModelLocation();


    Game game = Game(uniModel, uniColor, shaders[0]->GetToggleLocation(), listValue, count);

    
    while (!mainWindow.GetShouldClose()) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaders[0]->UseShader();

        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(uniProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model;
        pair<float, float> temp = mainWindow.GetMousePos();
        game.Update(mainWindow.GetKeys(), mainWindow.GetMouseButton(), temp.first, temp.second);

        glUseProgram(0);

        mainWindow.SwapBuffer();
    }
    // Clean up
    
    BASS_Free();

    return 0;
}