// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileShooterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI)
class UProjectileShooterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IProjectileShooterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "ProjectileShooter")
        FVector GetLauncherLocation() const;
};
