#pragma once
#include "framework/utils.h"
#include "graphics/material.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "framework/entities/entitycollider.h"
#include <fstream>
#include <iostream>

class World {
public:

	World(const char* sceneFile);
	
	static const int days_length = 200;
	Entity* root;
	EntityDrop* boat = nullptr;
	static EntityMesh* sky;
	static Texture* day_sky;
	static Texture* night_sky;

	static void changeSky(float s) {
		World::sky->material.diffuse = night_sky;
		if (25.f < s && s < 75.f) {
			World::sky->material.diffuse = day_sky;
		}
	};

	bool parseScene(const char* filename, Entity* root);
	void loadSky();

	void update(float seconds_elapsed) { World::sky->update(seconds_elapsed); this->root->update(seconds_elapsed); };
	void render(Camera* camera);

	//void ray_cast() {};

};