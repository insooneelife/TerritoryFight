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
        TSubclassOf<AActor> AttackClass;

    UPROPERTY(EditAnywhere, Category = "My")
        bool DebugShape;

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

    // interface
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hittable")
        void OnHitBlueprint(float InDamage, int InHitIdx);

    virtual void OnHitBlueprint_Implementation(float InDamage, int InHitIdx) override { OnHit(InDamage, InHitIdx); }

    void OnHit(float InDamage, int InHitIdx) override;

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
        void PlayAttackMontageMulticast(int InAttackIdx);


    UFUNCTION(NetMulticast, Reliable)
        void PlayHitMontageMulticast(int InHitIdx);


    UFUNCTION(NetMulticast, Reliable)
        void SpawnParticleMulticast(FVector ImpactPos);

    // animation notify
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My")
        void OnAttackStart();

    virtual void OnAttackStart_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My")
        void OnAttackEnd();

    virtual void OnAttackEnd_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My")
        void OnResetCombo();

    virtual void OnResetCombo_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My")
        void OnSaveAttack();

    virtual void OnSaveAttack_Implementation() override;

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



