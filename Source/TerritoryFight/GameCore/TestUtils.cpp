// Fill out your copyright notice in the Description page of Project Settings.


#include "TestUtils.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include <cmath>


void TestUtils::DrawTransform(const UWorld* InWorld, FTransform T, FColor SphereColor, float Length)
{
    FVector Loc = T.GetLocation();
    DrawDebugSphere(InWorld, Loc, 10, 26, SphereColor, true, -1, 0, 2);

    FVector Forward = T.GetUnitAxis(EAxis::X);
    FVector Right = T.GetUnitAxis(EAxis::Y);
    FVector Up = T.GetUnitAxis(EAxis::Z);

    DrawDebugDirectionalArrow(InWorld, Loc, Loc + Forward * Length, 25.f, FColor::Red, true, -1.f, 0, 5.f);
    DrawDebugDirectionalArrow(InWorld, Loc, Loc + Right * Length, 25.f, FColor::Green, true, -1.f, 0, 5.f);
    DrawDebugDirectionalArrow(InWorld, Loc, Loc + Up * Length, 25.f, FColor::Blue, true, -1.f, 0, 5.f);
}

FVector TestUtils::QuatToRotationVector(FQuat quat)
{
    FVector axis = quat.GetRotationAxis();
    float angle = quat.GetAngle();

    if (angle < 0 || angle > 6.29) {
        UE_LOG(LogTemp, Warning, TEXT("QuatToRotationVector: Rotation angle error!!"));
    }

    angle = std::fmod(angle + PI, 2 * PI) - PI;

    return axis * angle;
}

FQuat TestUtils::RotationVectorToQuat(FVector rv)
{
    float angle = rv.Size();
    FVector axis = rv.GetSafeNormal();
    return FQuat(axis, angle);
}