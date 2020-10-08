// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SpawnerAIController.generated.h"


UCLASS()
class TERRITORYFIGHT_API ASpawnerAIController : public AAIController
{
	GENERATED_BODY()
	

public:
    UPROPERTY(EditAnywhere, Category = "My")
        TSubclassOf<APawn> PawnToSpawn;

    UPROPERTY(EditAnywhere, Category = "My")
        UBehaviorTree* BTreeToSpawn;

    UPROPERTY(EditAnywhere, Category = "My")
    int MaxSpawnNum;

    UFUNCTION(BlueprintCallable, Category = "Spawner")
        bool SpawnAI();

    UFUNCTION(BlueprintCallable, Category = "Spawner")
        FTransform GetSpawnTransform() const;

private:

    int CurrentSpawnedNum;
};
