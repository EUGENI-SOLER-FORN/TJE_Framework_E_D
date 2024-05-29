#pragma once
#include "entityplayer.h"
#include "game/game.h"

EntityPlayer::EntityPlayer() : EntityMesh(){
	this->name = "player";
	this->model = Matrix44::IDENTITY;
	this->model.setTranslation(this->player_height + Vector3(0.f, 10.f, 0.f));
	this->inventory = new Inventory();
	this->point = new PointCross();
	this->minimap = new MiniMap();
	
	this->player_camera2D = new Camera();
	this->player_camera2D->view_matrix = Matrix44();
	this->player_camera2D->setOrthographic(0.f, (float)Game::instance->window_width, 0.f, (float)Game::instance->window_height, -1.f, 1.f);
}

void EntityPlayer::update(float seconds_elapsed){
	// Check if we are in a playstage, if not return;
	PlayStage* ps = dynamic_cast<PlayStage*>(Game::instance->manager->current);
	if (!ps) return;

	// Update sleep and stamina
	this->player_sleepiness -= 0.15f * seconds_elapsed;
	this->player_hunger -= 0.15f * seconds_elapsed;

	// Set some variables
	float moving_speed = this->player_speed;
	Vector3 moving_direction;

	const Vector3& player_front = this->model.frontVector().normalize();
	const Vector3& player_right = this->model.rightVector().normalize();
	Vector3 current_position = position();

	// Code adapted from Camera controls given in the framework skeleton
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) moving_speed *= 3.f;
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP))	moving_direction += player_front;
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN))	moving_direction -= player_front;
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))	moving_direction += player_right;
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) moving_direction -= player_right;
	// Normalize so we don't move faster diagonally and multiply with speed
	moving_direction = moving_direction.normalize() * moving_speed;
	// Set velocity vector
	this->velocity += moving_direction;

	// Check and manage possible collisions
	World* current_world = PlayStage::current_stage->scene;
	std::vector<sCollisionData> collisions;

	const Vector3 possible_position = current_position + this->velocity * seconds_elapsed;
	for (auto entity : current_world->root->children) {
		EntityCollider* entity_collider = dynamic_cast<EntityCollider*>(entity);
		if (entity_collider) entity_collider->checkPlayerCollisions(possible_position, collisions);
	}
	// Run the array of collisions
	bool grounded = false;
	for (sCollisionData& col : collisions) {
		// If normal is pointing upwards, it means it's a floor collision
		float up_factor = col.colNormal.dot(Vector3::UP);
		if (up_factor > 0.8) { 
			grounded = true; 
			this->velocity.y = 0.f >  this->velocity.y ? 0.f : this->velocity.y;
			continue;
		}
		
		const Vector3 slide_direction = velocity.dot(col.colNormal) * col.colNormal;
		this->velocity.x -= slide_direction.x;
		this->velocity.z -= slide_direction.z;
	}
	if (grounded && Input::isKeyPressed(SDL_SCANCODE_SPACE)) this->velocity.y = 2.0f;
	const Vector3& new_position = current_position + this->velocity * seconds_elapsed;

	// Reduce velocity for next frame stopping
	this->velocity.x *= 0.5f;
	if(!grounded) this->velocity.y  -= 9.8f * seconds_elapsed;
	this->velocity.z *= 0.5f;

	// Rotate to look at camera is poining
	this->model.setTranslation(new_position);
	this->model.rotate(ps->camera_yaw, Vector3::UP);
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
	m.translate(this->player_height);
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