// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spawner.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class USpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API ISpawner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnSpawnableDead", ScriptName = "OnSpawnableDead"))
    void K2_OnSpawnableDead(AActor* DeadActor);

    virtual void K2_OnSpawnableDead_Implementation(AActor* DeadActor) = 0;

    virtual void OnSpawnableDead(AActor* DeadActor) = 0;
};
