/**
 * @file OrbitalBody.h
 * @authors Alejandro Heir, Matías Álvarez
 * @brief Clase OrbitalBody, para implementar cuerpos en la simulación
 * de sistema planetario (OrbitalSim)
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ORBITALBODY_H
#define ORBITALBODY_H

#include "raylib-cpp.hpp"

class OrbitalBody
{
public:
    // Setters
    void setAcceleration(raylib::Vector3 acceleration);
    void setVelocity(raylib::Vector3 velocity);
    void setPosition(raylib::Vector3 position);
    void setMass(float mass);
    void setColor(raylib::Color color);
    void setRadius(float radius);

    // Getters
    raylib::Vector3 getAcceleration();
    float getMass();
    raylib::Vector3 getVelocity();
    float getRadius();
    raylib::Vector3 getPosition();
    raylib::Color getColor();

private:
    raylib::Vector3 position;
    raylib::Vector3 velocity;
    raylib::Vector3 acceleration;
    float mass;
    float radius;
    raylib::Color color;
};

#endif // ORBITALBODY_H
