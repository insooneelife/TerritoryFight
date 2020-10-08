// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


bool ASpawnerAIController::SpawnAI()
{
    if (MaxSpawnNum > CurrentSpawnedNum)
    {
        APawn* SpawnedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
            GetWorld(),
            this->PawnToSpawn,
            this->BTreeToSpawn,
            GetSpawnTransform().GetLocation(),
            GetSpawnTransform().GetRotation().Rotator(), 
            true
        );

        if (SpawnedPawn != nullptr)
        {
            return true;
        }
    }

    return false;
}

FTransform ASpawnerAIController::GetSpawnTransform() const
{
    return GetPawn()->GetTransform();
}