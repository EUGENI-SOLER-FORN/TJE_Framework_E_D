#pragma once
#include "stage.h"
#include "game/game.h"

PlayStage* PlayStage::current_stage = nullptr;
EntityPlayer* PlayStage::player = nullptr;

StageManager::StageManager()
{
	this->stages["menu_principal"] = new MenuStage();
	this->stages["beach_scene"] = new PlayStage("data/scenes/beach_scene.scene");
	this->stages["cross_scene"] = new PlayStage("data/scenes/cross_scene.scene");
	this->stages["island_scene"] = new PlayStage("data/scenes/island_scene.scene");

	this->current = this->stages["menu_principal"];
	this->goTo("menu_principal");

	PlayStage::current_stage = (PlayStage*)StageManager::current;
	PlayStage::player = new EntityPlayer();
}

StageManager::~StageManager(){
	for (auto s : this->stages) { delete s.second; }
}

MenuStage::MenuStage(){
	Material background_material;
	background_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_material.diffuse = Texture::Get("data/textures/background.png");
	this->background = new EntityUI(Vector2(0.f,0.f) ,Vector2((float)Game::instance->window_width, (float)Game::instance->window_height), background_material);
	this->background->setType(BACKGROUND);
}

void MenuStage::update(float seconds_elapsed) {
	// donar sentit, veure si el mouse està dins del rectangle, donar color i veure si es clica dins 
}

PlayStage::PlayStage(const char* sceneFile)
{
//	PlayStage::current_stage = this;

	this->scene = new World(sceneFile);
	this->stageCamera = new Camera();

	int height = Game::instance->window_height;
	int width = Game::instance->window_width;
	Vector3 camera_eye = Vector3(1.0f);
	Vector3 camera_center = Vector3(0.0f);

	this->stageCamera->lookAt(camera_eye, camera_center, Vector3::UP);
	this->stageCamera->setPerspective(60.f, width / (float)height, 0.01f, 1000.f); 
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
}

void PlayStage::render(Camera* camera){
	this->scene->render(camera); 
	PlayStage::player->render(camera);
}

void PlayStage::update(float seconds_elapsed){
	this->scene->update(seconds_elapsed); 
	PlayStage::player->update(seconds_elapsed); 
	this->updateSceneCamera(seconds_elapsed);
}
