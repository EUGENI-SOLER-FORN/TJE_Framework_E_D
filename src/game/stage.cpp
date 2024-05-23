#pragma once
#include "stage.h"
#include "game/game.h"

StageManager::StageManager()
{
	this->stages["menu_principal"] = new MenuStage();
	this->stages["beach_scene"] = new PlayStage("data/scenes/beach_scene.scene");
	this->stages["cross_scene"] = new PlayStage("data/scenes/cross_scene.scene");
	this->current = this->stages["beach_scene"];
	this->goTo("beach_scene");
}

StageManager::~StageManager()
{
}

MenuStage::MenuStage()
{
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

PlayStage::~PlayStage()
{
	delete this->scene;
}
