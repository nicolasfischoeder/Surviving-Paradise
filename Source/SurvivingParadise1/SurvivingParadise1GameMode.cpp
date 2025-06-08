// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivingParadise1GameMode.h"
#include "SurvivingParadise1Character.h"
#include "UObject/ConstructorHelpers.h"

ASurvivingParadise1GameMode::ASurvivingParadise1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
