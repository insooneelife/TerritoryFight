// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRITORYFIGHT_API TestUtils
{

public:

    static void DrawTransform(
        const UWorld* InWorld, FTransform T, FColor SphereColor = FColor::White, float Length = 50);

    static FVector QuatToRotationVector(FQuat quat);
    static FQuat RotationVectorToQuat(FVector rv);

};

