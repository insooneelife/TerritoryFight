// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UProjectileInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TERRITORYFIGHT_API IProjectileInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Projectile")
        APawn* GetShooterOwner() const;

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Projectile")
        void SetShooterOwner(APawn* Owner);
};
