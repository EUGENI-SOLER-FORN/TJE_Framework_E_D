#pragma once
#include "entityplayer.h"
#include "entitycollider.h"
#include "game/stage.h"

void EntityPlayer::update(float seconds_elapsed)
{

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
	for (auto entity : current_world->root->children) {
		EntityCollider* entity_collider = dynamic_cast<EntityCollider*>(entity);
		std::vector<sCollisionData> collisions;
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
	// Render mesh
	EntityMesh::render(camera);

	// Load shaders and mesh, get model matrix and enable shader
	Shader* shader = Shader::Get("basic.vs", "flat.fs");
	Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");
	Matrix44 m = model;

	if (shader) {
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
	}
}