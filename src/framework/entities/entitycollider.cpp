#pragma once
#include "entitycollider.h"
#include "game/stage.h"
#include <algorithm>

bool EntityCollider::checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& wall_collisions, std::vector<sCollisionData>& floor_collisions) {
	Vector3 col_point;
	Vector3 col_normal;

	const Vector3& player_head = target_pos + PlayStage::player->player_height;
	const float sphereRadius = PlayStage::player->sphere_radius;
	const Vector3& player_foot = target_pos + Vector3(0.f, sphereRadius, 0.f);
	const float max_ray_dist = PlayStage::player->player_height.y;

	if (!this->isInstanced) {
		// TESTING SPHERE COLLISION ON HEAD
		if (this->mesh->testSphereCollision(this->model, player_head, sphereRadius, col_point, col_normal)) {
			wall_collisions.push_back({ col_point, col_normal.normalize(), player_head.distance(col_point), this });
			std::cout << "wall!" << std::endl;
		}
		
		// TESTING SPHERE COLLISION ON FOOT
		if (this->mesh->testSphereCollision(this->model, player_foot, sphereRadius, col_point, col_normal)) {
			wall_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
			std::cout << "wall!" << std::endl;
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
				std::cout << "wall inst!" << std::endl;
			}

			// TESTING SPHERE COLLISION ON FOOT
			if (this->mesh->testSphereCollision(this->models[i], player_foot, sphereRadius, col_point, col_normal)) {
				wall_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
				std::cout << "wall inst!" << std::endl;
			}

			// TESTING RAY COLLISION WITH FLOOR
			if (this->mesh->testRayCollision(this->models[i], player_foot, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
				floor_collisions.push_back({ col_point, col_normal.normalize(), player_foot.distance(col_point), this });
			}
		}
	}

	return (wall_collisions.empty() || floor_collisions.empty());
}