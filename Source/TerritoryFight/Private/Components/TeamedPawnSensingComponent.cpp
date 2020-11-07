// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamedPawnSensingComponent.h"
#include "Interfaces/GameData.h"
#include "Interfaces/Hittable.h"

bool UTeamedPawnSensingComponent::CouldSeePawn(const APawn* Other, bool bMaySkipChecks) const
{
    if (Super::CouldSeePawn(Other, bMaySkipChecks))
    {
        const IGameData* PawnGameData = Cast<IGameData>(GetSensorActor());
        const IHittable* OtherPawnHittable = Cast<IHittable>(Other);

        if (PawnGameData != nullptr && OtherPawnHittable != nullptr)
        {
            int TeamId = PawnGameData->GetTeamId();
            int OtherTeamId = OtherPawnHittable->GetTeamId();

            if (TeamId != OtherTeamId && !OtherPawnHittable->IsDead())
            {
                return true;
            }
        }
    }
    
    return false;
}