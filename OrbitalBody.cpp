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

void OrbitalBody::setMass(float mass)
{
    this->mass = mass;
}

void OrbitalBody::setColor(raylib::Color color)
{
    this->color = color;
}

void OrbitalBody::setRadius(float radius)
{
    this->radius = radius;
}

raylib::Vector3 OrbitalBody::getAcceleration()
{
    return acceleration;
}

float OrbitalBody::getMass()
{
    return mass;
}


raylib::Vector3 OrbitalBody::getVelocity()
{
    return velocity;
}

float OrbitalBody::getRadius()
{
    return radius;
}


raylib::Vector3 OrbitalBody::getPosition()
{
    return position;
}

raylib::Color OrbitalBody::getColor()
{
    return color;
}
