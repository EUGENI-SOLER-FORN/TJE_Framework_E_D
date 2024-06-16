#pragma once
#include "stage.h"
#include "game/game.h"

PlayStage* PlayStage::current_stage = nullptr;
EntityPlayer* PlayStage::player = nullptr;
StageManager* StageManager::instance = nullptr;

StageManager::StageManager() {

	StageManager::instance = this;

	this->stages["video_intro"] = new VideoStage();
	this->stages["menu_principal"] = new MenuStage();
	this->stages["island_scene"] = new PlayStage("data/scenes/island_scene.scene");
	this->stages["game_over"] = new GameOverStage();

	this->current = this->stages["video_intro"];
	this->goTo(this->stages["video_intro"]);

	PlayStage::current_stage = (PlayStage*)this->stages["island_scene"];
	PlayStage::player = new EntityPlayer();
}

void StageManager::update(float seconds_elapsed) { 
	this->current->update(seconds_elapsed); 
	if (this->current == this->stages["video_intro"] || (this->current->go_menu == true)) {
		if ((this->current->go_to_next_stage == true) || (this->current->go_menu == true)) {
			this->current->go_to_next_stage = false;
			this->current->go_menu = false;
			this->current = this->stages["menu_principal"];
			this->goTo(this->stages["menu_principal"]);
		}
	}
	else if (this->current == this->stages["menu_principal"]) {
		if (this->current->go_to_next_stage == true) {
			this->current->go_to_next_stage = false;
			this->current = this->stages["island_scene"];
			this->goTo(this->stages["island_scene"]);
		}
	}
};

StageManager::~StageManager(){
	for (auto s : this->stages) { delete s.second; }
}

VideoStage::VideoStage() {
	this->stageCamera = new Camera();
	this->stageCamera->view_matrix = Matrix44();
	this->stageCamera->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);

	Vector2 position = Vector2(0.5f * Game::instance->window_width, 0.5f * Game::instance->window_height);
	Vector2 size = Vector2((float)Game::instance->window_width, (float)Game::instance->window_height);

	for (int i = 4; i < 10; i+=4) {
		Material mat;
		std::string frame_path = "data/video/frames_png/frame_000" + std::to_string(i) + ".png"; 
		mat.diffuse = Texture::Get(frame_path.c_str());
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		video_frames.push_back(new EntityUI(position, size, mat));
	}
	for (int i = 12; i < 100; i+=4) {
		Material mat;
		std::string frame_path = "data/video/frames_png/frame_00" + std::to_string(i) + ".png";
		mat.diffuse = Texture::Get(frame_path.c_str());
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		video_frames.push_back(new EntityUI(position, size, mat));
	}
	for (int i = 100; i < 1000; i+=4) {
		Material mat;
		std::string frame_path = "data/video/frames_png/frame_0" + std::to_string(i) + ".png";
		mat.diffuse = Texture::Get(frame_path.c_str());
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		video_frames.push_back(new EntityUI(position, size, mat));
	}
	for (int i = 1000; i <= num_frames; i+=4) {
		Material mat;
		std::string frame_path = "data/video/frames_png/frame_" + std::to_string(i) + ".png";
		mat.diffuse = Texture::Get(frame_path.c_str());
		mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		video_frames.push_back(new EntityUI(position, size, mat));
	}
}

void VideoStage::render(Camera* camera) {
	video_frames[current_frame]->render(camera);
}

void VideoStage::update(float seconds_elapsed) {
	// Avanzar solo si no se ha terminado el video
	if (!go_to_next_stage){
		// Calcula el tiempo transcurrido desde el último frame
		static float accumulated_time = 0.0f;
		accumulated_time += seconds_elapsed;

		// Define el tiempo necesario para avanzar un frame
		const float frame_duration = 0.133333f; // Suponiendo que cada frame debe durar 0.04 segundos (25 fps)

		while (accumulated_time >= frame_duration) {
			// Avanza al siguiente frame
			if (current_frame < (num_frames / 4) - 1) {
				current_frame++;
			}
			else {
				go_to_next_stage = true;
				video_audio->Stop(video_sound);
			}
			accumulated_time -= frame_duration; // Ajusta el tiempo acumulado
		}
	}
}

void VideoStage::onEnter(Stage* prev_stage) {
	video_audio->Init();
	video_audio->Get("data/audio/intro_audio.wav");
	video_sound = video_audio->Play("data/audio/intro_audio.wav", 0.5, true);
}

void VideoStage::onLeave(Stage* next_stage) {
	// video_audio->Stop(video_sound);
}

VideoStage::~VideoStage() {
	for (auto frame : video_frames) {
		delete frame;
	}
	video_frames.clear();
}

MenuStage::MenuStage() {
	this->stageCamera = new Camera();
	this->stageCamera->view_matrix = Matrix44();
	this->stageCamera->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);

	float width = (float)Game::instance->window_width;
	float height = (float)Game::instance->window_height;
	Vector2 screen_center = Vector2(0.5f * width, 0.5f * height);

	Material background_material;
	background_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	background_material.diffuse = Texture::Get("data/textures/menu/menu_background.tga");
	background = new EntityUI(screen_center, Vector2(width, height), background_material);
	background->setType(BACKGROUND);

	Material play_material;
	play_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	play_material.diffuse = Texture::Get("data/textures/menu/play.tga");
	playbutton = new EntityUI(Vector2(250.0f, 70.0f), Vector2(200.0f, 80.0f), play_material);
	playbutton->setHoverTexture(Texture::Get("data/textures/menu/play2.tga"));
	playbutton->setType(PLAY_BUTTON);

	Material exit_material;
	exit_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	exit_material.diffuse = Texture::Get("data/textures/menu/exit.tga");
	exitbutton = new EntityUI(Vector2(550.0f, 70.0f), Vector2(200.0f, 80.0f), exit_material);
	exitbutton->setHoverTexture(Texture::Get("data/textures/menu/exit2.tga"));
	exitbutton->setType(EXIT_BUTTON);
}

void MenuStage::render(Camera* camera) {
	playbutton->render(camera);
	exitbutton->render(camera);
	background->render(camera);
}

void MenuStage::update(float seconds_elapsed) {
	playbutton->update(seconds_elapsed);
	exitbutton->update(seconds_elapsed);
	background->update(seconds_elapsed);

	if (playbutton->isHover() && Input::isMousePressed(SDL_BUTTON_LEFT)) {
		onButtonPressed(playbutton->type);
	}
	else if (exitbutton->isHover() && Input::isMousePressed(SDL_BUTTON_LEFT)) {
		onButtonPressed(exitbutton->type);
	}
}

void MenuStage::onButtonPressed(eButtonId buttonID) {
	switch (buttonID) {
	case PLAY_BUTTON:
		go_to_next_stage = true;
		menu_audio->Stop(menu_sound);
		break;
	case EXIT_BUTTON:
		exit(0);
		break;
	default:
		break;
	}
}

void MenuStage::onEnter(Stage* prev_stage) {
	menu_audio->Init();
	menu_audio->Get("data/audio/menu_audio.wav");
	menu_sound = menu_audio->Play("data/audio/menu_audio.wav", 0.3, true);
}

void MenuStage::onLeave(Stage* next_stage) {
	// menu_audio->Stop(menu_sound);
}

MenuStage::~MenuStage(){
	delete background;
	delete playbutton;
	delete exitbutton;
}

GameOverStage::GameOverStage() : Stage() {
		this->stageCamera = new Camera();
		this->stageCamera->view_matrix = Matrix44();
		this->stageCamera->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);
		float width = (float)Game::instance->window_width;
		float height = (float)Game::instance->window_height;
		Vector2 screen_center = Vector2(0.5f * width, 0.5f * height);
		Material bckg_material;
		bckg_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		bckg_material.diffuse = Texture::Get("data/textures/menu/gameover_loose_background.png");
		this->background = new EntityUI(Vector2(width/2.f, height/2.f), Vector2(width, height), bckg_material);
		this->background->setHoverTexture(Texture::Get("data/textures/menu/gameover_win_background.png"));
		Material play_material;
		play_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		play_material.diffuse = Texture::Get("data/textures/menu/restart2.tga");
		restartbutton = new EntityUI(Vector2(250.0f, 70.0f), Vector2(200.0f, 80.0f), play_material);
		restartbutton->setHoverTexture(Texture::Get("data/textures/menu/restart1.tga"));
		restartbutton->setType(RESET_BUTTON);
		Material exit_material;
		exit_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		exit_material.diffuse = Texture::Get("data/textures/menu/exit.tga");
		exitbutton = new EntityUI(Vector2(550.0f, 70.0f), Vector2(200.0f, 80.0f), exit_material);
		exitbutton->setHoverTexture(Texture::Get("data/textures/menu/exit2.tga"));
		exitbutton->setType(EXIT_BUTTON);

		for (int i = 0; 10 > i; i++) {
			std::string num_path = "data/textures/menu/" + std::to_string(i) + ".png";
			this->numbers.push_back(Texture::Get(num_path.c_str()));
		}

		Material digit_material;
		digit_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
		digit_material.diffuse = this->numbers[0];
		this->score_digit_1 = new EntityUI(Vector2(width-150, height/2.f - 110.f), Vector2(75.0f), digit_material);
		this->score_digit_2 = new EntityUI(Vector2(width-75, height / 2.f - 110.f), Vector2(75.0f), digit_material);
}

PlayStage::PlayStage(const char* sceneFile) {
	this->scene = new World(sceneFile);
	this->stageCamera = new Camera();

	int height = Game::instance->window_height;
	int width = Game::instance->window_width;
	Vector3 camera_eye = Vector3(1.0f);
	Vector3 camera_center = Vector3(0.0f);

	this->stageCamera->lookAt(camera_eye, camera_center, Vector3::UP);
	this->stageCamera->setPerspective(60.f, width / (float)height, 0.01f, 1000.f); 
}

void PlayStage::onEnter(Stage* prev_stage) { 
	Game::instance->time = 50.f; 
	Game::instance->mouse_locked = true; 
	SDL_ShowCursor(!Game::instance->mouse_locked); 
}

void PlayStage::onLeave(Stage* prev_stage) { 
	Game::instance->mouse_locked = false; 
	SDL_ShowCursor(!Game::instance->mouse_locked);
}

void PlayStage::updateSceneCamera(float seconds_elapsed){
	// Code seen in theory class
	// Get yaw and pitch (rotations in Z and Y axis) from mouse input and camera speed
	this->camera_yaw -= Input::mouse_delta.x * this->camera_speed * seconds_elapsed;
	this->camera_pitch -= Input::mouse_delta.y * this->camera_speed * seconds_elapsed;
	// clamp pitch so you don't do a full rotation
	this->camera_pitch = clamp(this->camera_pitch, -(float)M_PI * 0.4f, (float)M_PI * 0.4f);
	// Create rotation matrice sand combine with product
	Matrix44 matrix_yaw, matrix_pitch;
	matrix_yaw.setRotation(this->camera_yaw, Vector3::UP);
	matrix_pitch.setRotation(this->camera_pitch, Vector3(-1, 0, 0));
	const Vector3& new_front = (matrix_pitch * matrix_yaw).frontVector();

	const Vector3& new_eye = PlayStage::player->position() + PlayStage::player->player_height;
	const Vector3& new_center = new_eye + new_front;

	this->stageCamera->lookAt(new_eye, new_center, Vector3::UP);
	
	//move sky
	this->scene->sky->model.setTranslation(new_eye);
}

void PlayStage::render(Camera* camera){
	if (PlayStage::player->sleep_cooldown) {
		PlayStage::player->render(camera);
		return;
	}
	this->scene->render(camera); 
	PlayStage::player->render(camera);
}

void PlayStage::update(float seconds_elapsed){
	PlayStage::player->update(seconds_elapsed);

	if (PlayStage::player->sleep_cooldown) return;
	if (!PlayStage::player->menu_game->display_menu) {
		this->scene->update(seconds_elapsed);
		this->updateSceneCamera(seconds_elapsed);
	}

	if (PlayStage::player->menu_game->go_menu == true) {
		go_menu = true;
		if (PlayStage::player->timebar->is_night_audio) {
			PlayStage::player->timebar->night_audio->Stop(PlayStage::player->timebar->night_channel);
			PlayStage::player->timebar->is_night_audio = false;
		} 
		if (PlayStage::player->timebar->is_day_audio) {
			PlayStage::player->timebar->day_audio->Stop(PlayStage::player->timebar->day_channel);
			PlayStage::player->timebar->is_day_audio = false;
		}
	}
	PlayStage::player->menu_game->go_menu = false;
}

void GameOverStage::setBackground(int score){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (this->background == nullptr) return;
	
	this->background->regularTexture = Texture::Get("data/textures/menu/gameover_win_background.png");
	this->win = true;
	this->restartbutton->position = Vector2(250.f, 70.f);
	this->exitbutton->position = Vector2(550.f, 70.f);
	
	if (score == 30) {
		this->background->regularTexture = Texture::Get("data/textures/menu/gameover_loose_background.png");
		this->score_digit_1->changeVisibility();
		this->score_digit_2->changeVisibility();
		this->restartbutton->position.y += 100.f;
		this->exitbutton->position.y += 100.f;
		this->win = false;
	}

	this->restartbutton->updateMesh();
	this->exitbutton->updateMesh();

	this->score_digit_1->regularTexture = this->numbers[score / 10];
	this->score_digit_2->regularTexture = this->numbers[score % 10];
}

void GameOverStage::onButtonPressed(eButtonId buttonID) {
	switch (buttonID) {
	case EXIT_BUTTON:
		exit(0);
		break;
	case RESET_BUTTON:
		PlayStage::current_stage->reset();
		StageManager::goTo("island_scene");
	default:
		break;
	}
}

void GameOverStage::update(float seconds_elapsed) {
	this->restartbutton->update(seconds_elapsed);
	this->exitbutton->update(seconds_elapsed);
	this->score_digit_1->update(seconds_elapsed);
	this->score_digit_2->update(seconds_elapsed);
	this->background->update(seconds_elapsed);
	if (this->restartbutton->isHover() && Input::isMousePressed(SDL_BUTTON_LEFT)) {
		onButtonPressed(this->restartbutton->type);
	}
	else if (this->exitbutton->isHover() && Input::isMousePressed(SDL_BUTTON_LEFT)) {
		onButtonPressed(this->exitbutton->type);
	}
}
