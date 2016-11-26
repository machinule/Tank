// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Tank.h"
#include "TankGameMode.h"
#include "TankHUD.h"
#include "TankCharacter.h"

ATankGameMode::ATankGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATankHUD::StaticClass();
}
