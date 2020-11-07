// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData.h"
#include "Targetable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UTargetable : public UGameData
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API ITargetable : public IGameData
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetTarget", ScriptName = "SetTarget"))
    void K2_SetTarget(AActor* InTarget);

    virtual void K2_SetTarget_Implementation(AActor* InTarget) = 0;

    virtual void SetTarget(AActor* InTarget) = 0;

    virtual AActor* GetTarget() const = 0;

    virtual void ClearTarget() = 0;
};
