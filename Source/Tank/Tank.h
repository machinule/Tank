// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __TANK_H__
#define __TANK_H__

#include "EngineMinimal.h"

// Trace channel
#define ECC_InteractChannel ECC_GameTraceChannel2

// Interactive Modules
#define ELEVATION "Elevating-HandwheelSocket"
#define TRAVERSE "Traversing-HandwheelSocket"
#define BREECH_ACTUATOR "Actuating-CylinderSocket"

// Movement Factors and Maxes
#define ELEVATION_FACTOR 0.25
#define TRAVERSE_FACTOR 0.25
#define WHEEL_FACTOR 10
#define ELEVATION_MAX 85/ELEVATION_FACTOR
#define ELEVATION_MIN -8/TRAVERSE_FACTOR
#define BREECH_TRANSLATE -2.0f

#endif
