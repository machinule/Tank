// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "TickingFloat.h"
#include "U88Anim.generated.h"

UCLASS()//transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class TANK_API U88Anim : public UAnimInstance, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	U88Anim(const FObjectInitializer& ObjectInitializer);

	// Required overrides for tickable
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	/** Rate of horizontal traverse */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float Traverse;

	/** Rate of vertical elevation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float Elevation;

	/** Factor of vertical elevation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float TraverseFactor;

	/** Factor of vertical elevation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float ElevationFactor;

	/** Factor of handwheel rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float WheelFactor;

	/** Breech translate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float BreechTranslate;

	/** Breech actuator angle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		int BreechActuator;

	/** Angle of the firing switch (upper portion of firing mechanism) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float FiringSwitch;

	/** Angle of the firing handle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float FiringHandle;

	/** Setters and validators for modules*/
	void SetTraverse(float Value);
	void SetElevation(float Value);

	void ToggleBreech();
	void SetBreech(bool State);
	
	bool IsBreechOpen;

	void FireHandle();
	
	TickingFloat* BreechTranslateTicking;
	TickingFloat* BreechActuatorTicking;
	
	TickingFloat* FiringHandleTicking;
	TickingFloat* FiringSwitchTicking;
};
