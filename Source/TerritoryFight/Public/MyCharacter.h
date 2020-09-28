// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UStaticMeshComponent;
class ACollisionActor;

UCLASS()
class TERRITORYFIGHT_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // blueprint assetlink

    // Sets default values for this character's properties
    AMyCharacter();

    UPROPERTY(EditAnywhere, Category = "My")
        TSubclassOf<AActor> AttackClass;

    UPROPERTY(EditAnywhere, Category = "My")
        bool DebugShape;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> AttackMontages;

    UPROPERTY(EditAnywhere, Category = "My")
        TArray<UAnimMontage*> HitMontages;

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


    void OnAttacked(float InDamage, int Side);

    // network
public:

    //UPROPERTY(BlueprintReadWrite, Category = "My")
    UPROPERTY(ReplicatedUsing = OnRep_Hp, BlueprintReadWrite, Category = "My")
        float Hp;

    UPROPERTY(Replicated)
        int LastAttackSide;

    UFUNCTION()
        void OnRep_Hp();

    UFUNCTION(Server, Reliable)
        void AttackRPC();


    UFUNCTION(NetMulticast, Reliable)
        void AttackMulticast();


    // animation notify
public:
    UFUNCTION(BlueprintCallable, Category = "AMyCharacter")
        void AttackStart();

    UFUNCTION(BlueprintCallable, Category = "AMyCharacter")
        void AttackEnd();

    UFUNCTION(BlueprintCallable, Category = "AMyCharacter")
        void ResetCombo();

    UFUNCTION(BlueprintCallable, Category = "AMyCharacter")
        void ComboAttackSave();


private:
    void PlayNextAttackAnimMontage();
    void SphereSweep(FVector Start, FVector End, float Radius);


private:
    FBodyInstance* RHand;
    FBodyInstance* LHand;

    UCapsuleComponent* RootCapsule;
    FVector SaveAttackStartPos;

    bool IsOverlapped;
    bool IsAttacking;
    bool SaveAttack;
    int AttackCount;
};



