// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawner.h"
#include "Spawnable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class USpawnable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API ISpawnable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    virtual APawn* GetOwnerSpawner() const = 0;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetOwnerSpawner", ScriptName = "SetOwnerSpawner"))
        void K2_SetOwnerSpawner(APawn* InOwnerSpawner);

    virtual void K2_SetOwnerSpawner_Implementation(APawn* InOwnerSpawner) = 0;

    virtual void SetOwnerSpawner(APawn* OwnerSpawner) = 0;

};
