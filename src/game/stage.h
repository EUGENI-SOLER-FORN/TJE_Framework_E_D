#pragma once
#include "framework/entities/entityplayer.h"
#include "framework/input.h"
#include "game/world.h"
#include <map>
#include <string>

class Stage
{
public:
	Stage() {};
	~Stage() { delete this->stageCamera; };

	virtual void onEnter(Stage* prev_stage) {};
	virtual void onLeave(Stage* next_stage) {};

	Camera* stageCamera;
	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
	float camera_speed = 10.f;

	virtual void render(Camera* camera) {};
	virtual void update(float seconds_elapsed) {};
};

class StageManager
{
public:
	Stage* current;
	StageManager();
	~StageManager();

	std::map<std::string, Stage*> stages;

	void goTo(const std::string stageName) { Stage* next_stage = this->stages[stageName]; this->goTo(next_stage); };
	void goTo(Stage* next_stage) { if (next_stage == nullptr) return; next_stage->onEnter(this->current); this->current->onLeave(next_stage); this->current = next_stage; };

	void render() { this->current->render(this->current->stageCamera); };
	void update(float seconds_elapsed) { this->current->update(seconds_elapsed); };
};

class MenuStage : public Stage
{
public:
	MenuStage();
	~MenuStage();

	EntityUI* background;
	EntityUI* startbutton;

	void onButtonPressed(eButtonId buttonID) {};
	void render(Camera* camera) override { this->background->render(camera); };
};

class PlayStage : public Stage
{
public:
	PlayStage(const char* sceneFile);
	~PlayStage() { delete this->scene; };

	static PlayStage* current_stage;
	static EntityPlayer* player;
	
	World* scene;
	void updateSceneCamera(float seconds_elapsed);

	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};