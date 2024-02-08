#pragma once

#include <GL/glew.h>	
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

#include "bass.h"

#include "Mesh.h"
#include "Texture.h"

using namespace std;

class Game
{
public:
	Game(GLuint uniModel, GLuint uniColor, GLuint toggle, unordered_set<int> numbers, int count);
	
	void Update(bool* keys, bool button, float mouseX, float mouseY);
	
	~Game();
		
private:
	void Change(int newIndexX, int newIndexY);

	void CreateTexture();

	void DrawBackground();
	void DrawNumberBoxes();
	void DrawWinningBackground();
	void DrawMenu();

	void ProccessKeys();
	void ProccessMouse();
	void ProcessMenuButtons(int index, glm::mat4 model);

	void NewPosition();
	void BlackBackground();
	void ClickedButton(glm::mat4 model);

	void ButtonClicked();

	GLfloat vertices[20] = {
		 -1.0f,  1.0f,  1.0f,         0.0f, 0.0f,
		  1.0f,  1.0f,	1.0f,         1.0f, 0.0f,
		  1.0f, -1.0f,	1.0f,         1.0f, 1.0f,
		 -1.0f, -1.0f,	1.0f,         0.0f, 1.0f
	};
	unsigned int indices[6] = {
		0, 3, 2,
		0, 1, 2
	};
	vector<vector<pair<float, float>>> positions ={
		{	make_pair(-260.0f,260.0f),	make_pair(0.0f,260.0f),	make_pair(260.0f,260.0f)	},
		{	make_pair(-260.0f,0.0f),	make_pair(0.0f,0.0f),	make_pair(260.0f,0.0f)		},
		{	make_pair(-260.0f,-260.0f),	make_pair(0.0f,-260.0f),make_pair(260.0f,-260.0f)	}	
	};

	vector<vector<pair<float, float>>> cornorPos = {
		{make_pair(590.0f, 160.0f),	make_pair(850.0f, 160.0f),	make_pair(1110.0f, 160.0f)},
		{make_pair(590.0f, 420.0f),	make_pair(850.0f, 420.0f),	make_pair(1110.0f, 420.0f)},
		{make_pair(590.0f, 680.0f),	make_pair(850.0f, 680.0f),	make_pair(1110.0f, 680.0f)}
	};

	int blankIndexX = 2;
	int blankIndexY = 2;

	int targetedIndexX = -1;
	int targetedIndexY = -1;


	vector<vector<int>> winOrder = {
	{1,2,3},
	{4,5,6},
	{7,8,0}
	};

	vector<vector<int>> numbers = {
		{1,2,3},
		{4,6,8},
		{7,5,0}
	};

	vector<vector<int>> numbersRestart = {
		{1,2,3},
		{4,6,8},
		{7,5,0}
	};

	bool isProccess = false;
	bool disableControl = false;
	bool openMenu = false;
	bool closeMenu = false;
	bool blackBackgound = false;
	bool muteToggle = false;
	bool restartToggle = false;

	bool oneTime = true;
	bool oneTimeSound = true;
	bool sound = true;

	bool up = false, down = false, left = false, right = false;

	bool* keys;
	bool mouseButton;
	float mouseX, mouseY;

	GLfloat now, lastTime = 0, deltaTime, fade = 0.0f;

	GLuint uniModel, uniColor, toggle;
	glm::vec3 model;

	pair<float, float> oldPos;

	Texture boardTex;
	Texture backgroundTex;
	Texture backgroundTexWinning;
	vector<vector<Texture*>> numberBoxTex;
	vector<vector<Texture*>> numberBoxTexRestart;

	Texture menuButton, exitButton, muteButton, closeButton, restartButton, menu;

	pair<float, float> menuButtonsPos = make_pair(65.0f,65.0f);
	vector<pair<float, float>> menuButtons = {
		make_pair(710.0f,495.0f), make_pair(910,495.0f), make_pair(1110,495.0f), make_pair(1220.0f,380.0f)
	};

	Mesh obj;

	HSTREAM slideWood, buttonClicked, winningSound, soundtrack;
};

