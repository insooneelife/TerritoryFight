// Fill out your copyright notice in the Description page of Project Settings.


#include "Attackable.h"

// Add default functionality here for any IAttackable functions that are not pure virtual.


bool IAttackable::SphereSweep(FHitResult& OutHit, AActor* OwnerActor, FVector Start, FVector End, float Radius)
{
    FCollisionQueryParams CollParam;
    FCollisionObjectQueryParams CollObjParam(ECollisionChannel::ECC_Pawn);
    CollParam.AddIgnoredActor(OwnerActor);

    if (OwnerActor->GetWorld()->SweepSingleByObjectType(
        OutHit,
        Start,
        End,
        FQuat::Identity,
        CollObjParam,
        FCollisionShape::MakeSphere(Radius),
        CollParam))
    {
        return true;
    }
    return false;
}