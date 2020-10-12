// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hittable.h"
#include "Targetable.h"
#include "TowerPawn.generated.h"

UCLASS(BlueprintType)
class TERRITORYFIGHT_API ATowerPawn : public APawn, public IHittable, public ITargetable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATowerPawn();

    // assets
public:
    UPROPERTY(EditAnywhere, Category = "My")
        TSubclassOf<AActor> Projectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // network
public:

    UPROPERTY(Replicated, BlueprintReadWrite, Category = "My")
        float Hp;

    // IHittable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnHit", ScriptName = "OnHit"), Category = "Hittable")
        void K2_OnHit(float InDamage, int InHitIdx);

    virtual void K2_OnHit_Implementation(float InDamage, int InHitIdx) override { OnHit(InDamage, InHitIdx); }

    virtual void OnHit(float InDamage, int InHitIdx) override;

    virtual void SetDead() override;

    // ITargetable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetTarget", ScriptName = "SetTarget"), Category = "Targetable")
        void K2_SetTarget(AActor* InTarget);

    virtual void K2_SetTarget_Implementation(AActor* InTarget) override { SetTarget(InTarget); };

    virtual void SetTarget(AActor* InTarget) override;

    virtual void ClearTarget() override { Target = nullptr; }

private:
    void RunAttack();

    void RangeAttack();

private:
    UPROPERTY()
        UStaticMeshComponent* MainMesh;
    
    UPROPERTY()
        USceneComponent* Launcher;

    UPROPERTY()
        AActor* Target;

    bool IsDead;
};
