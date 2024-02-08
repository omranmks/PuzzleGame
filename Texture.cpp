#include "Texture.h"

Texture::Texture() {
	width = 0;
	height = 0;
	textureID = 0;
	bitDepth = 0;
}

void Texture::LoadTexture(const char* location, bool option) {
	fileLocation = location;
	unsigned char* texData = stbi_load(location, &width, &height, &bitDepth , 0);
	
	if (!texData) {
		cout << "Failed to find the texture." << endl;
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	if (option) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
}
GLFWcursor* Texture::CreateCursor(const char* filePath) {
	GLFWimage cursorImage;
	int width, height, numChannels;
	unsigned char* image;

	image = stbi_load(filePath, &width, &height, &numChannels, 0);
	if (!image) {
		cout << "Failed to find the texture." << endl;
	}
	cursorImage.width = width;
	cursorImage.height = height;
	cursorImage.pixels = image;
	GLFWcursor* temp = glfwCreateCursor(&cursorImage, 0, 0);
	stbi_image_free(image);
	return temp;
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	width = 0;
	height = 0;
	bitDepth = 0;
	textureID = 0;
}

Texture::~Texture() {
	ClearTexture();
}