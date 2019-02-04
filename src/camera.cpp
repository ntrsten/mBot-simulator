#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up,
               float yaw, float pitch):
    Position(position),
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    WorldUp(up),
    Yaw(yaw),
    Pitch(pitch),
    MovementSpeed(CAMERA_SPEED / 100),
    MouseSensitivity(SENSITIVITY / 100)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == UP)
        Position += Up * velocity;
    if (direction == DOWN)
        Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    //get jaw and pitch angles
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    //from spherical to Cartesian coordinates
    glm::vec3 fwd;
    fwd.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    fwd.y = sin(glm::radians(Pitch));
    fwd.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(fwd);

    //get reference vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::SetCamera(glm::vec3 position, float yaw, float pitch)
{
    this->Position = position;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

Camera::~Camera()
{

}
