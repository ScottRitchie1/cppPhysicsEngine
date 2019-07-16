#include "PhysicsApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsApp();

	// initialise and loop
	app->run("AIE", 576, 1024 , false);

	// deallocation
	delete app;

	return 0;
}