// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
#include "MeleeAttackable.h"
#include "Targetable.h"
#include "Spawnable.h"
#include "../DataTypes.h"
#include "MinionCharacter.generated.h"

UCLASS(BlueprintType)
class TERRITORYFIGHT_API AMinionCharacter : 
    public ACharacter, public IHittable, public IMeleeAttackable, public ITargetable, public ISpawnable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionCharacter();

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> AttackMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> HitMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> DeathMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        float WalkSpeed;

    UPROPERTY(EditAnywhere, Category = "My")
        float RunSpeedMultiplier;
    
    UPROPERTY(EditAnywhere, Category = "My")
        float MeleeAttackDistance;

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

    UFUNCTION(NetMulticast, Reliable)
        void PlayMontageMulticast(UAnimMontage* Montage);

    UFUNCTION(NetMulticast, Reliable)
        void SetRagdollMulticast();

private:
    UPROPERTY(Replicated)
        bool IsDead;

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
    
    virtual void ClearTarget() override;

    // IMeleeAttackable
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryMeleeAttack", ScriptName = "TryMeleeAttack"), Category = "MeleeAttackable")
        void K2_TryMeleeAttack(AActor* InOwner);

    virtual void K2_TryMeleeAttack_Implementation(AActor* InOwner) override { TryMeleeAttack(InOwner); }

    virtual float GetMeleeAttackRange() const override { return 200.0f; }
    virtual float GetMeleeAttackAreaRadius() const override { return 100.0f; }
    virtual float GetMeleeAttackDamage() const override { return 10.0f; }

    // ISpawnable
public:
    virtual APawn* GetOwnerSpawner() const override { return this->OwnerSpawner; }


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetOwnerSpawner", ScriptName = "SetOwnerSpawner"), Category = "Spawnable")
        void K2_SetOwnerSpawner(APawn* InOwnerSpawner);

    virtual void K2_SetOwnerSpawner_Implementation(APawn* InOwnerSpawner) override { SetOwnerSpawner(InOwnerSpawner); }

    virtual void SetOwnerSpawner(APawn* InOwnerSpawner) override { this->OwnerSpawner = InOwnerSpawner; }

private:
    void SetRoaming();
    void SetAgro();

    void ChaseTarget();

    void RunAttack();

    void SetAIMovementType(EAIMovementType MoveType); 
    float PickAttack(float DistanceToPlayer);

    bool GetRandomPointInRadius(const FVector& Origin, float Radius, FVector& OutResult);


    

private:
    UPROPERTY()
        AActor* Target;

    UPROPERTY()
        APawn* OwnerSpawner;

    bool IsCombat;
    FVector StartLocation;
    FVector StartCombatLocation;
};
