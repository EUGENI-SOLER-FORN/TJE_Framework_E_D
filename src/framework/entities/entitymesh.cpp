#pragma once
#include "entitymesh.h"

// Code from theory slide 8 - Scene Representation
void EntityMesh::render(Camera* camera) {
	if (!this->isVisible || !this->mesh) return;

	this->material.shader->enable();
	// Set color default
	this->material.shader->setUniform("u_color", this->material.color);
	// Set model, or parent model if instanced
	this->material.shader->setUniform("u_model", this->model);
	if(this->isInstanced) this->material.shader->setUniform("u_model", this->getGlobalMatrix());
	
	// Set viewprojection matrix of the camera
	this->material.shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	// Set texture
	if(this->material.diffuse) this->material.shader->setTexture("u_texture", this->material.diffuse, 0);


	// Render the mesh using the shader
	this->mesh->render(GL_TRIANGLES);

	// Disable shader after finishing rendering
	this->material.shader->disable();
}