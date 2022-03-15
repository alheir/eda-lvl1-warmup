/**
 * @file OrbitalSim.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-15
 *
 * @copyright Copyright (c) 2022
 *
 */

enum PLANETARY_SYSTEM
{
    SOLAR,
    ALPHACENTAURI
};

class OrbitalSim
{
public:
    OrbitalSim(float timeStep,
               int system = SOLAR,
               bool setBlackHole = false,
               bool tweakJupiterMass = false,
               unsigned int asteroidsNum = 0,
               bool partyTime = false);

    ~OrbitalSim();

    void update();

    float getTimeStep();
    float getTime();
    int getBodyNumCore();
    int getBodyNum();
    void setTimeStep(float timeStep);

private:
    float timeStep;
    float time;
    int bodyNumCore;
    int bodyNum;

    // OrbitalBody **bodies;
}