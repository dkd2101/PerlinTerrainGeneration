/** @file Camera.hpp
 *  @brief Sets up an OpenGL camera.
 *  
 *  Sets up an OpenGL Camera. The camera is what
 *  sets up our 'view' matrix.
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

enum camMove {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera{
public:
	// Constructor to create a camera
    Camera();
    // Return a 'view' matrix with our
    // camera transformation applied.
    glm::mat4 GetWorldToViewmatrix() const;
    // Move the camera around
    void MouseLook(int mouseX, int mouseY);
    // A method to process keyboard inputs given the defined enum
    void ProcessKeyboard(camMove direction);
    // Set the position for the camera
    void SetCameraEyePosition(float x, float y, float z);
    // Returns the Camera X Position where the eye is 
    float GetEyeXPosition();
    // Returns the Camera Y Position where the eye is 
    float GetEyeYPosition();
    // Returns the Camera Z Position where the eye is 
    float GetEyeZPosition();
	// Returns the X 'view' direction
    float GetViewXDirection();
    // Returns the Y 'view' direction
    float GetViewYDirection();
    // Returns the Z 'view' direction
    float GetViewZDirection();

    // euler Angles
    float Yaw;
    float Pitch;

    // speed of camera movement
    float speed;

private:

    // Track the old mouse position
    glm::vec2 m_oldMousePosition;
    // Where is our camera positioned
    glm::vec3 position;
    // What direction is the camera looking
    glm::vec3 front;
    // Which direction is 'up' in our world
    // Generally this is constant, but if you wanted
    // to 'rock' or 'rattle' the camera you might play
    // with modifying this value.
    glm::vec3 worldUp;
    // The direction pointing up from the perspective of the camera space (local)
    glm::vec3 up;
    // the direction pointing right from the perspective of the camera space (local)
    glm::vec3 right;
    // with new yaw and pitch values we update the orientation, world and local, of the camera
    void updateCameraVectors();
};




#endif
