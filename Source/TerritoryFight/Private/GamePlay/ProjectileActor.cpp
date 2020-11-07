// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Hittable.h"

// Sets default values
AProjectileActor::AProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    LifeTime = 3.0f;
}

// Called when the game starts or when spawned
void AProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
    UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent());

    if (RootComp != nullptr)
    {
        //RootComp->OnComponentHit.AddDynamic(this, &AProjectileActor::OnHit);
        RootComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectileActor::OnBeginOverlap);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Root component is null!"));
    }
    SetLifeSpan(LifeTime);

}

// Called every frame
void AProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileActor::OnHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("OnHit!"));
}


void AProjectileActor::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor != ShooterOwner && ShooterOwner != nullptr)
    {
        IHittable* Hittable = Cast<IHittable>(OtherActor);
        IGameData* GameData = Cast<IGameData>(OtherActor);
        if (Hittable != nullptr && GameData != nullptr)
        {
            if (GameData->GetTeamId() != TeamId)
            {
                Hittable->OnHit(10, 0);
                SpawnParticleMulticast();
                SetLifeSpan(0.025);
            }
        }
    }
}


void AProjectileActor::SpawnParticleMulticast_Implementation()
{
    FVector SpawnLocation = GetActorLocation();

    UParticleSystemComponent* ParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(
        GetWorld(), CollisionParticle, FTransform(FQuat::Identity, SpawnLocation));
}