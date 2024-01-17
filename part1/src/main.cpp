// Support Code written by Michael D. Shah
// Last Updated: 6/11/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"
#include "Perlin.hpp"
#include <iostream>


int main(int argc, char** argv){

	std::cout << "Creating perlin objects" << std::endl;
	Perlin perlin = Perlin();

	std::cout << "generatiing ppms" << std::endl;
	perlin.generatePPM(false);
	std::cout << "saving ppm to file" << std::endl;

	perlin.savePPM("", "perlin_texture.ppm");

	// Create an instance of an object for a SDLGraphicsProgram
	SDLGraphicsProgram mySDLGraphicsProgram(1280,720);
	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
