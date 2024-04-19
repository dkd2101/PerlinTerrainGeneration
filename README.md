# Terrain Generation Using Perlin Noise, Sophisticated Camera Contolling in a 3D space

***Note: This project was transferred from a private repository as it was submittted as a part of Northeastern's CS4300 Computer Graphics class. This project was created in a 1-month timeline and has a much more complex commit history that is not represented in this repo.

## YouTube Explanation Video: https://youtu.be/s-RPZjqrISE

[![Mountains.png](https://i.postimg.cc/d0prSR40/Mountains.png)](https://postimg.cc/FfbYYStt)
[![Hills.png](https://i.postimg.cc/N0YW2T15/Hills.png)](https://postimg.cc/gxSMfxtW)
[![CloseUp.png](https://i.postimg.cc/W3sTVC9Y/CloseUp.png)](https://postimg.cc/yJpwXQC0)

## About
A random terrain Generator using OpenGL library and C++. Upon startup the program generates a Perlin ppm image and passes that as a texture into OpenGL to use as a height map to render terrains. A custom shader blends together passed in textures together based off of the height of the pixel in the scene. Smooth camera movement with up, down, and cursor looking are implemented to traverse the terrain. 

Two types of Perlin generation are used, fractal brownian and non-fractal brownian generation. Fractal brownian motion allows more jagged and varying heights wheresas non-fractal brownian gives a more hill-like terrain very smooth with lots of valleys and peaks.



## Team Members
  *Linwood Blaisdell*
  *Dylan Dinio*
