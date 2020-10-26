// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefaultAIController.generated.h"

class UPawnSensingComponent;
/**
 * 
 */
UCLASS()
class TERRITORYFIGHT_API ADefaultAIController : public AAIController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    //virtual void OnPossess(APawn* InPawn) override;
    //virtual void OnUnPossess() override;

    UFUNCTION()
    void OnSeePawn(APawn* InPawn);

private:
    void ResetTarget();


    UPROPERTY()
    UPawnSensingComponent* PawnSensing;

    UPROPERTY()
    AActor* Target;
};
