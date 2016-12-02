// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Tank.h"
#include "TankCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "U88Anim.h"
#include "Shell.h"

#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ATankCharacter

ATankCharacter::ATankCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	PlayerInteractionDistance = 250;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	IsGrabbing = false;
}

void ATankCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATankCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATankCharacter::TouchStarted);
	if (EnableTouchscreenMovement(PlayerInputComponent) == false)
	{
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankCharacter::OnFire);
	}

	PlayerInputComponent->BindAction("Grab", IE_Pressed, this, &ATankCharacter::OnGrab);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATankCharacter::OnResetVR);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankCharacter::MoveRight);

	//Input to interact w/ modules - set in Project Properties in Editor
	PlayerInputComponent->BindAxis("Interact", this, &ATankCharacter::Interact);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATankCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATankCharacter::LookUpAtRate);
}

void ATankCharacter::Tick(float DeltaTime)
{
	//After ever tick
	Super::Tick(DeltaTime);
}

void ATankCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATankCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ATankCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ATankCharacter::OnFire()
{
	// try and play the sound if specified
	if (FireSound != NULL)
	{
		//Annoying fire sound that I commented out to save my sanity
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	FHitResult HitData = Trace(ECollisionChannel::ECC_InteractChannel);
	bool HitActor = HitData.GetActor() != NULL;

	if (HitActor) {
		//Get the name of the bone the collider is attached to
		FName interactionBone = HitData.GetActor()->GetAttachParentSocketName();
		AActor* Arty = HitData.GetActor()->GetAttachParentActor();

		//Get the skeletal mesh of the 88
		if (Arty != NULL) {
			USkeletalMeshComponent* mesh = CastChecked<USkeletalMeshComponent>(Arty->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
			U88Anim * Animation = Cast<U88Anim>(mesh->GetAnimInstance());
			if (interactionBone.ToString() == BREECH_ACTUATOR) {
				Animation->ToggleBreech();
			}
			if (interactionBone.ToString() == FIRING_HANDLE) {
				Animation->FireHandle();
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("88 Not Found"));
		}
	}
}

void ATankCharacter::OnGrab()
{
	if (!IsGrabbing)
	{
		FHitResult GrabResult = Trace(ECollisionChannel::ECC_GrabbableChannel);
		if (GrabResult.GetActor() != NULL)
		{
			GrabTrace(GrabResult);
		}
	}
	else
	{
		DropGrabbable();
		ClearGrabbed();
	}
}

void ATankCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ATankCharacter::ClearGrabbed()
{
	Grabbed = NULL;
}

void ATankCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATankCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATankCharacter::Interact(float Value)
{
	if (Value != 0.0f)
	{
		FHitResult HitData = Trace(ECollisionChannel::ECC_InteractChannel);
		bool HitActor = HitData.GetActor() != NULL;

		if (HitActor) {
			//Get the name of the bone the collider is attached to
			FName interactionBone = HitData.GetActor()->GetAttachParentSocketName();
			AActor* Arty = HitData.GetActor()->GetAttachParentActor();

			//Get the skeletal mesh of the 88
			if (Arty != NULL) {
				USkeletalMeshComponent* mesh = CastChecked<USkeletalMeshComponent>(Arty->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
				U88Anim * Animation = Cast<U88Anim>(mesh->GetAnimInstance());
				if (Animation != NULL) {
					if (interactionBone.ToString() == ELEVATION) {
						Animation->SetElevation(Value);
					}
					else if (interactionBone.ToString() == TRAVERSE) {
						Animation->SetTraverse(Value);
					}
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("Animation class not found - check Editor settings"));
				}
			} else {
				UE_LOG(LogTemp, Error, TEXT("88 Not Found - check C++ Object Iterator"));
			}
		}
	}
}

void ATankCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATankCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ATankCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATankCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ATankCharacter::EndTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATankCharacter::TouchUpdate);
	}
	return bResult;
}

//Helper method for trace
FHitResult ATankCharacter::Trace(ECollisionChannel Channel) {
	//Hit contains information about what the raycast hit.
	FHitResult HitData;

	//The origin of the raycast
	FVector StartLocation = FirstPersonCameraComponent->GetComponentLocation();

	//The EndLocation of the raycast
	FVector EndLocation = StartLocation + (FirstPersonCameraComponent->GetForwardVector() * PlayerInteractionDistance);

	//Collision parameters. The following syntax means that we don't want the trace to be complex
	//This will probably change later
	FCollisionQueryParams CollisionParameters;

	//Perform the line trace
	//The ECollisionChannel parameter is used in order to determine what we are looking for when performing the raycast
	// We use a unique interaction channel set in the Editor under Project Properties -> Collisions
	check(ECollisionChannel::ECC_InteractChannel);
	GetWorld()->LineTraceSingleByChannel(HitData, StartLocation, EndLocation, Channel, CollisionParameters);

	return HitData;
}