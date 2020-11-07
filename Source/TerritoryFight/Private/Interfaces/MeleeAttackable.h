// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeleeAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UMeleeAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IMeleeAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryMeleeAttack", ScriptName = "TryMeleeAttack"))
        void K2_TryMeleeAttack(AActor* InOwner);

    virtual void K2_TryMeleeAttack_Implementation(AActor* InOwner) = 0;

    virtual void TryMeleeAttack(AActor* InOwner);

    virtual float GetMeleeAttackRange() const = 0;
    virtual float GetMeleeAttackAreaRadius() const = 0;
    virtual float GetMeleeAttackDamage() const = 0;
};
