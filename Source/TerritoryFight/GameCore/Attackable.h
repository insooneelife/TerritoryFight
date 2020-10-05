// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Attackable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
        void OnAttackStart();

    virtual void OnAttackStart_Implementation() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
        void OnAttackEnd();

    virtual void OnAttackEnd_Implementation() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
        void OnResetCombo();

    virtual void OnResetCombo_Implementation() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
        void OnSaveAttack();

    virtual void OnSaveAttack_Implementation() = 0;
};
