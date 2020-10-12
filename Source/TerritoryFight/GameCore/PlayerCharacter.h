// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
#include "Attackable.h"
#include "PlayerCharacter.generated.h"

class UStaticMeshComponent;
class ACollisionActor;
class UParticleSystem;

UCLASS(BlueprintType)
class TERRITORYFIGHT_API APlayerCharacter : public ACharacter, public IHittable, public IAttackable
{
    GENERATED_BODY()

public:
    // blueprint assetlink

    // Sets default values for this character's properties
    APlayerCharacter();

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> AttackMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> HitMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UParticleSystem*> MeleeAttackChargeParticles;

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My")
        float BaseTurnRate;

    /** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My")
        float BaseLookUpRate;

    // getter && setter
public:
    float GetHp() const { return this->Hp; }
    void SetHp(float InHp) { this->Hp = InHp; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // events
protected:
    void OpenMenu();

    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
    void MoveForward(float Value);
    void MoveRight(float Value);

    void OnAttackPress();

    UFUNCTION()
        void OnBeginOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult& SweepResult);

    UFUNCTION()
        void OnEndOverlap(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor,
            UPrimitiveComponent* OtherComp,
            int32 OtherBodyIndex);

    // network
public:

    UPROPERTY(ReplicatedUsing = OnRep_Hp, BlueprintReadWrite, Category = "My")
        float Hp;

    UPROPERTY(Replicated)
        bool IsAttacking;

    UPROPERTY(Replicated)
        bool SaveAttack;

    UPROPERTY(Replicated)
        int AttackIdx;

    UFUNCTION()
        void OnRep_Hp();

    UFUNCTION(Server, Reliable)
        void AttackRPC();

    UFUNCTION(NetMulticast, Reliable)
        void PlayMontageMulticast(UAnimMontage* Montage);

    UFUNCTION(NetMulticast, Reliable)
        void SpawnParticleMulticast(FVector ImpactPos);

    // hit interface
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnHit", ScriptName = "OnHit"), Category = "Hittable")
        void K2_OnHit(float InDamage, int InHitIdx);

    virtual void K2_OnHit_Implementation(float InDamage, int InHitIdx) override { OnHit(InDamage, InHitIdx); }

    virtual void OnHit(float InDamage, int InHitIdx) override;

    virtual void SetDead() override {}

    // attackable interface
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "TryAttack", ScriptName = "TryAttack"), Category = "Attack")
        void K2_TryAttack();

    virtual void K2_TryAttack_Implementation() override { TryAttack(); }

    virtual void TryAttack() override;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnAttackStart", ScriptName = "OnAttackStart"), Category = "Attack")
        void K2_OnAttackStart();

    virtual void K2_OnAttackStart_Implementation() override { OnAttackStart(); }

    virtual void OnAttackStart() override;


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnAttackEnd", ScriptName = "OnAttackEnd"), Category = "Attack")
        void K2_OnAttackEnd();

    virtual void K2_OnAttackEnd_Implementation() override { OnAttackEnd(); }

    virtual void OnAttackEnd() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnResetCombo", ScriptName = "OnResetCombo"), Category = "Attack")
        void K2_OnResetCombo();

    virtual void K2_OnResetCombo_Implementation() override { OnResetCombo(); }

    virtual void OnResetCombo() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "OnSaveAttack", ScriptName = "OnSaveAttack"), Category = "Attack")
        void K2_OnSaveAttack();

    virtual void K2_OnSaveAttack_Implementation() override { OnSaveAttack(); }

    virtual void OnSaveAttack() override;

    

    // particle system
private:
    void SpawnParticle(UParticleSystem* Particle, FVector SpawnLocation);


private:
    void SphereSweep(FVector Start, FVector End, float Radius);


private:

    FBodyInstance* RHand;

    FBodyInstance* LHand;

    FVector SaveAttackStartPos;
    
};



