#pragma once
#include "game/world.h"
#include "framework/entities/entityplayer.h"
#include "framework/input.h"
#include "framework/audio.h"
#include <map>
#include <string>

class Stage{
public:
	Stage() { };
	~Stage() { delete this->stageCamera; };

	virtual void onEnter(Stage* prev_stage) {};
	virtual void onLeave(Stage* next_stage) {};

	Camera* stageCamera = new Camera();
	float camera_yaw = 0.f;
	float camera_pitch = 0.f;
<<<<<<< Updated upstream
	float camera_speed = 1.5f;
=======
	float camera_speed = 3.f;
	bool go_to_next_stage = false;
>>>>>>> Stashed changes

	virtual void render(Camera* camera) {};
	virtual void update(float seconds_elapsed) {};
};

class StageManager
{
public:
	static StageManager* instance;
	Stage* current;
	StageManager();
	~StageManager();

	std::map<std::string, Stage*> stages;

	static void goTo(const std::string stageName) { 
		Stage* next_stage = StageManager::instance->stages[stageName]; 
		StageManager::goTo(next_stage); 
	};
	static void goTo(Stage* next_stage) { 
		if (!next_stage) return; 
		next_stage->onEnter(StageManager::instance->current); 
		StageManager::instance->current->onLeave(next_stage); 
		StageManager::instance->current = next_stage; 
	};
	
	void render() { this->current->render(this->current->stageCamera); };
	void update(float seconds_elapsed);
};

class VideoStage : public Stage
{
public:
	VideoStage(); 
	~VideoStage();

	Audio* video_audio;
	HCHANNEL video_sound;
	int num_frames = 1716; 
	int current_frame = 0;
	std::vector<EntityUI*> video_frames; 

	void onEnter(Stage* prev_stage) override;
	void onLeave(Stage* next_stage) override;
	void render(Camera* camera) override; 
	void update(float seconds_elapsed) override; 
};

class MenuStage : public Stage
{
public:
	MenuStage();
	~MenuStage();

	Audio* menu_audio;
	HCHANNEL menu_sound;
	EntityUI* background;
	EntityUI* playbutton;
	EntityUI* exitbutton;

	void onEnter(Stage* prev_stage) override;
	void onLeave(Stage* next_stage) override;
	void onButtonPressed(eButtonId buttonID);
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};

class PlayStage : public Stage
{
public:
	PlayStage(const char* sceneFile);
	~PlayStage() { delete this->scene; };

	Audio* play_audio;
	HCHANNEL play_sound;
	static PlayStage* current_stage;
	static EntityPlayer* player;
	
	World* scene;
	void updateSceneCamera(float seconds_elapsed);

	void onEnter(Stage* prev_stage) override;
<<<<<<< Updated upstream
	void onLeave(Stage* prev_stage) override;

=======
	void onLeave(Stage* next_stage) override;
>>>>>>> Stashed changes
	void render(Camera* camera) override;
	void update(float seconds_elapsed) override;
};