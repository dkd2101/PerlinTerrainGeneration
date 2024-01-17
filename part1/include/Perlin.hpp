#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <vector>
#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include <string>

class Perlin{
    public:

        Perlin();

        ~Perlin();

        // return the given number parsed through the fade equation
        float fade(float num);

        // generates a random unit vector to be used in the perlin equation
        glm::vec2 gradient(int perm);

        // creates the 2D noise
        float interpolate(float a1, float a2, float a3);

        //generates a noise value from a given number between 0 and 1
        float noise2D(float x, float y);

        // shuffle the values inside the permutation
        void shuffle();

        // returns the given float value gone through Fractal Brownian motion
        // which takes into consideration amplitude and octaves allowing for
        // more or less smoothing;
        float fractalBrown(float x, float y);

        // does a basic calculation of perlin noise returning a more hill like terrain
        float basicGeneration(float x, float y);

        // saves the calculated color values to a PPM file and returns the filepath
        void savePPM(std::string path, std::string filename);

        // generate perlin pixels to be put into the pixel array. 
        void generatePPM(bool fractal);

    private:
        std::vector<int> permutations;
        std::vector<int> perlin_pixels;

};

#endif

