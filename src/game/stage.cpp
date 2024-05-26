#pragma once
#include "stage.h"
#include "game/game.h"

EntityPlayer* PlayStage::player = nullptr;

StageManager::StageManager()
{
	this->stages["menu_principal"] = new MenuStage();
	this->stages["beach_scene"] = new PlayStage("data/scenes/beach_scene.scene");
	this->stages["cross_scene"] = new PlayStage("data/scenes/cross_scene.scene");
	this->current = this->stages["beach_scene"];
	this->goTo("beach_scene");
	
	PlayStage::player = new EntityPlayer();
}

StageManager::~StageManager()
{
	for (auto s : this->stages) { delete s.second; }
}

MenuStage::MenuStage(){
	Material background_material;
	background_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	background_material.diffuse = Texture::Get("data/textures/background.png");
	this->background = new EntityUI(Vector2(0.f,0.f) ,Vector2(Game::instance->window_width, Game::instance->window_height), background_material);
	this->background->setType(BACKGROUND);
}

MenuStage::~MenuStage()
{

}

PlayStage::PlayStage(const char* sceneFile)
{
	this->scene = new World(sceneFile);

	int height = Game::instance->window_height;
	int width = Game::instance->window_width;
	this->stageCamera = new Camera();
	Vector3 camera_eye = Vector3(1.0f);
	Vector3 camera_center = Vector3(0.0f);
	this->stageCamera->lookAt(camera_eye, camera_center, Vector3::UP);
	this->stageCamera->setPerspective(60.f, width / (float)height, 0.01f, 1000.f); 
}

void PlayStage::updateSceneCamera(float seconds_elapsed){
	/*
	if (Input::isMousePressed(SDL_BUTTON_LEFT) || Game::instance->mouse_locked)
	{
		this->stageCamera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		this->stageCamera->rotate(Input::mouse_delta.y * 0.005f, this->stageCamera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}
	*/
	
	// Code seen in theory class
	// Get yaw and pitch (rotations in Z and Y axis) from mouse input and camera speed
	this->camera_yaw -= Input::mouse_delta.x * this->camera_speed * seconds_elapsed;
	this->camera_pitch -= Input::mouse_delta.y * this->camera_speed * seconds_elapsed;
	// clamp pitch so you don't do a full rotation
	this->camera_pitch = clamp(this->camera_pitch, -M_PI * 0.4f, M_PI * 0.4f);
	// Create rotation matrice sand combine with product
	Matrix44 matrix_yaw, matrix_pitch;
	matrix_yaw.setRotation(this->camera_yaw, Vector3::UP);
	matrix_pitch.setRotation(this->camera_pitch, Vector3(-1, 0, 0));
	const Vector3& new_front = (matrix_pitch * matrix_yaw).frontVector();

	const Vector3& player_offset = Vector3(0.f);	
	const Vector3& new_eye = PlayStage::player->position + player_offset - new_front / new_front.length() * 2.0f;;
	const Vector3& new_center = new_eye + new_front;

	this->stageCamera->lookAt(new_eye, new_center, Vector3::UP);
	this->player->model.setRotation(camera_yaw, Vector3::UP);
}
