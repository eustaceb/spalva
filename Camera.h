#ifndef _Camera_H_
#define _Camera_H_

#define GLEW_STATIC
#define GLEW_NO_GLU
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
    BACKWARD, FORWARD,
    LEFT, RIGHT
};

struct CameraState {
    glm::vec3 position;
    GLfloat yaw;
    GLfloat pitch;
    CameraState(){};
    CameraState(glm::vec3 position, GLfloat yaw, GLfloat pitch)
        : position(position), yaw(yaw), pitch(pitch) {};
};


const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.f;
const GLfloat ZOOM = 45.f;
const GLfloat SPEED = 5.f;
const GLfloat SENSITIVITY = 0.4f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat speed;
    GLfloat sensitivity;
    GLfloat zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = YAW, GLfloat pitch = PITCH);

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ,
           GLfloat yaw = YAW, GLfloat pitch = PITCH);

    glm::mat4 getViewMatrix();
    void processKeyboard(CameraMovement direction, GLfloat deltaTime);
    void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat yOffset);
    void toggleControls();

    CameraState getState();
    void setState(const CameraState &state);

    glm::vec3 getPos();
    void setPos(const glm::vec3 &value);
    //void debug();

private:
    void updateCameraVectors();
    bool m_Controllable;
};


#endif //_Camera_H_
