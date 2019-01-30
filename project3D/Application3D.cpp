#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE

#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() 
{
}

Application3D::~Application3D() 
{
}

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

	return true;
}

void Application3D::shutdown() 
{
	Gizmos::destroy();
}

mat4 tank_base;
mat4 turrent_bullet;
mat4 turrent_base;
mat4 barrel_base;
mat4 world;
mat4 Matrix3;

struct Tank
{
	mat4 tank_base = world;
	mat4 world = mat4(1);

	mat4 tank_base_rot = glm::rotate(tank_base, 0.0f, vec3(0, 1, 0));
	mat4 tank_base_trans = glm::translate(tank_base, vec3(0, 0, 0));
	
	mat4 turrent_base = glm::translate(tank_base, vec3(0, 1, 0));
	mat4 turrent_base_rot = glm::rotate(turrent_base, 0.0f, vec3(0, 0, 0));
	mat4 turrent_base_trans = glm::translate(turrent_base, vec3(0, 1, 0));

	mat4 turrent_barrel = glm::translate(turrent_base, vec3(0, 0, 0));
	mat4 turrent_barrel_rot = glm::rotate(turrent_barrel, 1.57f, vec3(0, 0, 1));
	mat4 turrent_barrel_trans = glm::translate(turrent_barrel, vec3(0, 0, 0));

	mat4 turrent_bullet_trans = glm::translate(turrent_barrel, vec3(0, 0, 1));
	mat4 turrent_bullet = glm::rotate(turrent_base, 0.5f, vec3(0, 0, 1));
	mat4 turrent_bullet_rot = glm::rotate(turrent_bullet, 1.57f, vec3(0, 0, 1));

	float turrent_bullet_size = 0.4f;
};

Tank move;

void Application3D::update(float deltaTime)
{	
	// query time since application started
	float time = getTime();
	time = 0;

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 0, 10, glm::cos(time) * 10),
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
	move.tank_base = world * move.tank_base_trans * move.tank_base_rot;
	move.turrent_base = move.world * move.turrent_base_trans * move.turrent_base_rot;
	move.turrent_barrel = move.turrent_base * move.turrent_barrel_trans * move.turrent_barrel_rot;
	//the bullet has no parent, it has an initial rotation of the barrels rotation in world
	move.turrent_bullet = move.turrent_base * move.turrent_bullet_trans * move.turrent_bullet_rot;

	vec3 forward_tank = move.tank_base[2] * (10 * deltaTime);
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		move.tank_base_trans = glm::translate(move.tank_base_trans, forward_tank);
		move.turrent_base_trans = glm::translate(move.turrent_base_trans, forward_tank);
	}

	// input :: shoots the bullet in the direction of the turret
	vec3 forward_bullet = vec3(move.turrent_barrel[2].x, move.turrent_barrel[2].z, -move.turrent_barrel[2].y) * (40 * deltaTime);
	//vec3 forward_bullet = abrams.turret_base[3].xyz;
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		move.turrent_bullet_size = 0.4f;
		move.turrent_bullet_trans = glm::translate(move.turrent_bullet_trans, forward_bullet);
	}
	else
	{
		move.turrent_bullet_trans = move.turrent_barrel_trans;
		move.turrent_bullet_size = 0.0f;
	}

	// input :: rotates the Tank left and right
	if (input->isKeyDown(aie::INPUT_KEY_A))
		move.tank_base_rot = glm::rotate(move.tank_base_rot, 0.1f, vec3(0, 1, 0));
	if (input->isKeyDown(aie::INPUT_KEY_D))
		move.tank_base_rot = glm::rotate(move.tank_base_rot, -0.1f, vec3(0, 1, 0));

	// input :: rotate just the turret right and left
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		move.turrent_base_rot = glm::rotate(move.turrent_base_rot, .05f, vec3(0, 1, 0));
	}
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		move.turrent_base_rot = glm::rotate(move.turrent_base_rot, -.05f, vec3(0, 1, 0));
	}

	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	
	//// demonstrate a few shapes
	Gizmos::addTransform(move.tank_base, 5);
	Gizmos::addTransform(move.turrent_base, 5);
	Gizmos::addTransform(move.turrent_barrel, 5);

	// renders the Tank, Tank's turret base, and Tank's turret's base
	Gizmos::addAABBFilled(move.world[3], vec3(1, 0.5, 1), red, &move.tank_base); //body
	Gizmos::addSphere(move.world[3], 0.8, 8, 8, red, &move.turrent_base);//bullet
	Gizmos::addSphere(vec3(1), move.turrent_bullet_size, 8, 8, red, &move.turrent_bullet); //left sphere
	Gizmos::addCylinderFilled(move.world[3], 0.3, 1, 15, red, &move.turrent_barrel);//center
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
