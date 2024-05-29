#pragma once
#include "entityplayer.h"
#include "game/game.h"

EntityPlayer::EntityPlayer() : EntityMesh(){
	this->name = "player";
	this->model = Matrix44::IDENTITY;
	
	this->inventory = new Inventory();
	this->point = new PointCross();
	this->minimap = new MiniMap();
	
	this->player_camera2D = new Camera();
	this->player_camera2D->view_matrix = Matrix44();
	this->player_camera2D->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);
}

void EntityPlayer::update(float seconds_elapsed)
{
	// Update sleep and stamina
	this->player_sleepiness -= 0.15f * seconds_elapsed;
	this->player_hunger -= 0.15f * seconds_elapsed;

	// Set some variables
	float moving_speed = this->player_speed;
	const Vector3& player_front = this->model.frontVector().normalize();
	const Vector3& player_right = this->model.rightVector().normalize();
	Vector3 current_position = position();

	// Code adapted from Camera controls given in the framework skeleton
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) moving_speed *= 10.0f;
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) this->model.translate(- player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) this->model.translate( player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) this->model.translate( - player_right * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->model.translate(player_right * moving_speed);
	Vector3 possible_position = position();

	
	// Check and manage possible collisions
	World* current_world = PlayStage::current_stage->scene;
	std::vector<sCollisionData> collisions;

	for (auto entity : current_world->root->children) {
		EntityCollider* entity_collider = dynamic_cast<EntityCollider*>(entity);
		if (entity_collider && entity_collider->checkPlayerCollisions(possible_position, collisions)) {
			this->model.setTranslation(current_position);
		}
		else {
			this->model.setTranslation(possible_position);
		}
	}
	
}

void EntityPlayer::render(Camera* camera)
{
	// Render mesh (not necessary, since it's first person)
	// EntityMesh::render(camera);

	// Load shaders and mesh, get model matrix and enable shader
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
	Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");
	Matrix44 m = this->model;
	shader->enable();
	// Do translation and scaling transformations
	m.translate(0.0f, player_height, 0.0f);
	m.scale(sphere_radius, sphere_radius, sphere_radius);

	// Set basic uniforms values
	shader->setUniform("u_color", Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_model", m);

	// Render mesh
	mesh->render(GL_LINES);

	// Disable shader
	shader->disable();

	this->inventory->render(this->player_camera2D);
	this->point->render(this->player_camera2D);
	this->minimap->render();
}