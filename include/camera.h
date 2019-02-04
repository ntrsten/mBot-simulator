#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float CAMERA_SPEED    = 1;
const float SENSITIVITY     = 5;

class Camera
{
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = 0.0f, float pitch = -45.0f);

        glm::mat4 GetViewMatrix();
        void SetCamera(glm::vec3 position, float yaw, float pitch);
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset);
        glm::vec3 Position;
        glm::vec3 Front, Up, Right;
        glm::vec3 WorldUp;
        float Yaw, Pitch;

        virtual ~Camera();

    protected:

    private:
        float MovementSpeed;
        float MouseSensitivity;

        void updateCameraVectors();
};

#endif // CAMERA_H
