// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firstperson415GameMode.h"
#include "Firstperson415Character.h"
#include "UObject/ConstructorHelpers.h"

AFirstperson415GameMode::AFirstperson415GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
