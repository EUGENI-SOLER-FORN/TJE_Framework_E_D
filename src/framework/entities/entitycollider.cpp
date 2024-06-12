#pragma once
#include "entitycollider.h"
#include "game/stage.h"
#include "game/game.h"
#include <algorithm>

bool EntityCollider::checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& wall_collisions, std::vector<sCollisionData>& floor_collisions, eEntityType t) {
	if (this->layer != t) return false;
	Vector3 col_point;
	Vector3 col_normal;

	const Vector3& player_head = target_pos + PlayStage::player->player_height;
	const float sphereRadius = PlayStage::player->sphere_radius;
	const Vector3& player_foot = target_pos + Vector3(0.f, sphereRadius * 0.75f, 0.f);
	const float max_ray_dist = PlayStage::player->player_height.y;

	if (!this->isInstanced) {
		// TESTING SPHERE COLLISION ON HEAD
		if (this->mesh->testSphereCollision(this->model, player_head, sphereRadius, col_point, col_normal)) {
			wall_collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
		}
		
		// TESTING SPHERE COLLISION ON FOOT
		if (this->mesh->testSphereCollision(this->model, player_foot, sphereRadius * 0.75f, col_point, col_normal)) {
			wall_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
		}

		// TESTING RAY COLLISION WITH FLOOR
		if (this->mesh->testRayCollision(this->model, player_foot, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
			floor_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
			// std::cout << "Ray Collision!" << std::endl;
		}
	}
	else {
		for (int i = 0; i < this->models.size(); ++i){
			// TESTING SPHERE COLLISION ON HEAD
			if (this->mesh->testSphereCollision(this->models[i], player_head, sphereRadius, col_point, col_normal)) {
				wall_collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
			}

			// TESTING SPHERE COLLISION ON FOOT
			if (this->mesh->testSphereCollision(this->models[i], player_foot, sphereRadius * 0.75f, col_point, col_normal)) {
				wall_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
			}

			// TESTING RAY COLLISION WITH FLOOR
			if (this->mesh->testRayCollision(this->models[i], player_foot, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
				floor_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
			}
		}
	}

	return (wall_collisions.empty() || floor_collisions.empty());
}

void EntityDrop::hit(){
	if (!this->hitpoints) return;
	this->hitpoints -= 1;
	this->health = 0.01f*(this->hitpoints * 20.f);
	if (this->health == 0.f) {
		this->changeVisibility();
		PlayStage::player->inventory->add(WOOD);
	}
	std::cout << health <<  std::endl;
}

void EntityDrop::render(Camera* camera){
	// same as EntityMesh, but set the mask to the shader if it's being pointed at:
	if (!this->isVisible || !this->mesh || !this->material.shader) return;

	if (this->healthbar && this->type == TREE) {
		this->healthbar->render(PlayStage::player->player_camera2D);
	}

	const Matrix44& globalMat = this->getGlobalMatrix();
	const Vector3& mesh_box_center = globalMat * this->mesh->box.center;
	const Vector3& mesh_box_halfsize = globalMat * this->mesh->box.halfsize;
	if (!this->isInstanced && !camera->testBoxInFrustum(mesh_box_center, mesh_box_halfsize) || camera->eye.distance(mesh_box_center) > 200.0f)	return;

	this->material.shader->enable();
	// Set color default
	this->material.shader->setUniform("u_color", this->material.color);
	// Set model, or parent model if instanced
	this->material.shader->setUniform("u_model", this->model);
	if (!this->isInstanced) this->material.shader->setUniform("u_model", globalMat);

	// Set viewprojection matrix of the camera
	this->material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	this->material.shader->setUniform("u_camera_pos", camera->eye);

	// Set texture
	if (this->material.diffuse) this->material.shader->setTexture("u_texture", this->material.diffuse, 0);
	this->material.shader->setUniform("u_mask", this->mask);

	// Render the mesh using the shader and depending on
	if (this->isInstanced) this->mesh->renderInstanced(GL_TRIANGLES, this->models.data(), (int)this->models.size());
	else this->mesh->render(GL_TRIANGLES);

	this->material.shader->setUniform("u_mask", 0.f);
	this->material.shader->setUniform("u_minimap", 0.f);

	// Disable shader after finishing rendering
	this->material.shader->disable();

	for (auto e : this->children) e->render(camera);
	this->mask = 0.f;
}
void EntityDrop::update(float seconds_elapsed){
	EntityMesh::update(seconds_elapsed);
	if (!this->healthbar) return;

	float w = (float)Game::instance->window_width;
	float h = (float)Game::instance->window_height;
	Camera* c = PlayStage::current_stage->stageCamera;
	Vector3 lookdir = (c->center - c->eye);
	Vector3 treedir = (this->model.getTranslation() - c->eye);
	if (lookdir.dot(treedir) > 0.f) {
		Vector3 clip_space = c->project(this->model.getTranslation(), w, h);
		this->healthbar->position.x = clip_space.x;
		this->healthbar->position.y = clip_space.y;
		this->healthbar->update_stat(this->health);
	}
};

EntityDrop::EntityDrop(Mesh* mesh, Material& material) : EntityCollider(mesh, material) {
	this->layer = TREE; this->name = "Tree";

	float h = (float)Game::instance->window_height / 2.f;
	float w = (float)Game::instance->window_width / 2.f;
	Material mat;
	mat.color = Vector4(0.f, 1.f, 0.f, 1.f);
	mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/statbar.fs");
	this->healthbar = new StatBar(this->health, Vector2(w + 50.f, h), Vector2(25.f, 200.f), mat);
	this->healthbar->setTreeIcon(); 
}