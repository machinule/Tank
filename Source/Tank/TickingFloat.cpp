// David Mihai/William Hess 2017

#include "Tank.h"
#include "TickingFloat.h"


TickingFloat::TickingFloat(float Current, float Target, float Interval)
{
	this->Target = Target;
	this->Current = Current;
	this->Interval = Interval;
}

void TickingFloat::Tick(float DeltaTime)
{
	float mult = 1;

	if (Current == Target) {
		return;
	}
	
	float diff = Current - Target;
	if (fabs(diff) <= fabs(Interval)) {
		Current = Target;
	}
	else if (Current > Target) {
		Current = Current - fabs(Interval);
	} 
	else if (Current < Target) {
		Current = Current + fabs(Interval);
	}
}

bool TickingFloat::IsActive() {
	return(Target != Current);
}

void TickingFloat::SetTarget(float Value) {
	this->Target = Value;
}

void TickingFloat::SetInterval(float Value) {
	this->Interval = Value;
}

float TickingFloat::GetCurrent() {
	return Current;
}

bool TickingFloat::IsTickable() const
{
	return true;
}

bool TickingFloat::IsTickableInEditor() const
{
	return false;
}

bool TickingFloat::IsTickableWhenPaused() const
{
	return false;
}

TStatId TickingFloat::GetStatId() const
{
	return TStatId();
}