#include "entitycollider.h"

bool EntityCollider::checkPlayerCollisions(const Vector3& target_pos, std::vector<sCollisionData>& collisions)
{

	Vector3 col_point;
	Vector3 col_normal;
	Vector3 center = target_pos + Vector3(0.f, 1.25f, 0.f);
	float max_ray_dist = 100.f;
	float sphereRadius = 1.0f;

	// TESTING RAY COLLISION 
	if (mesh->testRayCollision(this->model, target_pos, Vector3(0.0f, -1.0f, 0.0f), col_point, col_normal, max_ray_dist, false)) {
		collisions.push_back({ col_point, col_normal.normalize() });
		std::cout << "Ray Collision!" << std::endl;
		return true;
	}

	// TESTING SPHERE COLLISION
	if (mesh->testSphereCollision(model, center, sphereRadius, col_point, col_normal)) {
		collisions.push_back({ col_point, col_normal.normalize() });
		std::cout << "Sphere Collision!" << std::endl;
		return true;
	}

	return false;
}