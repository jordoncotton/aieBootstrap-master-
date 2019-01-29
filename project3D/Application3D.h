#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

class Application3D : public aie::Application 
{
public:
	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();



protected:
	bool bulletExist = false;
	float bulletTime = 0;

	const glm::mat4 identity = glm::mat4(1);
	glm::mat4 tank_base = identity;
	
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	glm::mat4 turrent_base;

	glm::mat4 front;

	glm::mat4 barrel_base;
	
	glm::mat4 bullet; 
	
	glm::mat4 barrelCap1;
	glm::mat4 barrelCap2;
	
	glm::mat4 forward = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, .05, 0, 0, 1);
	glm::mat4 back = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -.05, 0, 0, 1);
	glm::mat4 down = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -.05, 0, 0, 1);
	glm::mat4 up = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, .05, 0, 0, 1);


	glm::mat4 *rotateLeft = new glm::mat4(cos(.025), 0, -sin(.025), 0, 0, 1, 0, 0, sin(.025), 0, cos(.025), 0, 0, 0, 0, 1);
	glm::mat4 *rotateRight = new glm::mat4(cos(-.025), 0, -sin(-.025), 0, 0, 1, 0, 0, sin(-.025), 0, cos(-.025), 0, 0, 0, 0, 1);

	void shoot();
	void bulletSet(float deltaTime);
	void demolishBullet();
	float zCam;
};