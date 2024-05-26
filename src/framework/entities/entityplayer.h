#pragma once
#include "entitymesh.h"
#include "framework/input.h"

class EntityPlayer : public EntityMesh {
public:

	// constructor methods
	EntityPlayer() : EntityMesh() { this->name = "player"; this->model = Matrix44::IDENTITY; };
	~EntityPlayer() {};
	
	// general attributes
	float player_height = 1.0f;
	float sphere_radius = 1.0f;

	// stats of the player
	float player_speed = 0.01f;
	float player_hunger = 100.0f;
	float player_sleepiness = 100.0f;
	
	// methods to manage hunger and sleep
	void hunger() { this->player_hunger = std::min(100.0f, this->player_hunger + 50.0f); }
	void sleep() { this->player_sleepiness = std::min(100.0f, this->player_sleepiness + 50.0f); }

	// update and render methods
	void update(float seconds_elapsed);
	void render(Camera* camera);
};