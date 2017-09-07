//
// Created by eustace on 06/04/17.
//

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM),
        m_Controllable(false)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
               GLfloat upX, GLfloat upY, GLfloat upZ,
               GLfloat yaw, GLfloat pitch)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM),
        m_Controllable(false)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::processKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    if (!m_Controllable) return;
    GLfloat velocity = deltaTime * this->speed;
    if(direction == FORWARD)
        this->position += velocity * this->front;
    if(direction == BACKWARD)
        this->position -= velocity * this->front;
    if(direction == LEFT)
        this->position -= velocity * this->right;
    if(direction == RIGHT)
        this->position += velocity * this->right;
    this->updateCameraVectors();
}

void Camera::processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
    if (!m_Controllable) return;
    xOffset *= this->sensitivity;
    yOffset *= this->sensitivity;
    this->yaw += xOffset;
    this->pitch += yOffset;
    if (constrainPitch)
    {
        if (this->pitch > 89.f) this->pitch = 89.f;
        else if (this->pitch < -89.f) this->pitch = -89.f;
    }
    this->updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset)
{
    if (!m_Controllable) return;
    if (this->zoom >= 1.f && this->zoom <= 45.f) this->zoom -= yOffset;
    if (this->zoom < 1.f) this->zoom = 1.f;
    if (this->zoom > 45.f) this->zoom = 45.f;
    this->updateCameraVectors();
}

void Camera::toggleControls()
{
    m_Controllable = !m_Controllable;
}

CameraState Camera::getState()
{
    return CameraState(this->position, this->yaw, this->pitch);
}

void Camera::setState(const CameraState & state)
{
    this->position = state.position;
    this->yaw = state.yaw;
    this->pitch = state.pitch;
    this->updateCameraVectors();
}

glm::vec3 Camera::getPos()
{
    return this->position;
}

void Camera::setPos(const glm::vec3 &value)
{
    this->position = value;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    front.y = sin(glm::radians(this->pitch));
    front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}