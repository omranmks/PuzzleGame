#include "Game.h"

Game::Game(GLuint uniModel, GLuint uniColor, GLuint toggle, unordered_set<int> numbersList, int count) {
	if (count == 1) {
		cout << "Default values will be loaded" << endl;
	}
	else {
		unordered_set<int>::iterator it = numbersList.begin();

		for (int i = 0; i < 3;i++) {
			for (int j = 0;j < 3;j++) {
				if (*it == 0) {
					blankIndexX = i;
					blankIndexY = j;
				}
				numbers[i][j] = *it;
				it++;

			}
		}
		numbersRestart = numbers;
	}

	slideWood = BASS_StreamCreateFile(FALSE, "SoundEffect/slideWood.mp3", 0, 0, 0);
	buttonClicked = BASS_StreamCreateFile(FALSE, "SoundEffect/buttonClicked.mp3", 0, 0, 0);
	winningSound = BASS_StreamCreateFile(FALSE, "SoundEffect/winning.mp3", 0, 0, 0);
	soundtrack = BASS_StreamCreateFile(FALSE, "SoundEffect/soundtrack.mp3", 0, 0, BASS_SAMPLE_LOOP);

	if (slideWood == 0) {
		std::cout << "Failed to load audio file" << std::endl;
	}
	BASS_ChannelSetAttribute(slideWood, BASS_ATTRIB_VOL, 0.15f);
	BASS_ChannelSetAttribute(buttonClicked, BASS_ATTRIB_VOL, 0.75f);
	BASS_ChannelSetAttribute(winningSound, BASS_ATTRIB_VOL, 0.75f);
	BASS_ChannelSetAttribute(soundtrack, BASS_ATTRIB_VOL, 0.5f);

	this->uniModel = uniModel;
	this->uniColor = uniColor;
	this->toggle = toggle;

	CreateTexture();
	obj.CreateMesh(vertices, indices, 20, 6);
	
	BASS_ChannelPlay(soundtrack, true);
}

void Game::CreateTexture() {
	backgroundTex.LoadTexture("Texture/wood.png", 0);
	boardTex.LoadTexture("Texture/board.png", 1);
	backgroundTexWinning.LoadTexture("Texture/youWin.png", 1);
	
	menu.LoadTexture("Texture/menu.png", 1);
	menuButton.LoadTexture("Texture/menuButton.png", 1);
	exitButton.LoadTexture("Texture/exitButton.png", 1);
	muteButton.LoadTexture("Texture/muteButton.png", 1);
	restartButton.LoadTexture("Texture/restartButton.png", 1);
	closeButton.LoadTexture("Texture/closeButton.png", 1);


	for (int i = 0; i < 3; i++) {
		vector<Texture*> temps;
		numberBoxTex.push_back(temps);
		for (int j = 0; j < 3; j++) {
			Texture* temp = new Texture();
			numberBoxTex[i].push_back(temp);
			if (i == blankIndexX && j == blankIndexY)
				continue;
			numberBoxTex[i][j]->LoadTexture(("Texture/number-" + to_string(numbers[i][j]) + ".png").c_str(), 1);
		}
	}

	numberBoxTexRestart = numberBoxTex;
}
void Game::Update(bool* keys, bool button, float mouseX, float mouseY) {
		
	this->keys = keys;
	this->mouseButton = button;
	this->mouseX = mouseX;
	this->mouseY = mouseY;

	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	if (!disableControl) {
		ProccessKeys();
		ProccessMouse();
	}

	DrawBackground();
	DrawNumberBoxes();

	if (winOrder == numbers)
	{
		disableControl = true;
		DrawWinningBackground();
	}

	DrawMenu();
}
void Game::DrawBackground() {

	glm::mat4 model;

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(960.0f, 540.0f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	backgroundTex.UseTexture();
	obj.RenderMesh();


	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(500.0f, 500.0f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	boardTex.UseTexture();
	obj.RenderMesh();
}

void Game::DrawNumberBoxes() {
	for (int i = 0; i < 3;i++) {
		for (int j = 0; j < 3;j++) {
			if (numbers[i][j] == 0)
				continue;

			glm::mat4 model = glm::mat4(1.0f);

			if (targetedIndexX == i && targetedIndexY == j) {
				NewPosition();
				if (((down || right) && (oldPos.first <= positions[i][j].first && oldPos.second >= positions[i][j].second)) ||
					((up || left) && (oldPos.first >= positions[i][j].first && oldPos.second <= positions[i][j].second)))
					model = glm::translate(model, glm::vec3(oldPos.first, oldPos.second, 0.0f));
				else {
					model = glm::translate(model, glm::vec3(positions[i][j].first, positions[i][j].second, 0.0f));
					isProccess = false;
					up = down = left = right = false;
					targetedIndexX = -1;
					targetedIndexY = -1;
					oldPos.first = -1;
					oldPos.second = -1;
				}
			}
			else
				model = glm::translate(model, glm::vec3(positions[i][j].first, positions[i][j].second, 0.0f));
			model = glm::scale(model, glm::vec3(130.0f, 130.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			numberBoxTex[i][j]->UseTexture();
			obj.RenderMesh();
		}
	}
}

void Game::ProccessKeys() {
	if (!isProccess) {
		if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
			if (blankIndexX - 1 >= 0) {
				isProccess = true;
				down = true;
				Change(blankIndexX - 1, blankIndexY);
			}
		}
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
			if (blankIndexX + 1 <= 2) {
				isProccess = true;
				up = true;
				Change(blankIndexX + 1, blankIndexY);
			}
		}
		if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
			if (blankIndexY + 1 <= 2) {
				isProccess = true;
				left = true;
				Change(blankIndexX,blankIndexY + 1);
			}
		}
		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
			if (blankIndexY - 1 >= 0) {
				isProccess = true;
				right = true;
				Change(blankIndexX, blankIndexY - 1);
			}
		}
	}
}
void Game::ProccessMouse() {
	if (!isProccess) {
		if (mouseButton) {
			//DOWN
			if (blankIndexX - 1 >= 0 &&
				cornorPos[blankIndexX - 1][blankIndexY].first <= mouseX &&
				mouseX <= cornorPos[blankIndexX - 1][blankIndexY].first + 230.0f &&
				cornorPos[blankIndexX - 1][blankIndexY].second <= mouseY &&
				mouseY <= cornorPos[blankIndexX - 1][blankIndexY].second + 230.0f)
			{
				isProccess = true;
				down = true;
				Change(blankIndexX - 1, blankIndexY);
			}
			//UP
			else if (blankIndexX + 1 <= 2 &&
				cornorPos[blankIndexX + 1][blankIndexY].first <= mouseX &&
				mouseX <= cornorPos[blankIndexX + 1][blankIndexY].first + 230.0f &&
				cornorPos[blankIndexX + 1][blankIndexY].second <= mouseY &&
				mouseY <= cornorPos[blankIndexX + 1][blankIndexY].second + 230.0f)
			{
				isProccess = true;
				up = true;
				Change(blankIndexX + 1, blankIndexY);
			}
			//RIGHT
			else if (blankIndexY - 1 >= 0 &&
				cornorPos[blankIndexX][blankIndexY - 1].first <= mouseX &&
				mouseX <= cornorPos[blankIndexX][blankIndexY - 1].first + 230.0f &&
				cornorPos[blankIndexX][blankIndexY - 1].second <= mouseY &&
				mouseY <= cornorPos[blankIndexX][blankIndexY - 1].second + 230.0f)
			{
				isProccess = true;
				right = true;
				Change(blankIndexX, blankIndexY - 1);
			}
			//LEFT
			else if (blankIndexY + 1 <= 2 &&
				cornorPos[blankIndexX][blankIndexY + 1].first <= mouseX &&
				mouseX <= cornorPos[blankIndexX][blankIndexY + 1].first + 230.0f &&
				cornorPos[blankIndexX][blankIndexY + 1].second <= mouseY &&
				mouseY <= cornorPos[blankIndexX][blankIndexY + 1].second + 230.0f)
			{
				isProccess = true;
				left = true;
				Change(blankIndexX, blankIndexY + 1);
			}
			//else {
			//	restartToggle = false;
			//	disableControl = false;
			//	//closeMenu = true;
			//	oneTimeSound = true;
			//	numbers = numbersRestart;
			//	blankIndexX = 2;
			//	blankIndexY = 2;
			//	numberBoxTex = numberBoxTexRestart;
			//}
		}
	}
}

void Game::Change(int newIndexX, int newIndexY) {
	if (sound) {
		BASS_ChannelSetPosition(slideWood, 0, BASS_POS_BYTE);
		BASS_ChannelPlay(slideWood, FALSE);
	}

	numbers[blankIndexX][blankIndexY] = numbers[newIndexX][newIndexY];
	numbers[newIndexX][newIndexY] = 0;

	numberBoxTex[blankIndexX][blankIndexY] = numberBoxTex[newIndexX][newIndexY];
	numberBoxTex[newIndexX][newIndexY] = new Texture();

	oldPos.first = positions[newIndexX][newIndexY].first;
	oldPos.second = positions[newIndexX][newIndexY].second;

	targetedIndexX = blankIndexX;
	targetedIndexY = blankIndexY;

	blankIndexX = newIndexX;
	blankIndexY = newIndexY;

}
void Game::NewPosition() {
	if(up)
		oldPos.second += 400.0f * deltaTime;
	else if(down)		 
		oldPos.second -= 400.0f * deltaTime;
	else if(right)		 
		oldPos.first +=  400.0f * deltaTime;
	else if(left)		 
		oldPos.first -=  400.0f * deltaTime;
}

void Game::DrawWinningBackground() {
	if (!isProccess) {
		
		if (sound && oneTimeSound)
		{
			oneTimeSound = false;
			BASS_ChannelPlay(winningSound, FALSE);
		}

		glm::mat4 model;

		BlackBackground();

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(425.0f, 225.0f, 0.0f));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		backgroundTexWinning.UseTexture();
		obj.RenderMesh();
	}
}

void Game::DrawMenu() {
	glm::mat4 model;

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-850.0f, 420.0f, 0.0f));
	model = glm::scale(model, glm::vec3(45.0f, 50.0f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	menuButton.UseTexture();

	if (mouseButton &&
		menuButtonsPos.first <= mouseX &&
		mouseX <= menuButtonsPos.first + 90.0f &&
		menuButtonsPos.second <= mouseY &&
		mouseY <= menuButtonsPos.second + 100.0f &&
		oneTime) {

		ClickedButton(model);

		openMenu = true;
		disableControl = true;
	}
	else {
		obj.RenderMesh();

		if (openMenu) {
			if (oneTime) {
				oneTime = false;
				ButtonClicked();
			}

			BlackBackground();
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(350.0f, 150.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			menu.UseTexture();
			obj.RenderMesh();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-200.0f, -10.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f, 55.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			exitButton.UseTexture();
			ProcessMenuButtons(0, model);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.0f, -10.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f, 55.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			muteButton.UseTexture();
			ProcessMenuButtons(1, model);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(200.0f, -10.0f, 0.0f));
			model = glm::scale(model, glm::vec3(50.0f, 55.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			restartButton.UseTexture();
			ProcessMenuButtons(2, model);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(300.0f, 115.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f, 45.0f, 0.0f));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
			closeButton.UseTexture();
			ProcessMenuButtons(3, model);
		}
	}
}
void Game::ProcessMenuButtons(int index, glm::mat4 model) {
	
	if (mouseButton &&
		menuButtons[index].first <= mouseX &&
		mouseX <= menuButtons[index].first + 100.0f &&
		menuButtons[index].second <= mouseY &&
		mouseY <= menuButtons[index].second + 110.0f) {
		if (index == 0) {
			ClickedButton(model);
			keys[GLFW_KEY_ESCAPE] = true;
			
		}
		if (index == 1) {
			ClickedButton(model);
			muteToggle = true;
		}
		if (index == 2) {
			ClickedButton(model);
			restartToggle = true;
		}if (index == 3) {
			ClickedButton(model);
			closeMenu = true;
		}
	}
	else {
		obj.RenderMesh();
		if (index == 1 && muteToggle) {
			muteToggle = false;
			sound = !sound;
			if (sound)
				BASS_ChannelPlay(soundtrack, true);
			else
				BASS_ChannelStop(soundtrack);
			ButtonClicked();
		}
		if (index == 2 && restartToggle) {
			restartToggle = false;
			disableControl = false;
			closeMenu = true;
			oneTimeSound = true;
			numbers = numbersRestart;
			blankIndexX = 2;
			blankIndexY = 2;
			numberBoxTex = numberBoxTexRestart;
			ButtonClicked();
		}
		if (index == 3 && closeMenu) {
			openMenu = false;
			disableControl = false;
			blackBackgound = false;
			closeMenu = false;
			oneTime = true;
			ButtonClicked();
		}
	}
}
void Game::BlackBackground() {
	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(960.0f, 540.0f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform4fv(uniColor, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.60f)));
	glUniform1i(toggle, 1);
	obj.RenderMesh();
	glUniform1i(toggle, 0);
}
void Game::ClickedButton(glm::mat4 model) {
	model = glm::scale(model, glm::vec3(0.89f, 0.89f, 0.0f));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform4fv(uniColor, 1, glm::value_ptr(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)));
	glUniform1i(toggle, 2);	
	obj.RenderMesh();
	glUniform1i(toggle, 0);
}
void Game::ButtonClicked() {
	if (sound) {
		BASS_ChannelSetPosition(buttonClicked, 0, BASS_POS_BYTE);
		BASS_ChannelPlay(buttonClicked, FALSE);
	}
}
Game::~Game() {
	BASS_StreamFree(buttonClicked);
	BASS_StreamFree(winningSound);
	BASS_StreamFree(slideWood);
	BASS_StreamFree(soundtrack);
	for (int i = 0; i < 3; i++) {
		for (int j = 0;j < 3;j++) {
			delete numberBoxTex[i][j];
		}
	}
}
