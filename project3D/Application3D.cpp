#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE

#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

//struct Tank
//{
//	Tank();
//	glm::mat4 tank_base(1);
//
//	mat4 tank_base_rot;
//	mat4 tank_base_trans;
//
//	mat4 base_turret;
//	mat4 turret_base_rot;
//	mat4 turret_base_trans;
//
//	mat4 turret_barrel;
//	mat4 turret_barrel_rot;
//	mat4 turret_barrel_trans;
//
//	mat4 turret_bullet_trans;
//	mat4 turret_bullet;
//	mat4 turret_bullet_rot;
//
//	float turret_bullet_size;
//};
	/*mat4 tank_base(1);*/
	mat4 world(1);
	mat4 tank_base_rot(1);
	mat4 tank_base_trans(1);

	/*mat4 base_turret(1);*/
	mat4 turret_base_rot(1);
	mat4 turret_base_trans(1);

	/*mat4 turret_barrel(1);*/
	mat4 turret_barrel_rot(1);
	mat4 turret_barrel_trans(1);

	mat4 barrel_bullet_trans(1);
	/*mat4 turret_bullet(1);*/
	mat4 barrel_bullet_rot(1);
	

Application3D::Application3D()
{
}

Application3D::~Application3D() 
{
}

glm::mat4 tank_base = world * tank_base_trans * tank_base_rot;
glm::mat4 base_turret = tank_base * turret_base_trans * turret_base_rot;
glm::mat4 turret_barrel = base_turret * turret_barrel_trans * turret_barrel_rot;
glm::mat4 barrel_bullet = turret_barrel * barrel_bullet_trans * barrel_bullet_rot;

bool Application3D::startup() 
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);
	//base
	tank_base_rot = glm::rotate(tank_base, 0.0f, vec3(0, 1, 0));
	tank_base_trans = glm::translate(tank_base, vec3(0, 0, 1));
	//turret
	turret_base_rot = glm::rotate(base_turret, -1.7f, vec3(0, 2, 0));
	turret_base_trans = glm::translate(vec3(0, .5f, 0));
	//barrel
	turret_barrel_rot = glm::rotate(turret_barrel, -1.57f, vec3(0, 0, 1));
	turret_barrel_trans = glm::translate(turret_barrel, vec3(1, 0.3f, 0));
	//bullet
	barrel_bullet_trans = glm::translate(vec3(0, 0, 0));
	barrel_bullet_rot = glm::rotate(barrel_bullet, 1.57f, vec3(0, 0, 1));

	float turret_bullet_size = 0.4f;

	return true;
}

void Application3D::shutdown() 
{
	Gizmos::destroy();
}

void Application3D::update(float deltaTime)
{	
	// query time since application started
	float time = getTime();
	time = 0;

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 20, 10, glm::cos(time) * 20),
		vec3(0), vec3(0, 1, 0));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 red(1, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//add transform to see the axis
	Gizmos::addTransform(mat4(1));

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);

	//transform matrices for the tank
	tank_base = world * tank_base_trans * tank_base_rot;
	base_turret = tank_base * turret_base_trans * turret_base_rot;
	turret_barrel = base_turret * turret_barrel_trans * turret_barrel_rot;
	//the bullet has no parent, it has an initial rotation of the barrels rotation in world
	barrel_bullet = turret_barrel * barrel_bullet_trans * barrel_bullet_rot;

	vec3 forward_tank = tank_base[2] * (10 * deltaTime);
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		tank_base_trans = tank_base_trans * glm::translate(forward_tank);
		/*turret_base_trans = glm::translate(turret_base_trans, forward_tank);*/
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		tank_base_trans = tank_base_trans * glm::translate(-forward_tank);
		/*turret_base_trans = glm::translate(turret_base_trans, forward_tank);*/
	}

	// input :: shoots the bullet in the direction of the turret
	/*vec3 forward_bullet = vec3(turret_barrel[2].x, turret_barrel[2].z, -turret_barrel[2].y) * (40 * deltaTime);*/
	//vec3 forward_bullet = abrams.turret_base[3].xyz;
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		Gizmos::addSphere(barrel_bullet[3], .2, 8, 8, vec4(1, 1, 1, 1), &barrel_bullet); //bullet
		barrel_bullet_trans = barrel_bullet_trans * glm::translate(vec3(0, 5, 0));
	}
	if (input->isKeyUp(aie::INPUT_KEY_SPACE))
	{
		Gizmos::addSphere(barrel_bullet[3], .2, 8, 8, vec4(1,1,1,1), &barrel_bullet); //bullet
		barrel_bullet_trans = glm::translate(vec3(0, 0, 0));
	}
	
	//else
	//{
	//	barrel_bullet_trans = turret_barrel_trans;
	//}

	// input :: rotates the Tank left and right
	if (input->isKeyDown(aie::INPUT_KEY_A))
		tank_base_rot = glm::rotate(tank_base_rot, 0.1f, vec3(0, 1, 0));
	if (input->isKeyDown(aie::INPUT_KEY_D))
		tank_base_rot = glm::rotate(tank_base_rot, -0.1f, vec3(0, 1, 0));

	// input :: rotate just the turret right and left
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		turret_base_rot = glm::rotate(turret_base_rot, -.05f, vec3(0, 1, 0));
	}
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		turret_base_rot = glm::rotate(turret_base_rot, .05f, vec3(0, 1, 0));
	}
	//input :: rotate barrel
	if (input->isKeyUp(aie::INPUT_KEY_UP))
	{
	
		turret_barrel_rot = turret_barrel_rot * glm::rotate(-.05f,vec3(0, 0, 1));
	}
	if (input->isKeyUp(aie::INPUT_KEY_DOWN))
	{
		
		turret_barrel_rot = turret_barrel_rot * glm::rotate(.05f, vec3(0, 0, 1));
	}
	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	
	//// demonstrate a few shapes
	/*Gizmos::addTransform(tank_base, 5);
	Gizmos::addTransform(base_turret, 5);
	Gizmos::addTransform(turret_barrel, 5);*/

	// renders the Tank, Tank's turret base, and Tank's turret's base
	Gizmos::addAABBFilled(tank_base[3], vec3(2, 0.8f, 2), red, &tank_base); //body
	//Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(1, 0, 0, 1));
	Gizmos::addSphere(base_turret[3], 2, 8, 8, black, &base_turret);//turret
	
	Gizmos::addCylinderFilled(turret_barrel[3], 0.4f, 3, 13, red, &turret_barrel); //barrel
}

void Application3D::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// draw 3D gizmos
	Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}