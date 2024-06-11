#pragma once
#include "entitymesh.h"

// Code from theory slide 8 - Scene Representation
void EntityMesh::render(Camera* camera) {
	if (!this->isVisible || !this->mesh || !this->material.shader) return;
	
	const Matrix44& globalMat = this->getGlobalMatrix();
	const Vector3& mesh_box_center = globalMat * this->mesh->box.center;
	const Vector3& mesh_box_halfsize = globalMat * this->mesh->box.halfsize;
	if (!this->isInstanced&& !camera->testBoxInFrustum(mesh_box_center, mesh_box_halfsize) || camera->eye.distance(mesh_box_center) > 200.0f)	return;

	this->material.shader->enable();
	// Set color default
	this->material.shader->setUniform("u_color", this->material.color);
	// Set model, or parent model if instanced
	this->material.shader->setUniform("u_model", this->model);
	if(!this->isInstanced) this->material.shader->setUniform("u_model", globalMat);
	
	// Set viewprojection matrix of the camera
	this->material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	this->material.shader->setUniform("u_camera_pos", camera->eye);
	if (camera->type == Camera::ORTHOGRAPHIC) this->material.shader->setUniform("u_minimap", 1.f);

	// Set texture
	if (this->material.diffuse) this->material.shader->setTexture("u_texture", this->material.diffuse, 0);

	// Render the mesh using the shader and depending on
	if (this->isInstanced) this->mesh->renderInstanced(GL_TRIANGLES, this->models.data(), this->models.size());
	else this->mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	this->material.shader->disable();

	for (auto e : this->children) e->render(camera);
}

