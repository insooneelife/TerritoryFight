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
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryAttack", ScriptName = "TryAttack"))
        void K2_TryAttack();

    virtual void K2_TryAttack_Implementation() = 0;

    virtual void TryAttack() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnAttackStart", ScriptName = "OnAttackStart"))
        void K2_OnAttackStart();

    virtual void K2_OnAttackStart_Implementation() = 0;

    virtual void OnAttackStart() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnAttackEnd", ScriptName = "OnAttackEnd"))
        void K2_OnAttackEnd();

    virtual void K2_OnAttackEnd_Implementation() = 0;

    virtual void OnAttackEnd() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnResetCombo", ScriptName = "OnResetCombo"))
        void K2_OnResetCombo();

    virtual void K2_OnResetCombo_Implementation() = 0;

    virtual void OnResetCombo() = 0;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnSaveAttack", ScriptName = "OnSaveAttack"))
        void K2_OnSaveAttack();

    virtual void K2_OnSaveAttack_Implementation() = 0;

    virtual void OnSaveAttack() = 0;

    virtual bool SphereSweep(FHitResult& OutHit, AActor* OwnerActor, FVector Start, FVector End, float Radius);

};
