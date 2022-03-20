/**
 * @file main.cpp
 * @authors Alejandro Heir, Matías Álvarez
 * @brief Main file
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "OrbitalSim.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "EDA Orbital Simulation - C++ Version");

    // Camera
    raylib::Camera camera(raylib::Vector3(4.0f, 2.0f, 4.0f),
                          raylib::Vector3(0.0f, 1.8f, 0.0f),
                          raylib::Vector3(0.0f, 1.0f, 0.0f),
                          60.0f,
                          CAMERA_PERSPECTIVE);

    // W-A-S-D + mouse to move
    camera.SetMode(CAMERA_FIRST_PERSON);

    // Inicialización default
    // Ver documentación del constructor en el header de la clase para
    // conocer/modificar valores default.
    //
    // OrbitalSim sim;

    // Inicializacion custom
    // Ver documentación del constructor en el header de la clase, o seguir
    // las ayudas/referencias (de existir) del editor de texto al ir ingresando argumentos.
    //
    OrbitalSim sim(100, SOLAR, 10000, false, true, true, true);

    // Game loop
    while (!window.ShouldClose())
    {
        // Update simulation
        //
        // Sobre el window.GetFPS, ver comentario dentro del método update.
        sim.update(window.GetFPS());

        // Camera
        camera.Update();

        // Render
        BeginDrawing();
        {
            window.ClearBackground(BLACK);

            camera.BeginMode();
            sim.render3D();
            DrawGrid(10, 10.0f);
            camera.EndMode();

            sim.render2D();
        }

        EndDrawing();
    }

    return 0;
}