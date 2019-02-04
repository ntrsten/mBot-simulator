#ifndef CAR_H
#define CAR_H

#include "drawable.h"

#define VERTICAL_WALL int('|')
#define HORIZONTAL_WALL int('_')
#define BOTH_WALLS int('L')

struct Settings
{
    int displayWidth;
    int displayHeight;
    bool isFullscreen;
    std::string caption;
    int targetFramerate;
    bool waitOnStart;

    std::string levelFile;
    float tileSize;
    float wallHeight;

    std::string carBodyModel, carBodyTexture;
    std::string carWheelModel, carWheelTexture;
    std::string floorTexture, wallTexture;

    float wheelRadius, wheelWidth, wheelDistance;
    float bodyLength, bodyWidth, bodyHeight;
    float backBodyAxelOffset, bottomBodyAxelOffset;

    float wheelRotationPerSecond;
    float fieldOfView;

};

class Car
{
    public:
        Car(float startX, float startZ, float startAngle, Settings settings);

        void SetLWheelSpeed(float speed);
        void SetRWheelSpeed(float speed);
        void CalculateMovement(std::vector< std::vector< int > > walls, float deltaTime);
        bool CheckCollision(float x, float z, float newX, float newZ, float carAngle, float newCarAngle, std::vector< std::vector< int > > walls);
        float GetUltraSonicReading(std::vector< std::vector< int > > walls);

        void Draw(Shader *shader);

        virtual ~Car();

        float carAngle;
        float x, y, z;

    protected:

    private:
        Drawable *l_wheel, *r_wheel;
        Drawable *body;

        Settings m_settings;

        float lWheelAngle, lWheelSpeed;
        float rWheelAngle, rWheelSpeed;

        float distanceFromCenterToFrontPoint, frontPointAngle;
        float distanceFromCenterToBackPoint, backPointAngle;
};

#endif // CAR_H
