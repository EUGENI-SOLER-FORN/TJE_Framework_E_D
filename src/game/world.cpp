#pragma once
#include "world.h"
#include "framework/entities/entitycollider.h"

EntityMesh* World::sky = nullptr;
Texture* World::day_sky = nullptr;
Texture* World::night_sky = nullptr;

World::World(const char* sceneFile) {
	this->root = new Entity();
	this->parseScene(sceneFile, this->root);
	if (!World::sky) this->loadSky();
}

// Code taken from slides 10 - World Representation
void World::loadSky(){
	Material sky_material;
	sky_material.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/cubemap.fs");
	World::day_sky = new Texture();
	day_sky->loadCubemap("day", {
		"data/textures/skybox/day_left.png",
		"data/textures/skybox/day_right.png",
		"data/textures/skybox/day_bottom.png",
		"data/textures/skybox/day_up.png",
		"data/textures/skybox/day_front.png",
		"data/textures/skybox/day_back.png"
		});
	World::night_sky = new Texture();
	night_sky->loadCubemap("night", {
		"data/textures/skybox/night_left.png",
		"data/textures/skybox/night_right.png",
		"data/textures/skybox/night_bottom.png",
		"data/textures/skybox/night_up.png",
		"data/textures/skybox/night_front.png",
		"data/textures/skybox/night_back.png"
		});
	sky_material.diffuse = day_sky;
	World::sky = new EntityMesh(Mesh::Get("data/meshes/box.ASE"), sky_material, "sky");
}
// Code taken from AG:
bool World::parseScene(const char* filename, Entity* root)
{
	std::cout << " + Scene loading: " << filename << "..." << std::endl;
	
	// define variable meshes to load
	std::map<std::string, sRenderData> meshes_to_load;

	std::ifstream file(filename);

	if (!file.good()) {
		std::cerr << "Scene [ERROR]" << " File not found!" << std::endl;
		return false;
	}

	std::string scene_info, mesh_name, model_data;
	file >> scene_info; file >> scene_info;
	int mesh_count = 0;

	// Read file line by line and store mesh path and model info in separated variables
	while (file >> mesh_name >> model_data)
	{
		if (mesh_name[0] == '#')
			continue;

		// Get all 16 matrix floats
		std::vector<std::string> tokens = tokenize(model_data, ",");

		// Fill matrix converting chars to floats
		Matrix44 model;
		for (int t = 0; t < tokens.size(); ++t) {
			model.m[t] = (float)atof(tokens[t].c_str());
		}

		// Add model to mesh list (might be instanced!)
		sRenderData& render_data = meshes_to_load[mesh_name];
		render_data.models.push_back(model);
		mesh_count++;
	}

	// Iterate through meshes loaded and create corresponding entities
	for (auto data : meshes_to_load) {

		mesh_name = "data/scenes/" + data.first;
		sRenderData& render_data = data.second;
		
		// No transforms, nothing to do here
		if (render_data.models.empty())
			continue;


		Material mat = render_data.material;
		EntityMesh* new_entity = nullptr;

		size_t tree_tag = data.first.find("@tree");
		size_t house_tag = data.first.find("@house");
		size_t boat_tag = data.first.find("@boat");

		if (tree_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			// Create a different type of entity
			mat.diffuse = Texture::Get("data/textures/tree_texture.png");
			mat.color = mat.diffuse ? Vector4(0.9f, 0.9f, 0.9f, 1.0f) : Vector4(0.0f);
			mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			new_entity = new EntityDrop(mesh, mat);
			new_entity->name = "Tree";
			new_entity->type = TREE;
			static_cast<EntityDrop*>(new_entity)->healthbar->setTreeIcon();
			
		}
		else if (house_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			// Create a different type of entity
			mat.diffuse = Texture::Get("data/textures/colormap.png");
			mat.color = mat.diffuse ? Vector4(0.9f, 0.9f, 0.9f, 1.0f) : Vector4(0.0f);
			mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			new_entity = new EntityDrop(mesh, mat);
			new_entity->name = "House";
			new_entity->type = HOUSE;
		} 
		else if (boat_tag != std::string::npos) {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			// Create a different type of entity
			mat.diffuse = Texture::Get("data/textures/colormap.png");
			mat.color = mat.diffuse ? Vector4(0.9f, 0.9f, 0.9f, 1.0f) : Vector4(0.0f);
			mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			new_entity = new EntityDrop(mesh, mat);
			new_entity->name = "Boat";
			new_entity->type = BOAT;
			static_cast<EntityDrop*>(new_entity)->healthbar->setBoatIcon();
			static_cast<EntityDrop*>(new_entity)->health = 0.01f;
			this->boat = static_cast<EntityDrop*>(new_entity);
		}
		else {
			Mesh* mesh = Mesh::Get(mesh_name.c_str());
			mat.diffuse = Texture::Get("data/textures/colormap.png");
			mat.color = mat.diffuse ? Vector4(0.9f, 0.9f, 0.9f, 1.0f) : Vector4(0.0f);
			mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			new_entity = new EntityCollider(mesh, mat);
		}

		if (!new_entity) {
			continue;
		}

		// Create instanced entity
		if (render_data.models.size() > 1) {
			new_entity->isInstanced = true;
			new_entity->material.shader = Shader::Get("data/shaders/instanced.vs", new_entity->material.diffuse ? "data/shaders/texture.fs" : "data/shaders/flat.fs");
			new_entity->models = render_data.models; // Add all instances
		}
		// Create normal entity
		else {
			new_entity->model = render_data.models[0];
		}

		// Add entity to scene root
		root->addChild(new_entity);
	}

	std::cout << "Scene [OK]" << " Meshes added: " << mesh_count << std::endl;
	return true;
}

void World::render(Camera* camera) {
	glDisable(GL_DEPTH_TEST);
	World::sky->render(camera);
	glEnable(GL_DEPTH_TEST);

	this->root->render(camera);
}