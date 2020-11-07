// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerPawn.h"
#include "Interfaces/Spawnable.h"
#include "Interfaces/GameData.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ASpawnerPawn::ASpawnerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnerPawn::BeginPlay()
{
	Super::BeginPlay();
	

    TrySpawnAI();
}


// Called every frame
void ASpawnerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpawnerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpawnerPawn::OnSpawnableDead(AActor* DeadActor)
{
    this->SpawnedActors.Remove(DeadActor);
}

void ASpawnerPawn::TrySpawnAI()
{
    if (HasAuthority())
    {
        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
            UnusedHandle, this, &ASpawnerPawn::SpawnAI, 5.0f, false);
    }
}

void ASpawnerPawn::SpawnAI()
{
    if (MaxSpawnNum > SpawnedActors.Num())
    {
        APawn* SpawnedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
            GetWorld(),
            PawnToSpawn,
            nullptr,
            GetActorLocation());


        ISpawnable* Spawnable = Cast<ISpawnable>(SpawnedPawn);
        IGameData* GameData = Cast<IGameData>(SpawnedPawn);
        if (Spawnable != nullptr && GameData != nullptr)
        {
            Spawnable->SetOwnerSpawner(this);

            GameData->SetTeamId(TeamId);

            this->SpawnedActors.Add(SpawnedPawn);
            TrySpawnAI();
        }

    }
    else
    {
        TrySpawnAI();
    }

}