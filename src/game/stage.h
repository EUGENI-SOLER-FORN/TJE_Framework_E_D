#pragma once
//#include "framework/entities/entityui.h"
#include "framework/entities/entityplayer.h"
#include "world.h"
#include <map>
#include <string>

class EntityUI;
enum eButtonID;

class Stage
{
public:
	Stage() {};
	~Stage() {};

	virtual void onEnter(Stage* prev_stage) {};
	virtual void onLeave(Stage* next_stage) {};

	Camera* stageCamera;

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

	static Camera camera2D;

	EntityUI* background;
	EntityUI* startbutton;

	void onButtonPressed(eButtonID buttonID) {};
};

class PlayStage : public Stage
{
public:
	PlayStage(const char* sceneFile);
	~PlayStage();

	static EntityPlayer* player;

	World* scene;

	void render(Camera* camera) { this->scene->render(camera); };// PlayStage::player->render(camera); };
	void update(float seconds_elapsed) { this->scene->update(seconds_elapsed); };// PlayStage::player->update(seconds_elapsed); };
};