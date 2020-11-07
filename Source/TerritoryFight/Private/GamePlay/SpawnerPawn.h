// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Spawner.h"
#include "Interfaces/GameData.h"
#include "SpawnerPawn.generated.h"

UCLASS(BlueprintType)
class TERRITORYFIGHT_API ASpawnerPawn : public APawn, public ISpawner, public IGameData
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpawnerPawn();

public:
    UPROPERTY(EditAnywhere, Category = "My")
        TSubclassOf<APawn> PawnToSpawn;

    UPROPERTY(EditAnywhere, Category = "My")
        int MaxSpawnNum;

    UPROPERTY(EditAnywhere, Category = "My")
        FTransform SpawnTransform;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // ISpawner
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnSpawnableDead", ScriptName = "OnSpawnableDead"), Category = "Spawner")
        void K2_OnSpawnableDead(AActor* DeadActor);

    virtual void K2_OnSpawnableDead_Implementation(AActor* DeadActor) override { OnSpawnableDead(DeadActor); }

    virtual void OnSpawnableDead(AActor* DeadActor) override;

    // IGameData
public:
    virtual int GetTeamId() const override { return TeamId; }
    virtual void SetTeamId(int InTeamId) override { TeamId = InTeamId; }

private:
    void TrySpawnAI();
    void SpawnAI();

private:
    TArray<AActor*> SpawnedActors;

    UPROPERTY(EditAnywhere)
    int TeamId;
    
};
