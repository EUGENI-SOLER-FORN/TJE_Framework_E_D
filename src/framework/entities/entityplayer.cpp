#pragma once
#include "entityplayer.h"

void EntityPlayer::update(float seconds_elapsed)
{
	float moving_speed = this->player_speed;
	const Vector3& player_front = this->model.frontVector().normalize();
	const Vector3& player_right = this->model.rightVector().normalize();

	// Code adapted from Camera controls given in the framework skeleton
	// Run with shift
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) moving_speed *= 10;
	
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) this->model.translate(- player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) this->model.translate( player_front * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) this->model.translate( - player_right * moving_speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) this->model.translate(player_right * moving_speed);

	//TODO: Manage all related with collisions
}

void EntityPlayer::render(Camera* camera)
{
	
	// Render mesh
	EntityMesh::render(camera);

	// Load shaders and mesh, get model matrix and enable shader
	Shader* shader = Shader::Get("basic.vs", "flat.fs");
	Mesh* mesh = Mesh::Get("data/meshes/sphere.obj");
	Matrix44 m = model;
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

	//TODO: Try to render sphere for collisions
}

/*bool checkPlayerCollisions(const Vector3& target_pos,
	std::vector<sCollisionData>& collisions) {
	Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
	float sphereRadius = 0.75f;
	Vector3 colPoint, colNormal;
	// For each collider entity “e” in root:
	Mesh* mesh = e->mesh;
	if (mesh->testSphereCollision(e->model, center,
		sphereRadius, colPoint, colNormal)) {
		collisions.push_back({ colPoint,
		colNormal.normalize() });
	}
	// End loop
	return !collisions.empty();
}*/


