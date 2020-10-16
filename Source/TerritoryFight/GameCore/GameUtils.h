// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TERRITORYFIGHT_API GameUtils
{
public:

    static AActor* ShootProjectile(AActor* Shooter, TSubclassOf<AActor> Projectile, FVector LauncherLoc, FVector ToTarget);
};
