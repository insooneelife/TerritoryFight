// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Hittable.h"
#include "Attackable.h"
#include "PlayerCharacter.generated.h"

class UStaticMeshComponent;
class ACollisionActor;

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
        void OnHitBlueprint(float InDamage, int Side);

    virtual void OnHitBlueprint_Implementation(float InDamage, int Side) override { OnHit(InDamage, Side); }

    void OnHit(float InDamage, int Side) override;

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


    UFUNCTION(NetMulticast, Reliable)
        void PlayMontageMulticast();

    // animation notify
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AMyCharacter")
        void OnAttackStart();

    virtual void OnAttackStart_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AMyCharacter")
        void OnAttackEnd();

    virtual void OnAttackEnd_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AMyCharacter")
        void OnResetCombo();

    virtual void OnResetCombo_Implementation() override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AMyCharacter")
        void OnSaveAttack();

    virtual void OnSaveAttack_Implementation() override;


private:
    void PlayNextAttackAnimMontage();
    void SphereSweep(FVector Start, FVector End, float Radius);


private:

    FBodyInstance* RHand;

    FBodyInstance* LHand;

    FVector SaveAttackStartPos;

    bool IsOverlapped;
    bool IsAttacking;
    bool SaveAttack;
    int AttackIdx;
};



