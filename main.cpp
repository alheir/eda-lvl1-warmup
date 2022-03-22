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
#include "OrbitalSimView.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "EDA Orbital Simulation - C++ Version");

    // Camera
    raylib::Camera camera(raylib::Vector3(10.0f, 10.0f, 10.0f),
                          raylib::Vector3(0.0f, 0.0f, 0.0f),
                          raylib::Vector3(0.0f, 1.0f, 0.0f),
                          60.0f,
                          CAMERA_PERSPECTIVE);

    camera.SetMode(CAMERA_FREE);
    SetCameraPanControl(MOUSE_BUTTON_LEFT);

    // Inicialización default
    // Ver documentación del constructor en el header de la clase para
    // conocer/modificar valores default.
    //
    // OrbitalSim sim;

    // Inicializacion custom
    // Ver documentación del constructor en el header de la clase, o seguir
    // las ayudas/referencias (de existir) del editor de texto al ir ingresando argumentos.
    //
    // Se llegó a 30FPS con 50000 asteroides, en Linux
    //
    OrbitalSim sim(100, SOLAR, 10000, false, false, true, true);

    OrbitalSimView renderizator;

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
            renderizator.render3D(sim);
            DrawGrid(10, 10.0f);
            camera.EndMode();

            renderizator.render2D(sim);
        }

        EndDrawing();
    }

    return 0;
}