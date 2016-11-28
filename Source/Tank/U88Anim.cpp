// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TickingFloat.h"
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

	BreechTranslateTicking = new TickingFloat(BREECH_TRANSLATE, BREECH_TRANSLATE, BREECH_TRANSLATE/BREECH_FRAMES);
	BreechTranslate = BREECH_TRANSLATE;

	BreechActuatorTicking = new TickingFloat(90, 90, 90/BREECH_FRAMES);
	BreechActuator = 90;

	FiringHandleTicking = new TickingFloat(0, 0, FIRING_HANDLE_ANGLE/FIRING_FRAMES);
	FiringSwitchTicking = new TickingFloat(0, 0, FIRING_SWITCH_ANGLE/FIRING_FRAMES);

	IsBreechOpen = true;
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
	if (IsBreechOpen)
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
		BreechTranslateTicking->SetTarget(BREECH_TRANSLATE);
		BreechActuatorTicking->SetTarget(90);
		UE_LOG(LogTemp, Warning, TEXT("Opening breech"));
	}
	else
	{
		BreechTranslateTicking->SetTarget(0);
		BreechActuatorTicking->SetTarget(0);
		UE_LOG(LogTemp, Warning, TEXT("Closing breech"));
	}
		IsBreechOpen = State;
}

void U88Anim::Tick(float DeltaTime)
{
	if (BreechTranslateTicking != NULL) {
		BreechTranslate = BreechTranslateTicking->GetCurrent();
	}
	if (BreechActuatorTicking != NULL) {
		BreechActuator = BreechActuatorTicking->GetCurrent();
	}

	if (FiringHandleTicking != NULL) {
		FiringHandle = FiringHandleTicking->GetCurrent();
	}
	if (FiringSwitchTicking != NULL) {
		FiringSwitch = FiringSwitchTicking->GetCurrent();
	}

	if (FiringHandleTicking != NULL && FiringSwitchTicking != NULL) {
		if (!FiringHandleTicking->IsActive() && !FiringSwitchTicking->IsActive() &&
			FiringHandle == FIRING_HANDLE_ANGLE && FiringSwitch == FIRING_SWITCH_ANGLE) {
			FiringHandleTicking->SetTarget(0);
			FiringSwitchTicking->SetTarget(0);
		}
	}
}

void U88Anim::FireHandle() {
	FiringHandleTicking->SetTarget(FIRING_HANDLE_ANGLE);
	FiringSwitchTicking->SetTarget(FIRING_SWITCH_ANGLE);
}

/*
 * METHODS REQUIRED FOR TICKABLE
 */
bool U88Anim::IsTickable() const
{
	return true;
}

bool U88Anim::IsTickableInEditor() const
{
	return false;
}

bool U88Anim::IsTickableWhenPaused() const
{
	return false;
}

TStatId U88Anim::GetStatId() const
{
	return TStatId();
}