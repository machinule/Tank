// David Mihai/William Hess 2017

#pragma once

#include "Engine/StaticMeshActor.h"
#include "TankCharacter.h"
#include "Shell.generated.h"

UCLASS()//transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class TANK_API AShell : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShell(const FObjectInitializer& ObjectInitializer);

	/** Actor currently grabbing this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbing)
		ATankCharacter* Grabber;

	/** Actor currently grabbing this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grabbing)
		bool IsGrabbed;

	// Translate the shell into the breech
	UFUNCTION(BlueprintImplementableEvent, Category = Charging)
		void LoadAnim();
};
