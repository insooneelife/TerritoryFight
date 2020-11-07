// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/PawnSensingComponent.h"
#include "TeamedPawnSensingComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, HideCategories = (Activation, "Components|Activation", Collision), meta = (BlueprintSpawnableComponent))
class TERRITORYFIGHT_API UTeamedPawnSensingComponent : public UPawnSensingComponent
{
	GENERATED_BODY()
	
public:

    virtual bool CouldSeePawn(const APawn* Other, bool bMaySkipChecks = false) const override;
};
