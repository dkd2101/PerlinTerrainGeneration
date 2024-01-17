#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

void Camera::MouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    // Invert Y
    glm::vec2 newMousePosition(mouseX, -mouseY);
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.5f*(newMousePosition-m_oldMousePosition);
    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;

    // update pitch and yaw
    Yaw += mouseDelta.x;
    Pitch += mouseDelta.y;

    // make sure we do not flip over
    if (Pitch > 89.0f){
        Pitch = 89.0f;
    }
    if (Pitch < -89.0f){
        Pitch = -89.0f;
    }

    // update everything
    updateCameraVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(camMove direction)
{
    // using the enum we find what direction we want to move in
    if (direction == FORWARD){
        position += front * speed;
    }
    if (direction == BACKWARD){
        position -= front * speed;
    }
    if (direction == LEFT){
        position -= right * speed;
    }
    if (direction == RIGHT){
        position += right * speed;
    }
    if (direction == UP){
        position += worldUp * speed;
    }
    if (direction == DOWN){
        position -= worldUp * speed;
    }
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    position.x = x;
    position.y = y;
    position.z = z;
}

float Camera::GetEyeXPosition(){
    return position.x;
}

float Camera::GetEyeYPosition(){
    return position.y;
}

float Camera::GetEyeZPosition(){
    return position.z;
}

float Camera::GetViewXDirection(){
    return front.x;
}

float Camera::GetViewYDirection(){
    return front.y;
}

float Camera::GetViewZDirection(){
    return front.z;
}


Camera::Camera(){
    //std::cout << "(Constructor) Created a Camera!\n";
	// Position us at the origin.
    position = glm::vec3(0.0f,0.0f, 0.0f);
	// Looking down along the z-axis initially.
	// Remember, this is negative because we are looking 'into' the scene.
    front = glm::vec3(0.0f,0.0f, -1.0f);
	// For now--our upVector always points up along the y-axis
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // Our defaults for camera direction
    Pitch = 0.0f;
    Yaw = -90.0f;
    speed = 2.5f;
    updateCameraVectors();
}

glm::mat4 Camera::GetWorldToViewmatrix() const{
    // Think about the second argument and why that is
    // setup as it is.
    return glm::lookAt( position,
                        position + front,
                        up);
}

void Camera::updateCameraVectors(){
    // calculate the new Front vector
    glm::vec3 tempFront;

    tempFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    tempFront.y = sin(glm::radians(Pitch));
    tempFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    front = glm::normalize(tempFront); // normalize to rebase

    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
