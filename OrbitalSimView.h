/**
 * @file OrbitalSimView.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-21
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
    void render2D(const OrbitalSim &sim);
    void render3D(const OrbitalSim &sim);

private:
};

#endif // ORBITALSIMVIEW_H
