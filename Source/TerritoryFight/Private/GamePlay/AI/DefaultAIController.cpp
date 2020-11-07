// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/TeamedPawnSensingComponent.h"
#include "Interfaces/Targetable.h"
#include "Interfaces/Hittable.h"
#include "Interfaces/GameData.h"

#include "Navigation/PathFollowingComponent.h"

void ADefaultAIController::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing = Cast<UTeamedPawnSensingComponent>(GetComponentByClass(UTeamedPawnSensingComponent::StaticClass()));
	if (PawnSensing == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PawnSensing is nullptr!!"));
	}
	else
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &ADefaultAIController::OnSeePawn);
	}
}

void ADefaultAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ResetTarget();
}


void ADefaultAIController::OnSeePawn(APawn* InPawn)
{
    if (Target->IsValidLowLevel())
    {
    }
    else
    {
        IGameData* TargetGameDataInst = Cast<IGameData>(InPawn);
        IGameData* GameDataInst = Cast<IGameData>(GetPawn());

        if (TargetGameDataInst != nullptr && GameDataInst != nullptr)
        {
            if (GameDataInst->GetTeamId() == TargetGameDataInst->GetTeamId())
            {
                return;
            }
        }

        Target = InPawn;
        ITargetable* Targetable = Cast<ITargetable>(GetPawn());
        if (Targetable != nullptr)
        {
            Targetable->SetTarget(Target);
        }
    }
}


void ADefaultAIController::ResetTarget()
{
    if (PawnSensing == nullptr)
        return;

	if (Target != nullptr)
	{
        ITargetable* Targetable = Cast<ITargetable>(GetPawn());

        IHittable* Hittable = Cast<IHittable>(Target);
        if (Targetable != nullptr && Hittable != nullptr)
        {
            float Distance = GetPawn()->GetDistanceTo(Target);

            if (Distance > PawnSensing->SightRadius || Hittable->IsDead())
            {
                Target = nullptr;
                Targetable->SetTarget(nullptr);
            }
        }
	}
}