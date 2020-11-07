// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable, MinimalAPI)
class UHittable : public UGameData
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IHittable : public IGameData
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnHit", ScriptName = "OnHit"))
        void K2_OnHit(float InDamage, int InHitIdx);

    virtual void K2_OnHit_Implementation(float InDamage, int InHitIdx) = 0;

    virtual void OnHit(float InDamage, int InHitIdx) = 0;

    virtual void SetDead() = 0;

    virtual bool IsDead() const = 0;
};
