#pragma once
#include "entitycollider.h"
#include "game/stage.h"
#include <algorithm>

bool EntityCollider::checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& collisions) {
	Vector3 col_point;
	Vector3 col_normal;

	const Vector3& player_head = target_pos + PlayStage::player->player_height;
	const float max_ray_dist = PlayStage::player->player_height.y;
	const float sphereRadius = PlayStage::player->sphere_radius;

	if (!this->isInstanced) {
		// TESTING SPHERE COLLISION ON HEAD
		if (this->mesh->testSphereCollision(this->model, player_head, sphereRadius, col_point, col_normal)) {
			collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
			// std::cout << "Sphere Collision!" << std::endl;
		}
		
		// TESTING SPHERE COLLISION ON FOOT
		if (this->mesh->testSphereCollision(this->model, target_pos, sphereRadius, col_point, col_normal)) {
			collisions.push_back({ col_point, col_normal.normalize(), target_pos.distance(col_point), this });
			// std::cout << "Sphere Collision!" << std::endl;
		}

		// TESTING RAY COLLISION WITH FLOOR
		if (this->mesh->testRayCollision(this->model, target_pos, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
			collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
			// std::cout << "Ray Collision!" << std::endl;
		}
	}
	else {
		for (int i = 0; i < this->models.size(); ++i){
			// TESTING SPHERE COLLISION ON HEAD
			if (this->mesh->testSphereCollision(this->models[i], player_head, sphereRadius, col_point, col_normal)) {
				collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
				// std::cout << "Sphere Collision!" << std::endl;
			}

			// TESTING SPHERE COLLISION ON FOOT
			if (this->mesh->testSphereCollision(this->models[i], target_pos, sphereRadius, col_point, col_normal)) {
				collisions.push_back({ col_point, col_normal.normalize(), target_pos.distance(col_point), this });
				// std::cout << "Sphere Collision!" << std::endl;
			}

			// TESTING RAY COLLISION WITH FLOOR
			if (this->mesh->testRayCollision(this->models[i], target_pos, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
				collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
				// std::cout << "Ray Collision!" << std::endl;
			}
		}
	}

	return collisions.empty();
}