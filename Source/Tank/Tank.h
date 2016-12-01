// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __TANK_H__
#define __TANK_H__

#include "EngineMinimal.h"

// Trace channel
#define ECC_InteractChannel ECC_GameTraceChannel2
#define ECC_GrabbableChannel ECC_GameTraceChannel3

// Interactive Modules
#define ELEVATION "Elevating-HandwheelSocket"
#define TRAVERSE "Traversing-HandwheelSocket"
#define BREECH_ACTUATOR "Actuating-CylinderSocket"
#define FIRING_HANDLE "Firing-HandleSocket"

// Movement Factors and Maxes
#define ELEVATION_FACTOR 0.25
#define TRAVERSE_FACTOR 0.25
#define WHEEL_FACTOR 10
#define ELEVATION_MAX 85/ELEVATION_FACTOR
#define ELEVATION_MIN -8/TRAVERSE_FACTOR
#define BREECH_TRANSLATE -2.0f
#define FIRING_HANDLE_ANGLE 10.0f
#define FIRING_SWITCH_ANGLE 8.0f

#define BREECH_FRAMES 10.0F
#define FIRING_FRAMES 10.0F

#endif
