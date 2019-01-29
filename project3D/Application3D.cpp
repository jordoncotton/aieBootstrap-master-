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
mat4 turret_base;
mat4 barrel_base;
mat4 world;
mat4 Matrix3;

struct Tank
{
	mat4 tank_base = world;
	mat4 world = mat4(1);

	mat4 tank_base_rot = glm::rotate(tank_base, 0.5f, vec3(0, 1, 0));
	mat4 tank_base_trans = glm::translate(tank_base, vec3(0, 0, 0));
	
	mat4 turrent_base = glm::translate(tank_base, vec3(0, 1, 0));
	mat4 turrent_base_rot = glm::rotate(turrent_base, 0.5f, vec3(0, 0, 0));
	mat4 turrent_base_trans = glm::translate(turrent_base, vec3(0, 1, 0));

	//////////////////////////////////////////////////////////////////////////

	mat4 turrent_Barrel = glm::translate(turrent_base, vec3(0, 0, 0));
	mat4 turrent_barrel_rot = glm::rotate(turrent_barrel, 0.5f, vec3(0, 0, 1));
	mat4 turrent_barrel_trans = glm::translate(turrent_barrel, vec3(0, 0, 0));

	mat4 turrent_barrel_translation = glm::translate(turrent_barrel, vec3(0, 0, 1));
	mat4 turrent_barrel = glm::rotate(turrent_base, 0.5f, vec3(0, 0, 1));
	mat4 turrent_barrel_rotation = glm::rotate(turrent_bullet, 0.5f, vec3(0, 0, 1));
};

void Application3D::update(float deltaTime) 
{
	world = mat4(1);
	tank_base = world * tank_base * tank_base; 
	turrent_base = tank_base * turrent_base * turret_base;

	mat4 
	
	Matrix3 = mat4();
	
	// query time since application started
	float time = getTime();
	time = 0;

	// rotate camera
	m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 0, 10, glm::cos(time) * 10),
		vec3(0), vec3(0, 1, 0));


	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
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

	//// demonstrate a few shapes
	Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(.5, 0, 0, 0.5f)); //body
	Gizmos::addSphere(vec3(2, 1, 2),1 , 9, 9, vec4(.5, 0, 0, 0.5f));//bullet
	Gizmos::addSphere(vec3(0, 1, 0), 1, 8, 8, vec4(.5, 0, 0, 0.5f)); //left sphere
	Gizmos::addCylinderFilled(vec3(2, 4, 0),//center
		-1,//radius
		3,//halflength
		8,//segments
		vec4(.5, 0, 0, 0.5f)); //barrel

	mat4 t = glm::rotate(mat4(1), time, glm::normalize(vec3(1, 1, 1)));
	t[3] = vec4(-2, 0, 0, 1);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		tank_base *= forward_trans;
	}

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		tank_base *= *rotate_Left;
	}

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		tank_base *= back_trans;
	}

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		tank_base *= *rotate_Right;
	}

	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		turret_base *= *rotate_Left;
	}

	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		turret_base *= *rotate_Right;
	}

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();	
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

void Application3D::shoot()
{
	if (!bullet_exist)
	{
		bullet_exist = true;
		bullet = barrel_cap1;
	}
}

void Application3D::bullet_set(float deltaTime)
{
	if (bullet_exist)
	{
		bullet_time += deltaTime;
	}
}

void Application3D::demolish_bullet()
{
	bullet_exist = false;
	bullet_time = 0;
}
