// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameData.h"
#include "Interfaces/ProjectileInterface.h"
#include "ProjectileActor.generated.h"

class UParticleSystem;

UCLASS()
class TERRITORYFIGHT_API AProjectileActor : public AActor, public IProjectileInterface, public IGameData
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
    void OnHit(
        UPrimitiveComponent* HitComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

public:
    UFUNCTION(NetMulticast, Reliable)
    void SpawnParticleMulticast();

    // IGameData
public:
    virtual int GetTeamId() const override { return TeamId; }
    virtual void SetTeamId(int InTeamId) override { TeamId = InTeamId; }

    // IProjectileInterface
public:
    virtual AActor* GetShooterOwner() const override { return ShooterOwner; }
    virtual void SetShooterOwner(AActor* InOwner) override { ShooterOwner = InOwner; }

private:
    UPROPERTY()
    AActor* ShooterOwner;

    UPROPERTY(EditAnywhere)
    UParticleSystem* CollisionParticle;

    UPROPERTY(EditAnywhere)
    int TeamId;

    UPROPERTY(EditAnywhere)
    float LifeTime;
};
