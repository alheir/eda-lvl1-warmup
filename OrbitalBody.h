/**
 * @file OrbitalBody.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "raylib-cpp.hpp"

class OrbitalBody
{

using namespace raylib;


public:
    OrbitalBody();
    ~OrbitalBody();

    void setAcceleration(Vector3 acceleration);
    void setVelocity(Vector3 velocity);
    void setPosition(Vector3 position);
    void setMass(Vector3 mass);
    void setColor(Color color);

    Vector3 getAceleration();
    Vector3 getMass();
    Vector3 getVelocity();

private:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;
    float mass;
    float radius;
    Color color;
}