// David Mihai/William Hess 2017

#pragma once

#include "Tickable.h"

class TickingFloat : public FTickableGameObject
{

public:

	TickingFloat(float Current, float Target, float Interval);

	float GetCurrent();
	void SetTarget(float Value);
	void SetInterval(float Value);

	bool IsActive();

	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

protected:
	
	//Target to reach
	float Target;

	//Current value
	float Current;

	//Interval by which we move towards target
	float Interval;
};
