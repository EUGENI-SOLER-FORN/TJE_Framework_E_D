#pragma once
#include "framework/entities/entitycollider.h"
#include "framework/utils.h"
#include "graphics/material.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include <fstream>
#include <iostream>

class World {

public:

	World(const char* sceneFile);
	
	Entity* root;
	static EntityMesh* sky;
	
	bool parseScene(const char* filename, Entity* root);
	void loadSky();

	void update(float seconds_elapsed) { World::sky->update(seconds_elapsed); this->root->update(seconds_elapsed); };
	void render(Camera* camera);

	void ray_cast() {};

};