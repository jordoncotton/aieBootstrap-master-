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
	bool bullet_exist = false;
	float bullet_time = 0;

	const glm::mat4 identity = glm::mat4(1);
	glm::mat4 tank_base = identity;
	
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	glm::mat4 turrent_base;

	glm::mat4 front;

	glm::mat4 barrel_base;
	
	glm::mat4 bullet; 
	
	glm::mat4 barrel_cap1;
	glm::mat4 barrel_cap2;
	
	glm::mat4 forward_trans = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, .05, 0, 0, 1);
	glm::mat4 back_trans = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -.05, 0, 0, 1);
	glm::mat4 *rotate_Left = new glm::mat4(cos(.025), 0, -sin(.025), 0, 0, 1, 0, 0, sin(.025), 0, cos(.025), 0, 0, 0, 0, 1);
	glm::mat4 *rotate_Right = new glm::mat4(cos(-.025), 0, -sin(-.025), 0, 0, 1, 0, 0, sin(-.025), 0, cos(-.025), 0, 0, 0, 0, 1);

	void shoot();
	void bullet_set(float deltaTime);
	void demolish_bullet();
	float zCam;
};