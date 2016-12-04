// David Mihai/William Hess 2017

#include "Tank.h"
#include "Shell.h"
#include "Flak88.h"


// Sets default values
AFlak88::AFlak88()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFlak88::BeginPlay()
{
	Super::BeginPlay();
	
	USkeletalMeshComponent* mesh = CastChecked<USkeletalMeshComponent>(this->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	AnimInstance = Cast<U88Anim>(mesh->GetAnimInstance());
	if (AnimInstance == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get animation instance"));
	}
	AnimInstance->SetParent(this);

	IsLoaded = false;
}

// Called every frame
void AFlak88::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Get Anim Instance
U88Anim* AFlak88::GetAnimInstance()
{
	return this->AnimInstance;
}

// Load the gun
void AFlak88::LoadShell(AActor* Shell)
{
	Shell->DisableComponentsSimulatePhysics();
	Shell->AttachRootComponentToActor(this, "LoadingSocket", EAttachLocation::SnapToTarget);
	((AShell*)Shell)->LoadAnim();
	LoadedShell = Shell;
	IsLoaded = true;
}

// Accessor to toggle breech
void AFlak88::ToggleBreech()
{
	AnimInstance->ToggleBreech();
}

// If the gun can be loaded
bool AFlak88::ReadyToLoad()
{
	return !(AnimInstance->IsBreechClosed);
}

// Destroy the current shell actor on fire
void AFlak88::DestroyLoaded()
{
	LoadedShell->Destroy();
}
