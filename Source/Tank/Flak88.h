// David Mihai/William Hess 2017

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "U88Anim.h"
#include "Flak88.generated.h"

UCLASS(config = Game)
class TANK_API AFlak88 : public ASkeletalMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlak88();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Get animation instance
	U88Anim* GetAnimInstance();

	// If the gun can be loaded
	UFUNCTION(BlueprintCallable, Category = Charging)
		void LoadShell(AActor* Shell);

	// Toggle the breech
	UFUNCTION(BlueprintCallable, Category = Charging)
		void ToggleBreech();

	// Play the sound of the breech
	UFUNCTION(BlueprintImplementableEvent, Category = Charging)
		void BreechNoise();

	// If the gun can be loaded
	UFUNCTION(BlueprintCallable, Category = Charging)
		bool ReadyToLoad();

	// Fire the gun, spawn the canister and shell
	UFUNCTION(BlueprintImplementableEvent, Category = Firing)
		void FireShell();

	AActor* LoadingCollider;

	bool IsLoaded;

	void DestroyLoaded();
	AActor* LoadedShell;

protected:
	// Animation instance
	U88Anim* AnimInstance;
};
