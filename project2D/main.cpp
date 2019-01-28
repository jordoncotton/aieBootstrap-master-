#include "Application2D.h"
#include<Vectors.h>
int main() {
	
	Vector2 a = Vector2(1, 2);
	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}