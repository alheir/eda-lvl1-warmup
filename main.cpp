/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include "OrbitalSim.h"

#define SECONDS_PER_DAY 86400.0F

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "EDA Orbital Simulation");

    // Camera
    raylib::Camera camera(raylib::Vector3(4.0f, 2.0f, 4.0f),
                          raylib::Vector3(0.0f, 1.8f, 0.0f),
                          raylib::Vector3(0.0f, 1.0f, 0.0f),
                          60.0f,
                          CAMERA_PERSPECTIVE);

    camera.SetMode(CAMERA_FIRST_PERSON);

    // Orbital simulation
    const int daysPerSecond = 100;
    const float fps = 60.0F;                            // frames per second
    const float timeMultiplier = daysPerSecond * SECONDS_PER_DAY; // Simulation speed: days per real second
    const float timeStep = timeMultiplier / fps;

    OrbitalSim sim(timeStep, ALPHACENTAURI);

    // Game loop
    while (!window.ShouldClose())
    {
        // Update simulation
        sim.update();

        // Camera
        camera.Update();

        // Render
        BeginDrawing();
        window.ClearBackground(BLACK);

        camera.BeginMode();
        sim.render3D();
        DrawGrid(10, 10.0f);
        camera.EndMode();

        sim.render2D();
        EndDrawing();

        // Se hacen coincidir FPS de raylib con los de la cuenta de timeStep para que
        // este ultimo siempre "esté bien", sin importar los fps de raylib.
        // En un principio se ponía un topo a los fps con SetTargetFPS, pero si eran
        // menores al seteado, el avance temporal quedaba mal.
        //
        // Luego, se consigue NO limitar los fps, y que el timeStep siempre sea correcto.
        //
        // No se notaron impactos en el rendimiento al agregar una división en cada vuelta del loop
        sim.setTimeStep(timeMultiplier / window.GetFPS());
    }

    window.Close();

    return 0;
}