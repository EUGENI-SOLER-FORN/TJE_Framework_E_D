#pragma once
#include "framework/input.h"
#include "framework/entities/entityui.h"
#include "framework/entities/entitycollider.h"

class EntityPlayer : public EntityMesh {
public:
	// constructor methods
	EntityPlayer();
	~EntityPlayer() { 
		delete player_camera2D;
		delete point;
		delete inventory;
		delete minimap;
	};
	
	// general attributes
	const Vector3 player_height = Vector3(0.f, 1.f, 0.f);
	const float sphere_radius = .3f;

	// stats of the player
	const float player_speed = 2.f;
	Vector3 velocity;
	float player_hunger = 100.0f;
	float player_sleepiness = 100.0f;
	
	// 2D HUD and minimap
	Camera* player_camera2D;
	PointCross* point;
	Inventory* inventory;
	MiniMap* minimap;
	StatBar* stamina;
	StatBar* hunger;

	// methods to manage hunger and sleep
	void eat() { this->player_hunger = std::min(100.0f, this->player_hunger + 20.0f); }
	void sleep() { this->player_sleepiness = std::min(100.0f, this->player_sleepiness + 20.0f); }

	// For attacks
	EntityDrop* getEntityPointingAt(Camera* camera);
	EntityDrop* pointingAt = nullptr;
	// update and render methods
	void update(float seconds_elapsed) override;
	void render(Camera* camera) override;
};