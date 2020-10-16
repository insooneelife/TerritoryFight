// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeAttackable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Hittable.h"
#include "GameUtils.h"

// Add default functionality here for any IRangeAttackable functions that are not pure virtual.


void IRangeAttackable::TryRangeAttack(AActor* InOwner)
{
    if (InOwner->HasAuthority())
    {
        IRangeAttackable* RangeActor = Cast<IRangeAttackable>(InOwner);

        if (RangeActor != nullptr)
        {
            GameUtils::ShootProjectile(
                InOwner, 
                RangeActor->GetProjectile(), 
                RangeActor->GetLauncherLocation(), 
                RangeActor->GetToTargetDirection());;
        }
    }
}