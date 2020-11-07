// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum EAIMovementType
{
    EAM_Idle,
    EAM_Walking,
    EAM_Running
};

/**
 * 
 */
class TERRITORYFIGHT_API DataTypes
{
public:
	DataTypes();
	~DataTypes();
};
