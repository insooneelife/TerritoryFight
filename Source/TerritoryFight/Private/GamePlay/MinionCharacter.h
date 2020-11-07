// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hittable.h"
#include "Interfaces/MeleeAttackable.h"
#include "Interfaces/RangeAttackable.h"
#include "Interfaces/Targetable.h"
#include "Interfaces/Spawnable.h"
#include "Interfaces/GameData.h"
#include "GameCore/DataTypes.h"
#include "Blueprint/UserWidget.h"
#include "MinionCharacter.generated.h"

class UObjectWidget;
class UTextRenderComponent;

UCLASS(BlueprintType)
class TERRITORYFIGHT_API AMinionCharacter : 
	public ACharacter, public IHittable, public IMeleeAttackable, public IRangeAttackable, public ITargetable, public ISpawnable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinionCharacter();

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

	UPROPERTY(ReplicatedUsing = OnRep_Hp, BlueprintReadWrite, Category = "My")
	float Hp;

	UFUNCTION()
	void OnRep_Hp();

	UFUNCTION(NetMulticast, Reliable)
	void PlayMontageMulticast(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void SetRagdollMulticast();

private:
	UPROPERTY(Replicated)
	bool bIsDead;

	// IHittable
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnHit", ScriptName = "OnHit"), Category = "Hittable")
	void K2_OnHit(float InDamage, int InHitIdx);

	virtual void K2_OnHit_Implementation(float InDamage, int InHitIdx) override { OnHit(InDamage, InHitIdx); }

	virtual void OnHit(float InDamage, int InHitIdx) override;

	virtual void SetDead() override;

    virtual bool IsDead() const override { return bIsDead; }

	// ITargetable
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetTarget", ScriptName = "SetTarget"), Category = "Targetable")
	void K2_SetTarget(AActor* InTarget);

	virtual void K2_SetTarget_Implementation(AActor* InTarget) override { SetTarget(InTarget); };

	virtual void SetTarget(AActor* InTarget) override;
	
	virtual void ClearTarget() override;

	virtual AActor* GetTarget() const override { return Target; }

	// IMeleeAttackable
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryMeleeAttack", ScriptName = "TryMeleeAttack"), Category = "MeleeAttackable")
	void K2_TryMeleeAttack(AActor* InOwner);

	virtual void K2_TryMeleeAttack_Implementation(AActor* InOwner) override { TryMeleeAttack(InOwner); }

	virtual float GetMeleeAttackRange() const override { return AttackRange; }
	virtual float GetMeleeAttackAreaRadius() const override { return 100.0f; }
	virtual float GetMeleeAttackDamage() const override { return 10.0f; }

	// IRangeAttackable
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryRangeAttack", ScriptName = "TryRangeAttack"), Category = "RangeAttackable")
	void K2_TryRangeAttack(AActor* InOwner);

	virtual void K2_TryRangeAttack_Implementation(AActor* InOwner) override { TryRangeAttack(InOwner); }

	virtual TSubclassOf<AActor> GetProjectile() const override { return ProjectileToSpawn; }
	virtual FVector GetLauncherLocation() const override { return this->GetMesh()->GetSocketLocation(LauncherSocketName); }
	virtual FVector GetToTargetDirection() const override { return (this->Target->GetActorLocation() - GetLauncherLocation()).GetSafeNormal(); }
    virtual bool IsTargetValid() const override { return Target != nullptr; }


	// ISpawnable
public:
	virtual APawn* GetOwnerSpawner() const override { return this->OwnerSpawner; }


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "SetOwnerSpawner", ScriptName = "SetOwnerSpawner"), Category = "Spawnable")
	void K2_SetOwnerSpawner(APawn* InOwnerSpawner);

	virtual void K2_SetOwnerSpawner_Implementation(APawn* InOwnerSpawner) override { SetOwnerSpawner(InOwnerSpawner); }

	virtual void SetOwnerSpawner(APawn* InOwnerSpawner) override { this->OwnerSpawner = InOwnerSpawner; }

    // IGameData
public:
    virtual int GetTeamId() const override { return TeamId; }
    virtual void SetTeamId(int InTeamId) override { TeamId = InTeamId; }

	// getter && setter
public:
	void SetHp(float InHp);

private:
	void SetRoaming();
	void SetAgro();

	void ChaseTarget();

	void RunAttack();

	void SetAIMovementType(EAIMovementType MoveType); 
	float PickAttack(float DistanceToPlayer);

	bool GetRandomPointInRadius(const FVector& Origin, float Radius, FVector& OutResult);

private:
    UPROPERTY(EditAnywhere)
    TArray<UAnimMontage*> AttackMontages;

    UPROPERTY(EditAnywhere)
    TArray<UAnimMontage*> HitMontages;

    UPROPERTY(EditAnywhere)
    TArray<UAnimMontage*> DeathMontages;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileToSpawn;

    UPROPERTY(EditAnywhere)
    FName LauncherSocketName;

    UPROPERTY(EditAnywhere)
    bool IsRangeAttack;

    UPROPERTY(EditAnywhere)
    float WalkSpeed;

    UPROPERTY(EditAnywhere)
    float RunSpeedMultiplier;

    UPROPERTY(EditAnywhere)
    float AttackRange;

	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	APawn* OwnerSpawner;

	UPROPERTY()
	UObjectWidget* ObjectWidget;

	bool IsCombat;
	FVector StartLocation;
	FVector StartCombatLocation;

    UPROPERTY(EditAnywhere)
    int TeamId;

};
