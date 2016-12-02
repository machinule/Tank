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

	// If the gun can be loaded
	UFUNCTION(BlueprintCallable, Category = Charging)
		void ToggleBreech();

	// If the gun can be loaded
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = Charging)
		bool ReadyToLoad();

	AActor* LoadingCollider;

	bool IsLoaded;

protected:
	// Animation instance
	U88Anim* AnimInstance;
};
