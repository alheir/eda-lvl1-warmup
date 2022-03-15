/**
 * @file main.cpp
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation. Main module
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "orbitalSim.h"
#include "orbitalSimView.h"
#include <stdio.h>

#define SECONDS_PER_DAY 86400.0F

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "EDA Orbital Simulation");

    // Camera
    Camera3D camera;
    camera.position = {10.0f, 10.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FREE);

    SetCameraPanControl(MOUSE_BUTTON_LEFT);

    // Orbital simulation
    const float fps = 60.0F;                            // frames per second
    const float timeMultiplier = DAYS_PER_SECOND * SECONDS_PER_DAY; // Simulation speed: days per real second
    const float timeStep = timeMultiplier / fps;

    OrbitalSim *sim = makeOrbitalSim(timeStep);

    if (!sim)
    {
        printf("No se pudo inicializar orbitalSim...\n");
        CloseWindow();
        return 1;
    }

    // Game loop
    while (!WindowShouldClose())
    {
        // Update simulation
        updateOrbitalSim(sim);

        // Camera
        UpdateCamera(&camera);

        // Render
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        renderOrbitalSim3D(sim);
        DrawGrid(10, 10.0f);
        EndMode3D();

        renderOrbitalSim2D(sim);
        EndDrawing();

        // Se hacen coincidir FPS de raylib con los de la cuenta de timeStep para que
        // este ultimo siempre "esté bien", sin importar los fps de raylib.
        // En un principio se ponía un topo a los fps con SetTargetFPS, pero si eran
        // menores al seteado, el avance temporal quedaba mal.
        //
        // Luego, se consigue NO limitar los fps, y que el timeStep siempre sea correcto.
        //
        // No se notaron impactos en el rendimiento al agregar una división en cada vuelta del loop
        sim->timeStep = timeMultiplier / GetFPS();
    }

    CloseWindow();

    freeOrbitalSim(sim);

    return 0;
}
