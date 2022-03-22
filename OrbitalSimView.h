/**
 * @file OrbitalSimView.h
 * @authors Alejandro Heir, Matías Álvarez
 * @brief Clase OrbitalSimView, para hacer actualizaciones gráficas de una simulación de sistema planetario
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include "OrbitalSim.h"

class OrbitalSimView
{
public:
    void render2D(OrbitalSim &sim);
    void render3D(OrbitalSim &sim);

private:
};

#endif // ORBITALSIMVIEW_H
