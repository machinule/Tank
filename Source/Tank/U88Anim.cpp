// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "U88Anim.h"

// Sets default values
U88Anim::U88Anim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Elevation = 0.0f;
	Traverse = 0.0f;
	ElevationFactor = ELEVATION_FACTOR;
	TraverseFactor = TRAVERSE_FACTOR;
	WheelFactor = WHEEL_FACTOR;		
	BreechTranslate = 0.0f;
	BreechActuator = 0;
}

void U88Anim::SetTraverse(float Value) {
	this->Traverse = this->Traverse + Value;
}

void U88Anim::SetElevation(float Value) {
	float result = this->Elevation + Value;
	if (result > ELEVATION_MAX) {
		this->Elevation = ELEVATION_MAX;
		return;
	}
	if (result < ELEVATION_MIN) {
		this->Elevation = ELEVATION_MIN;
		return;
	}
	this->Elevation = result;
}

void U88Anim::ToggleBreech() {
	if (BreechActuator == 90)
	{
		SetBreech(false);
	}
	else
	{
		SetBreech(true);
	}
}

void U88Anim::SetBreech(bool State) {
	if (State)
	{
		BreechActuator = 90;
		BreechTranslate = BREECH_TRANSLATE;
	}
	else
	{
		BreechActuator = 0;
		BreechTranslate = 0.0f;
	}
}