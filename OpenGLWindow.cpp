#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow() {
    width = 1920;
    height = 1080;

    for (int i = 0; i < 1024;i++) {
        keys[i] = 0;
    }

    //CreateCursor();
}
OpenGLWindow::OpenGLWindow(GLint width, GLint height) {
    this->width = width;
    this->height = height;

    for (int i = 0; i < 1024;i++) {
        keys[i] = 0;
    }
}
int OpenGLWindow::Initialise() {
    if (!glfwInit())
    {
        cout << "GLFW did not initialise";
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "TESTING", /*NULL*/glfwGetPrimaryMonitor(), NULL);

    if (!mainWindow) {
        cout << "Could not create window";
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    CreateCallBacks();

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        cout << "GLEW did not inistialise";
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    //glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, bufferWidth, bufferHeight);

    pointer = Texture::CreateCursor("Texture/pointer.png");

    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursor(mainWindow, pointer);

    glfwSetWindowUserPointer(mainWindow, this);
}

void OpenGLWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
    OpenGLWindow* theWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            theWindow->keys[key] = false;
        }
    }
}
void OpenGLWindow::HandleMouse(GLFWwindow* window, double posX, double posY) {
    OpenGLWindow* theWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));

    theWindow->curX = posX;
    theWindow->curY = posY;

    cout << posX << '\t' << posY << endl;
}

void OpenGLWindow::HandleMouseButtons(GLFWwindow* window, int button, int action, int mods) {
    OpenGLWindow* theWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);

        if (action == GLFW_PRESS) {
            theWindow->mouseButton = true;
        }
        if (action == GLFW_RELEASE) {
            theWindow->mouseButton = false;
        }

        theWindow->curX = posX;
        theWindow->curY = posY;
    }

    if(theWindow->keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, GL_TRUE);
}

pair<GLfloat,GLfloat> OpenGLWindow::GetMousePos() {
    return make_pair(curX, curY);
}

void OpenGLWindow::CreateCallBacks() {
    glfwSetKeyCallback(mainWindow, HandleKeys);
    //glfwSetCursorPosCallback(mainWindow, HandleMouse);
    glfwSetMouseButtonCallback(mainWindow, HandleMouseButtons);
}
OpenGLWindow::~OpenGLWindow() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}