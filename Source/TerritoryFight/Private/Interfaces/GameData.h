// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameData : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IGameData
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual int GetTeamId() const = 0;
    virtual void SetTeamId(int InTeamId) = 0;
};
