// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUtils.h"
#include "Interfaces/ProjectileInterface.h"
#include "Interfaces/GameData.h"

AActor* GameUtils::ShootProjectile(AActor* Shooter, TSubclassOf<AActor> Projectile, FVector LauncherLoc, FVector ToTarget, int TeamId)
{
    FVector Location = LauncherLoc;
    FRotator Rotation = ToTarget.ToOrientationQuat().Rotator();
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = Shooter->GetWorld()->SpawnActor<AActor>(Projectile, Location, Rotation, SpawnInfo);

    if (SpawnedActor->IsValidLowLevel())
    {
        IProjectileInterface* Projectile = Cast<IProjectileInterface>(SpawnedActor);
        IGameData* GameData = Cast<IGameData>(SpawnedActor);

        if (Projectile != nullptr && GameData != nullptr)
        {
            Projectile->SetShooterOwner(Shooter);
            GameData->SetTeamId(TeamId);
        }
    }

    return SpawnedActor;
}