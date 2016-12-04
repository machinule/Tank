// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TickingFloat.h"
#include "U88Anim.h"
#include "Flak88.h"

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

	BreechActuatorTicking = new TickingFloat(0, 0, 90/BREECH_FRAMES);
	BreechActuator = 0;

	FiringHandleTicking = new TickingFloat(0, 0, FIRING_HANDLE_ANGLE/FIRING_FRAMES);
	FiringSwitchTicking = new TickingFloat(0, 0, FIRING_SWITCH_ANGLE/FIRING_FRAMES);

	RecoilAngleTicking= new TickingFloat(0, 0, 1);
	RecoilTranslateTicking = new TickingFloat(0, 0, 1);

	IsBreechClosed = true;

	Recoil = 0.0f;
}

// Sets parent gun 
void U88Anim::SetParent(AFlak88* Parent)
{
	this->Parent = Parent;
}

void U88Anim::SetTraverse(float Value) {
	this->Traverse = this->Traverse + Value;
}

void U88Anim::SetElevation(float Value) {
	float result = this->ElevationRaw + Value;
	if (result > ELEVATION_MAX) {
		this->ElevationRaw = ELEVATION_MAX;
		return;
	}
	if (result < ELEVATION_MIN) {
		this->ElevationRaw = ELEVATION_MIN;
		return;
	}
	this->ElevationRaw = result;
}

void U88Anim::ToggleBreech() {
	if (IsBreechClosed)
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
		BreechActuatorTicking->SetTarget(0);
		UE_LOG(LogTemp, Warning, TEXT("Closing breech"));
	}
	else
	{
		BreechTranslateTicking->SetTarget(0);
		BreechActuatorTicking->SetTarget(90);
		UE_LOG(LogTemp, Warning, TEXT("Opening breech"));
	}
	Parent->BreechNoise();
	IsBreechClosed = State;
}

void U88Anim::Tick(float DeltaTime)
{
	if (BreechTranslateTicking != NULL) {
		BreechTranslate = BreechTranslateTicking->GetCurrent();
	}
	if (BreechActuatorTicking != NULL) {
		BreechActuator = BreechActuatorTicking->GetCurrent();
	}

	if (RecoilTranslateTicking != NULL) {
		Recoil = RecoilTranslateTicking->GetCurrent();
	}
	if (RecoilAngleTicking != NULL)
	{
		Elevation = ElevationRaw + RecoilAngleTicking->GetCurrent();
	}

	if (FiringHandleTicking != NULL) {
		FiringHandle = FiringHandleTicking->GetCurrent();
	}
	if (FiringSwitchTicking != NULL) {
		FiringSwitch = FiringSwitchTicking->GetCurrent();
	}

	if (FiringHandleTicking != NULL && FiringSwitchTicking != NULL)
	{
		if (!FiringHandleTicking->IsActive() && !FiringSwitchTicking->IsActive() &&
			FiringHandle == FIRING_HANDLE_ANGLE && FiringSwitch == FIRING_SWITCH_ANGLE)
		{
			FiringHandleTicking->SetTarget(0);
			FiringSwitchTicking->SetTarget(0);
		}
	}

	if (RecoilTranslateTicking != NULL && RecoilAngleTicking != NULL)
	{
		if (!RecoilTranslateTicking->IsActive() && !RecoilAngleTicking->IsActive() &&
			RecoilTranslateTicking->GetCurrent() == RECOIL_TRANSLATE &&
			RecoilAngleTicking->GetCurrent() == RECOIL_ANGLE)
		{
			RecoilTranslateTicking->SetInterval(RECOIL_TRANSLATE/RECOIL_RECOVER_FRAMES);
			RecoilAngleTicking->SetInterval(RECOIL_ANGLE/RECOIL_RECOVER_FRAMES);
			RecoilTranslateTicking->SetTarget(0);
			RecoilAngleTicking->SetTarget(0);
		}
	}
}

void U88Anim::FireHandle()
{
	FiringHandleTicking->SetTarget(FIRING_HANDLE_ANGLE);
	FiringSwitchTicking->SetTarget(FIRING_SWITCH_ANGLE);
	if (Parent->IsLoaded)
	{
		Fire();
	}
}

void U88Anim::Fire()
{
	if (IsBreechClosed)
	{
		Parent->IsLoaded = false;
		this->ToggleBreech();
		RecoilTranslateTicking->SetInterval(RECOIL_TRANSLATE / RECOIL_FRAMES);
		RecoilAngleTicking->SetInterval(RECOIL_ANGLE / RECOIL_FRAMES);
		RecoilTranslateTicking->SetTarget(RECOIL_TRANSLATE);
		RecoilAngleTicking->SetTarget(RECOIL_ANGLE);
		Parent->DestroyLoaded();
		Parent->FireShell();
	}
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