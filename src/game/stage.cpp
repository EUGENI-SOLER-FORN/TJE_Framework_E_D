#pragma once
#include "stage.h"
#include "game/game.h"

PlayStage* PlayStage::current_stage = nullptr;
EntityPlayer* PlayStage::player = nullptr;
StageManager* StageManager::instance = nullptr;

StageManager::StageManager() {
	// TODO: create loading screen
	// this->stages["loading"] = new LoadingStage();
	
	this->stages["menu_principal"] = new MenuStage();
	this->stages["island_scene"] = new PlayStage("data/scenes/island_scene.scene");

	// TODO: create outro/restart stage
	// this->stages["final"] = new PlayStage("data/scenes/island_scene.scene");

	this->current = this->stages["menu_principal"];

	PlayStage::current_stage = (PlayStage*)this->stages["island_scene"];
	PlayStage::player = new EntityPlayer();
	
	StageManager::instance = this;
}

StageManager::~StageManager(){
	for (auto s : this->stages) { delete s.second; }
}

MenuStage::MenuStage(){

	camera_2D = new Camera();
	camera_2D->view_matrix = Matrix44();
	camera_2D->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);

	float width = (float)Game::instance->window_width;
	float height = (float)Game::instance->window_height;

	Material background_material;
	background_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	background_material.diffuse = Texture::Get("data/textures/menu/background.png");
	background = new EntityUI(Vector2(0.5f * width, 0.5f * height), Vector2(width, height), background_material);

	Material play_material;
	play_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	play_material.diffuse = Texture::Get("data/textures/menu/play.png");
	playbutton = new EntityUI(Vector2(10.0f, 0.5f * height), Vector2(30.0f), play_material);

	Material exit_material;
	exit_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture2D.fs");
	exit_material.diffuse = Texture::Get("data/textures/menu/exit.png");
	exitbutton = new EntityUI(Vector2(20.0f, 0.5f * height), Vector2(30.0f), exit_material);

	background->addChild(playbutton);
	background->addChild(exitbutton);

	this->stageCamera = new Camera();
	this->stageCamera->view_matrix = Matrix44();
	this->stageCamera->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);
}

void MenuStage::render(Camera* camera) {
	playbutton->render(camera);
	exitbutton->render(camera);
	background->render(camera);
}

void MenuStage::update(float seconds_elapsed) {
	// TODO: give sense, see if the mouse is inside rectangle, give color, see if it is clicked inside (onButtonPressed)
	// provisional
	if (Input::isKeyPressed(SDL_SCANCODE_TAB)) StageManager::instance->goTo("island_scene");
}

MenuStage::~MenuStage(){
	delete background;
	delete playbutton;
	delete exitbutton;
}

PlayStage::PlayStage(const char* sceneFile)
{
	this->scene = new World(sceneFile);
	this->stageCamera = new Camera();

	int height = Game::instance->window_height;
	int width = Game::instance->window_width;
	Vector3 camera_eye = Vector3(1.0f);
	Vector3 camera_center = Vector3(0.0f);

	this->stageCamera->lookAt(camera_eye, camera_center, Vector3::UP);
	this->stageCamera->setPerspective(60.f, width / (float)height, 0.01f, 1000.f); 
}
void PlayStage::onEnter(Stage* prev_stage) { Game::instance->time = 50.f; Game::instance->mouse_locked = true; }

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
	if (PlayStage::player->sleep_cooldown) {
		PlayStage::player->update(seconds_elapsed);
		return;
	}
	this->scene->update(seconds_elapsed);
	PlayStage::player->update(seconds_elapsed); 
	this->updateSceneCamera(seconds_elapsed);
}
