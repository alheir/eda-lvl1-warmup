/**
 * @file OrbitalBody.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "OrbitalBody.h"

void OrbitalBody::setAcceleration(raylib::Vector3 acceleration)
{
    this->acceleration = acceleration;
}

void OrbitalBody::setVelocity(raylib::Vector3 velocity)
{
    this->velocity = velocity;
}

void OrbitalBody::setPosition(raylib::Vector3 position)
{
    this->position = position;
}

void OrbitalBody::setMass(raylib::Vector3 mass)
{
    this->mass = mass;
}

void OrbitalBody::setColor(raylib::Color color)
{
    this->color = color;
}

raylib::Vector3 OrbitalBody::getAcceleration()
{
    return acceleration;
}

raylib::Vector3 OrbitalBody::getMass()
{
    return mass;
}


raylib::Vector3 OrbitalBody::getVelocity()
{
    return velocity;
}


raylib::Vector3 OrbitalBody::getPosition()
{
    return position;
}
