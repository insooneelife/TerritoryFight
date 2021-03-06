// Copyright Epic Games, Inc. All Rights Reserved.

#include "TerritoryFightGameMode.h"
#include "TerritoryFightCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATerritoryFightGameMode::ATerritoryFightGameMode()
{
	// set default pawn class to our Blueprinted character

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ParagonSteel/Characters/Heroes/Steel/MySteelCharacterBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
