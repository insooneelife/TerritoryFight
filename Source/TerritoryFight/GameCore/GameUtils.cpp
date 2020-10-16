// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUtils.h"
#include "ProjectileInterface.h"

AActor* GameUtils::ShootProjectile(AActor* Shooter, TSubclassOf<AActor> Projectile, FVector LauncherLoc, FVector ToTarget)
{
    FVector Location = LauncherLoc;
    FRotator Rotation = ToTarget.ToOrientationQuat().Rotator();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = Shooter->GetWorld()->SpawnActor<AActor>(Projectile, Location, Rotation, SpawnInfo);

    if (SpawnedActor->IsValidLowLevel())
    {
        if (SpawnedActor->GetClass()->ImplementsInterface(UProjectileInterface::StaticClass()))
        {
            IProjectileInterface::Execute_SetShooterOwner(SpawnedActor, Shooter);
        }
    }

    return SpawnedActor;
}