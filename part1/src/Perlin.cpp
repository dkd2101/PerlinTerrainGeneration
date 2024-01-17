#include "Perlin.hpp"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include<algorithm>

// generates a permutation of possible color values to select from.
Perlin::Perlin(){
    this->permutations = std::vector<int>();
    this->perlin_pixels = std::vector<int>();

    //std::cout << "Pushing intiial" << std::endl;
    for(int i = 0; i < 255; ++i){
        this->permutations.push_back(i);
    }

    this->shuffle();

    // we want this to be doubled so that we can more easily sample edges
    int prevSize = this->permutations.size() - 1;
    for(int i = 0; i < prevSize; ++i){
        int val = permutations.at(i);
        this->permutations.push_back(val);
    }
}


Perlin::~Perlin(){

}

// shuffles the permutation array by swapping randomly
void Perlin::shuffle(){
    for(int i = permutations.size(); i >= 0; i--) {
		int j = rand() % (i + 1);

		std::swap(permutations[i], permutations[j]);
	}
}

// performs a fade calculation on a given value (from perlin noise equation)
float Perlin::fade(float t){
    return ((6 * t - 15) * t + 10) * t * t * t;
}

// calculates the linear interpolation between two values given a percent t and two values
float Perlin::interpolate(float t, float one, float two){
    return one + t * (two - one);
}

// performs a 2d calculation of perlin noise generation using the given x and y values
float Perlin::noise2D(float x, float y){
    int xFloor = std::floor(x);
    int yFloor = std::floor(y);
    
    float diffX = x - xFloor;
    float diffY = y - yFloor; 

    glm::vec2 bottomLeft = glm::vec2(diffX, diffY);
    glm::vec2 topLeft = bottomLeft + glm::vec2(0.0, -1.0);
    glm::vec2 bottomRight = bottomLeft + glm::vec2(-1.0, 0.0);
    glm::vec2 topRight = bottomLeft + glm::vec2(-1.0, -1.0);

    int valTopRight = permutations[permutations[xFloor + 1] + yFloor + 1];
    int valueTopLeft = permutations[permutations[xFloor]+yFloor+1];
	int valueBottomRight = permutations[permutations[xFloor+1]+yFloor];
	int valueBottomLeft = permutations[permutations[xFloor]+yFloor];

    float topRightGrad = glm::dot(topRight, gradient(valTopRight));
    float topLeftGrad = glm::dot(topLeft, gradient(valueTopLeft));
    float bottomRightGrad = glm::dot(bottomRight, gradient(valueBottomRight));
    float bottomLeftGrad = glm::dot(bottomLeft, gradient(valueBottomLeft));

    float fadeX = this->fade(diffX);
    float fadeY = this->fade(diffY);


    float result = interpolate(fadeX, 
                       interpolate(fadeY, bottomLeftGrad, topLeftGrad), 
                       interpolate(fadeY, bottomRightGrad, topRightGrad));

    return result;
}

// generates a random gradient unit vector 
glm::vec2 Perlin::gradient(int perm){
    int seed = perm % 4;
    if(seed == 1){
        return glm::vec2(1.0, 1.0);
    } else if(seed == 2){
        return glm::vec2(-1.0, -1.0);
    } else if(seed == 3){
        return glm::vec2(1.0, -1.0);
    } else {
        return glm::vec2(-1.0, 1.0);
    }
}

// generates our array of grayscale values into our pixel vector
void Perlin::generatePPM(bool fractal){
    this->perlin_pixels = std::vector<int>();
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            if(fractal){
                this->perlin_pixels.push_back(fractalBrown(x, y));
            } else{
                this->perlin_pixels.push_back(basicGeneration(x, y));
            }
        
        }
    }
}

// generates basic perlin noise without any octaves or amplitude calculations
float Perlin::basicGeneration(float x, float y){
    float n = noise2D(x * 0.01, y * 0.01);
    // transform the scale back to 0.0-1.0 (needs to generate negatives for fractal brown)
    n += 1.0;
    n /= 2.0;

    int c = round(255 * n);
    return c;
}

// applies a frequency, amplitude, and calculates with octaves to make a more dynamic rendering
float Perlin::fractalBrown(float x, float y){
    float result = 0.0;
    float amplitude = 1.0;
    float frequency = 0.005;
    
    for(int oct = 0; oct < 8; oct++){
        float n = amplitude * noise2D(x * frequency, y * frequency);
        result += n;
        
        amplitude /= 2.0;
        frequency *= 2.0;
    }
    int ret = std::max(0, (int) round(255 * result));
    return ret;
}

// saves our current data as a ppm 
void Perlin::savePPM(std::string filepath, std::string filename){
    std::ofstream upload_img;

    upload_img.open(filename);

    upload_img << "P3" << std::endl;

    upload_img << std::to_string(512) << " ";
    upload_img << std::to_string(512) << std::endl;
    upload_img << std::to_string(255) << std::endl;

    for(int i = 0; i < this->perlin_pixels.size(); i++){
        upload_img << int(this->perlin_pixels[i]) << std::endl;
        upload_img << int(this->perlin_pixels[i]) << std::endl;
        upload_img << int(this->perlin_pixels[i]) << std::endl;
    }
}
