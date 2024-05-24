#pragma once
#include "entitymesh.h"
#include "framework/input.h"

class EntityPlayer : public EntityMesh {
public:
	EntityPlayer() : EntityMesh() { this->name = "player"; this->model = Matrix44::IDENTITY; };
	~EntityPlayer() {};
	
	// stats of the player
	float player_speed = 0.01f;
	float player_hunger = 0.f;
	float player_sleepiness = 0.f;
	
	// helper method
	Vector3 position() { return this->model.getTranslation(); };

	void update(float seconds_elapsed) override;
	// TODO
	void render(Camera* camera) override {};
};