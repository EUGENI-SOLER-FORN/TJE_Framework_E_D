#pragma once
#include "game/world.h"
#include "framework/entities/entityplayer.h"
#include "framework/input.h"
#include "framework/audio.h"
#include <map>
#include <vector>
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
	float camera_speed = 1.5f;
	bool go_to_next_stage = false;
	bool go_menu = false;

	virtual void render(Camera* camera) {};
	virtual void update(float seconds_elapsed) {};
	virtual void setBackground(int d) {};
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

class MenuStage : public Stage {
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
class GameOverStage : public Stage {
public:
	GameOverStage();
	EntityUI* background;
	EntityUI* restartbutton;
	EntityUI* exitbutton;

	EntityUI* score_digit_1 = nullptr;
	EntityUI* score_digit_2 = nullptr;
	std::vector<Texture*> numbers;
	
	bool win = false;
	
	void setBackground(int score) override;
	void onButtonPressed(eButtonId buttonID);

	void update(float seconds_elapsed) override;
	void render(Camera* camera) override { 
		this->restartbutton->render(camera);
		this->exitbutton->render(camera);
		
		if (this->win) this->score_digit_1->render(camera);
		if (this->win) this->score_digit_2->render(camera);

		this->background->render(camera);
	};

	~GameOverStage() { 
		delete score_digit_1; 
		delete score_digit_2;
		delete background;
		delete restartbutton;
		delete exitbutton;
	};
};

class PlayStage : public Stage {
public:
	PlayStage(const char* sceneFile);
	~PlayStage() { delete this->scene; };

	static PlayStage* current_stage;
	static EntityPlayer* player;
	
	World* scene;
	void updateSceneCamera(float seconds_elapsed);

	void onEnter(Stage* prev_stage) override;
	void onLeave(Stage* prev_stage) override;
	void render(Camera* camera) override;
	void reset() {
		PlayStage::player->days_counter = 0;
		PlayStage::player->model.setTranslation(PlayStage::player->player_height + Vector3(34.5f, 0.5f, 7.5f));
		for (int i = 0; PlayStage::player->inventory->elements.size() > i; i++) PlayStage::player->inventory->elements[i] = 0;
		PlayStage::player->player_hunger = 100.f;
		PlayStage::player->player_sleepiness = 100.f;
		for (Entity* e : this->scene->root->children) {
			EntityDrop* entity_drop = dynamic_cast<EntityDrop*>(e);
			if (!entity_drop) continue;
			if (entity_drop->type == BOAT) entity_drop->health = 0.1f;
			else if (entity_drop->type == TREE) entity_drop->health = 100.f;
		}
	}
	void update(float seconds_elapsed) override;
};