#pragma once
#include "framework/input.h"
#include "framework/entities/entityui.h"
#include "framework/entities/entitycollider.h"

class EntityPlayer : public EntityMesh {
public:
	// constructor methods
	EntityPlayer();
	~EntityPlayer() {};
	
	// general attributes
	float player_height = 1.0f;
	float sphere_radius = .5f;

	// stats of the player
	float player_speed = 0.01f;
	float player_hunger = 100.0f;
	float player_sleepiness = 100.0f;
	EntityUI* point;
	Camera* inventory_camera;
	Inventory* inventory;

	// methods to manage hunger and sleep
	void eat() { this->player_hunger = std::min(100.0f, this->player_hunger + 20.0f); }
	void sleep() { this->player_sleepiness = std::min(100.0f, this->player_sleepiness + 20.0f); }

	// update and render methods
	void update(float seconds_elapsed) override;
	void render(Camera* camera) override;
};