/*
 * Orbital simulation view
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 */

#include <time.h>

#include "orbitalSimView.h"

const char *getISODate(float currentTime)
{
	// Epoch: 2022-01-01
	struct tm epoch_tm = {0, 0, 0, 1, 0, 122};
	time_t epoch = mktime(&epoch_tm);

	// Convert epoch time to local time
	time_t local_time = epoch + (time_t)currentTime;

	// Print ISO date for local time
	struct tm *local_tm = localtime(&local_time);
	return TextFormat("Date: %04d-%02d-%02d", 1900 + local_tm->tm_year, local_tm->tm_mon + 1, local_tm->tm_mday);
}

void renderOrbitalSim3D(OrbitalSim *sim)
{
	int i;

	for(i = 0; i < sim->bodyNum; i++)
	{
		Vector3 position = Vector3Scale(sim->bodysArray[i]->position, 1E-11);
		Color color = sim->bodysArray[i]->color;

		DrawSphere(position,
				   logf(sim->bodysArray[i]->radius) * 0.005F,
				   color);
		
		DrawPoint3D(position, color);
	}
}

void renderOrbitalSim2D(OrbitalSim *sim)
{
	DrawFPS(0, 0);
	DrawText(getISODate(time(NULL)), 0, 20, 10, GOLD);
}
