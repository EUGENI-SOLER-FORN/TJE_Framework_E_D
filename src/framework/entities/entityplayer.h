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
		delete stamina;
		delete hunger;
		delete timebar;
	};
	
	// general attributes
	const Vector3 player_height = Vector3(0.f, .75f, 0.f);
	const float sphere_radius = .25f;

	// stats of the player
	const float player_speed = 2.f;
	Vector3 velocity;
	float player_hunger = 100.f;
	float player_sleepiness = 100.f;
	int days_counter = 0;
	
	// 2D HUD and minimap
	Camera* player_camera2D;
	PointCross* point;
	Inventory* inventory;
	MiniMap* minimap;
	StatBar* stamina;
	StatBar* hunger;
	TimeBar* timebar;

	// methods to manage hunger and sleep
	void eat() { this->player_hunger = std::min(100.0f, this->player_hunger + 20.0f); };
	void sleep();
	float sleep_cooldown = 0.f;

	// For attacks
	EntityDrop* getEntityPointingAt(Camera* camera);
	EntityDrop* pointingAt = nullptr;
	
	// update and render methods
	void update(float seconds_elapsed) override;
	void render(Camera* camera) override;
};