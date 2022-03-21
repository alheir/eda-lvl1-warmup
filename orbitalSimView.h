/**
 * @file orbitalSimView.h
 * @authors Marc Ressl - Alejandro Heir, Matías Álvarez
 * @brief Orbital simulation view. Módulo gráfico
 * @version 0.1
 * @date 2022-03-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include "orbitalSim.h"

// Renders the 3D views of a given orbital simulation
void renderOrbitalSim3D(OrbitalSim *sim);

// Renders simulation data
void renderOrbitalSim2D(OrbitalSim *sim);

#endif
