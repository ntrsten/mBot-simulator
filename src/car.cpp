#include "car.h"

#include <iostream>

void getCoterminal(float &angle)
{
    if (angle >= 360)
        angle -= 360;
    else if (angle < 0)
        angle += 360;
}

Car::Car(float startX, float startZ, float startAngle, Settings settings):
    carAngle(startAngle),
    x(startX), y(0), z(startZ),
    lWheelAngle(0.0f),
    lWheelSpeed(0.0f),
    rWheelAngle(0.0f),
    rWheelSpeed(0.0f)
{
    m_settings = settings;
    l_wheel = new Drawable(m_settings.carWheelModel.c_str(), m_settings.carWheelTexture.c_str());
    r_wheel = new Drawable(m_settings.carWheelModel.c_str(), m_settings.carWheelTexture.c_str());
    body = new Drawable(m_settings.carBodyModel.c_str(), m_settings.carBodyTexture.c_str());
    distanceFromCenterToFrontPoint = glm::sqrt((m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset)
                                               * (m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset)
                                               + m_settings.bodyWidth * m_settings.bodyWidth);
    distanceFromCenterToBackPoint = glm::sqrt(m_settings.backBodyAxelOffset * m_settings.backBodyAxelOffset
                                               + m_settings.bodyWidth * m_settings.bodyWidth);
    frontPointAngle = glm::asin(m_settings.bodyWidth / distanceFromCenterToFrontPoint);
    backPointAngle = glm::asin(m_settings.bodyWidth / distanceFromCenterToBackPoint);
}

Car::~Car()
{
    delete l_wheel;
    delete r_wheel;
    delete body;
}

void Car::SetRWheelSpeed(float speed)
{
    rWheelSpeed = float(m_settings.wheelRotationPerSecond) / float(1000) * (speed / float(255));
}

void Car::SetLWheelSpeed(float speed)
{
    lWheelSpeed = float(m_settings.wheelRotationPerSecond) / float(1000) * (speed / float(255));
}

void Car::CalculateMovement(std::vector< std::vector< int > > walls, float deltaTime)
{
    float deltaL = glm::radians(lWheelSpeed) * m_settings.wheelRadius * deltaTime;
    float deltaR = glm::radians(rWheelSpeed) * m_settings.wheelRadius * deltaTime;
    float newX, newZ, newCarAngle;
    if (fabs(deltaL - deltaR) < 1.0e-6)
    {
        newX = x + deltaR * sin(glm::radians(carAngle + 90));
        newZ = z + deltaR * cos(glm::radians(carAngle + 90));
        newCarAngle = carAngle;
    }else
    {
        float turnRadius = m_settings.wheelDistance * (deltaL + deltaR) / (2 * (deltaR - deltaL)),
              turnAngle = (deltaR - deltaL) / m_settings.wheelDistance;

        newX = x + (turnRadius * sin(turnAngle + glm::radians(carAngle)) - turnRadius * sin(glm::radians(carAngle)));
        newZ = z + (turnRadius * cos(turnAngle + glm::radians(carAngle)) - turnRadius * cos(glm::radians(carAngle)));

        newCarAngle = carAngle + glm::degrees(turnAngle);
        getCoterminal(newCarAngle);
    }

    if (!this->CheckCollision(x, z, newX, newZ, carAngle, newCarAngle, walls) &&
        //front middle point
        !this->CheckCollision(x - cos(glm::radians(180 - carAngle)) * (m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset),
                              z - sin(glm::radians(180 - carAngle)) * (m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset),
                              newX - cos(glm::radians(180 - newCarAngle)) * (m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset),
                              newZ - sin(glm::radians(180 - newCarAngle)) * (m_settings.bodyLength * 2 - m_settings.backBodyAxelOffset),
                              carAngle, newCarAngle, walls) &&
        //back middle point
        !this->CheckCollision(x + cos(glm::radians(180 - carAngle)) * (m_settings.backBodyAxelOffset),
                              z + sin(glm::radians(180 - carAngle)) * (m_settings.backBodyAxelOffset),
                              newX + cos(glm::radians(180 - newCarAngle)) * (m_settings.backBodyAxelOffset),
                              newZ + sin(glm::radians(180 - newCarAngle)) * (m_settings.backBodyAxelOffset),
                              carAngle, newCarAngle, walls) &&
        //front right point
        !this->CheckCollision(x - cos(glm::radians(180 - carAngle) + frontPointAngle) * (distanceFromCenterToFrontPoint),
                              z - sin(glm::radians(180 - carAngle) + frontPointAngle) * (distanceFromCenterToFrontPoint),
                              newX - cos(glm::radians(180 - newCarAngle) + frontPointAngle) * (distanceFromCenterToFrontPoint),
                              newZ - sin(glm::radians(180 - newCarAngle) + frontPointAngle) * (distanceFromCenterToFrontPoint),
                              carAngle, newCarAngle, walls) &&
        //front left point
        !this->CheckCollision(x - cos(glm::radians(180 - carAngle) - frontPointAngle) * (distanceFromCenterToFrontPoint),
                              z - sin(glm::radians(180 - carAngle) - frontPointAngle) * (distanceFromCenterToFrontPoint),
                              newX - cos(glm::radians(180 - newCarAngle) - frontPointAngle) * (distanceFromCenterToFrontPoint),
                              newZ - sin(glm::radians(180 - newCarAngle) - frontPointAngle) * (distanceFromCenterToFrontPoint),
                              carAngle, newCarAngle, walls) &&
        //back left point
        !this->CheckCollision(x + cos(glm::radians(180 - carAngle) + backPointAngle) * (distanceFromCenterToBackPoint),
                              z + sin(glm::radians(180 - carAngle) + backPointAngle) * (distanceFromCenterToBackPoint),
                              newX + cos(glm::radians(180 - newCarAngle) + backPointAngle) * (distanceFromCenterToBackPoint),
                              newZ + sin(glm::radians(180 - newCarAngle) + backPointAngle) * (distanceFromCenterToBackPoint),
                              carAngle, newCarAngle, walls) &&
        //back right point
        !this->CheckCollision(x + cos(glm::radians(180 - carAngle) - backPointAngle) * (distanceFromCenterToBackPoint),
                              z + sin(glm::radians(180 - carAngle) - backPointAngle) * (distanceFromCenterToBackPoint),
                              newX + cos(glm::radians(180 - newCarAngle) - backPointAngle) * (distanceFromCenterToBackPoint),
                              newZ + sin(glm::radians(180 - newCarAngle) - backPointAngle) * (distanceFromCenterToBackPoint),
                              carAngle, newCarAngle, walls) &&
        //right wheel point
        !this->CheckCollision(x + sin(glm::radians(carAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              z + cos(glm::radians(carAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              newX + sin(glm::radians(newCarAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              newZ + cos(glm::radians(newCarAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              carAngle, newCarAngle, walls) &&
        //right wheel point
        !this->CheckCollision(x - sin(glm::radians(carAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              z - cos(glm::radians(carAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              newX - sin(glm::radians(newCarAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              newZ - cos(glm::radians(newCarAngle)) * (m_settings.wheelDistance / 2 + m_settings.wheelWidth / 10),
                              carAngle, newCarAngle, walls)
        )
    {
        rWheelAngle -= rWheelSpeed * deltaTime;
        getCoterminal(rWheelAngle);
        lWheelAngle -= lWheelSpeed * deltaTime;
        getCoterminal(lWheelAngle);

        x = newX;
        z = newZ;
        carAngle = newCarAngle;
    }
}

bool Car::CheckCollision(float x, float z, float newX, float newZ, float carAngle, float newCarAngle, std::vector< std::vector< int > > walls)
{

    int Xindex = (x + m_settings.tileSize / 2) / m_settings.tileSize, Zindex = (z + m_settings.tileSize / 2) / m_settings.tileSize;
    int newXindex = (newX + m_settings.tileSize / 2) / m_settings.tileSize, newZindex = (newZ + m_settings.tileSize / 2) / m_settings.tileSize;
    if (newXindex < 0 || newZindex < 0 || (unsigned int)newXindex >= walls.size() || Xindex < 0 || Zindex < 0 || (unsigned int)Xindex >= walls.size())
    {
        return false;
    }
    else if ((unsigned int)newZindex >= walls[newXindex].size() || (unsigned int)Zindex >= walls[Xindex].size())
    {
        return false;
    }
    if (Xindex < newXindex)
        if (walls[newXindex][newZindex] == HORIZONTAL_WALL || walls[newXindex][newZindex] == BOTH_WALLS)
            return true;
    if (Xindex > newXindex)
        if (walls[Xindex][Zindex] == HORIZONTAL_WALL || walls[Xindex][Zindex] == BOTH_WALLS)
            return true;
    if (Zindex < newZindex)
        if (walls[newXindex][newZindex] == VERTICAL_WALL || walls[newXindex][newZindex] == BOTH_WALLS)
            return true;
    if (Zindex > newZindex)
        if (walls[Xindex][Zindex] == VERTICAL_WALL || walls[Xindex][Zindex] == BOTH_WALLS)
            return true;
    return false;
}

float Car::GetUltraSonicReading(std::vector< std::vector< int > > walls)
{
    bool isOver = false;
    float lastX = x + (m_settings.bodyLength / 2 + m_settings.backBodyAxelOffset) * sin(glm::radians(carAngle + 90));
    float lastZ = z + (m_settings.bodyLength / 2 + m_settings.backBodyAxelOffset) * cos(glm::radians(carAngle + 90));
    float calculatedDistance = 0.0f;
    float newX, newZ;
    int lastXindex, lastZindex;
    int newXindex, newZindex;
    while(!isOver)
    {
        if (calculatedDistance >= 50.00)
        {
            isOver = true;
            continue;
        }
        calculatedDistance += 0.01f;
        newX = lastX + 0.01 * sin(glm::radians(carAngle + 90));
        newZ = lastZ + 0.01 * cos(glm::radians(carAngle + 90));
        lastXindex = (lastX + m_settings.tileSize / 2) / m_settings.tileSize, lastZindex = (lastZ + m_settings.tileSize / 2) / m_settings.tileSize;
        newXindex = (newX + m_settings.tileSize / 2) / m_settings.tileSize, newZindex = (newZ + m_settings.tileSize / 2) / m_settings.tileSize;

        if (newXindex < 0 || newZindex < 0 || (unsigned int)newXindex >= walls.size() || lastXindex < 0 || lastZindex < 0 || (unsigned int)lastXindex >= walls.size())
        {
            lastX = newX;
            lastZ = newZ;
            continue;
        }
        else if ((unsigned int)newZindex >= walls[newXindex].size() || (unsigned int)lastZindex >= walls[lastXindex].size())
        {
            lastX = newX;
            lastZ = newZ;
            continue;
        }

        if (lastXindex < newXindex)
            if (walls[newXindex][newZindex] == HORIZONTAL_WALL || walls[newXindex][newZindex] == BOTH_WALLS)
                isOver = true;
        if (lastXindex > newXindex)
            if (walls[lastXindex][lastZindex] == HORIZONTAL_WALL || walls[lastXindex][lastZindex] == BOTH_WALLS)
                isOver = true;
        if (lastZindex < newZindex)
            if (walls[newXindex][newZindex] == VERTICAL_WALL || walls[newXindex][newZindex] == BOTH_WALLS)
                isOver = true;
        if (lastZindex > newZindex)
            if (walls[lastXindex][lastZindex] == VERTICAL_WALL || walls[lastXindex][lastZindex] == BOTH_WALLS)
                isOver = true;

        lastX = newX;
        lastZ = newZ;
    }
    return calculatedDistance * 10;
}

void Car::Draw(Shader *shader)
{
    l_wheel->Draw(shader,
                  glm::vec3(x - sin(glm::radians(carAngle)) * (m_settings.wheelDistance / 2),
                            y + m_settings.wheelRadius,
                            z - cos(glm::radians(carAngle)) * (m_settings.wheelDistance / 2)),
                  carAngle, lWheelAngle,
                  glm::normalize(glm::vec3(sin(glm::radians(carAngle)), 0, cos(glm::radians(carAngle)))),
                  glm::vec3(m_settings.wheelRadius, m_settings.wheelRadius, m_settings.wheelWidth));
    r_wheel->Draw(shader,
                  glm::vec3(x + sin(glm::radians(carAngle)) * (m_settings.wheelDistance / 2),
                            y + m_settings.wheelRadius,
                            z + cos(glm::radians(carAngle)) * (m_settings.wheelDistance / 2)),
                  carAngle, rWheelAngle,
                  glm::normalize(glm::vec3(sin(glm::radians(carAngle)), 0, cos(glm::radians(carAngle)))),
                  glm::vec3(m_settings.wheelRadius, m_settings.wheelRadius, m_settings.wheelWidth));
    body->Draw(shader,
               glm::vec3(x - cos(glm::radians(180 - carAngle)) * (m_settings.bodyLength - m_settings.backBodyAxelOffset),
                         y + m_settings.wheelRadius + m_settings.bodyHeight - m_settings.bottomBodyAxelOffset,
                         z - sin(glm::radians(180 - carAngle)) * (m_settings.bodyLength - m_settings.backBodyAxelOffset)),
               carAngle, 0,
               glm::normalize(glm::vec3(sin(glm::radians(carAngle)), 0, cos(glm::radians(carAngle)))),
               glm::vec3(m_settings.bodyLength, m_settings.bodyHeight, m_settings.bodyWidth));
}
