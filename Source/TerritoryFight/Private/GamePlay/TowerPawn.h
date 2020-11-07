// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Hittable.h"
#include "Interfaces/Targetable.h"
#include "Interfaces/GameData.h"
#include "Interfaces/RangeAttackable.h"
#include "TowerPawn.generated.h"

UCLASS(BlueprintType)
class TERRITORYFIGHT_API ATowerPawn : public APawn, public IHittable, public ITargetable, public IRangeAttackable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATowerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // IHittable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnHit", ScriptName = "OnHit"), Category = "Hittable")
        void K2_OnHit(float InDamage, int InHitIdx);

    virtual void K2_OnHit_Implementation(float InDamage, int InHitIdx) override { OnHit(InDamage, InHitIdx); }

    virtual void OnHit(float InDamage, int InHitIdx) override;

    virtual void SetDead() override;

    virtual bool IsDead() const override { return bIsDead; }
    virtual void SetTeamId(int InTeamId) override { TeamId = InTeamId; }

    // ITargetable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetTarget", ScriptName = "SetTarget"), Category = "Targetable")
        void K2_SetTarget(AActor* InTarget);

    virtual void K2_SetTarget_Implementation(AActor* InTarget) override { SetTarget(InTarget); };

    virtual void SetTarget(AActor* InTarget) override;

    virtual void ClearTarget() override { Target = nullptr; }

    virtual AActor* GetTarget() const override { return Target; }

    // IGameData
public:
    virtual int GetTeamId() const override { return TeamId; }

    // IRangeAttackable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryRangeAttack", ScriptName = "TryRangeAttack"), Category = "RangeAttackable")
        void K2_TryRangeAttack(AActor* InOwner);

    virtual void K2_TryRangeAttack_Implementation(AActor* InOwner) override { TryRangeAttack(InOwner); }

    virtual TSubclassOf<AActor> GetProjectile() const override { return ProjectileToSpawn; }
    virtual FVector GetLauncherLocation() const override { return this->Launcher->GetComponentToWorld().GetLocation(); }
    virtual FVector GetToTargetDirection() const override { return (this->Target->GetActorLocation() - GetLauncherLocation()).GetSafeNormal(); }
    virtual bool IsTargetValid() const override { return Target != nullptr; }

private:
    void RunAttack();

private:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileToSpawn;

    UPROPERTY()
    UStaticMeshComponent* MainMesh;
    
    UPROPERTY()
    USceneComponent* Launcher;

    UPROPERTY()
    AActor* Target;

    UPROPERTY(EditAnywhere)
    int TeamId;

    UPROPERTY(Replicated)
    float Hp;

    UPROPERTY(Replicated)
    bool bIsDead;
};
