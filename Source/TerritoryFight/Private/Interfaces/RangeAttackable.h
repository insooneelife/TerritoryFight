// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData.h"
#include "RangeAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class URangeAttackable : public UGameData
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IRangeAttackable: public IGameData
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryRangeAttack", ScriptName = "TryRangeAttack"))
        void K2_TryRangeAttack(AActor* InOwner);

    virtual void K2_TryRangeAttack_Implementation(AActor* InOwner) = 0;

    virtual void TryRangeAttack(AActor* InOwner);

    virtual TSubclassOf<AActor> GetProjectile() const = 0;
    virtual FVector GetLauncherLocation() const = 0;
    virtual FVector GetToTargetDirection() const = 0;
    virtual bool IsTargetValid() const = 0;
};
