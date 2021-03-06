// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerPawn.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "Interfaces/ProjectileInterface.h"
#include "GameCore/GameUtils.h"

// Sets default values
ATowerPawn::ATowerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATowerPawn::BeginPlay()
{
	Super::BeginPlay();
	
    this->Hp = 100;

    // get main mesh
    TArray<UActorComponent*> Comps = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName(TEXT("MainCollider")));

    if (Comps.Num() > 0)
    {
        this->MainMesh = Cast<UStaticMeshComponent>(Comps[0]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No tag MainCollider found!"));
    }

    // get launcher
    TArray<UActorComponent*> LauncherComps = GetComponentsByTag(USceneComponent::StaticClass(), FName(TEXT("Launcher")));

    if (LauncherComps.Num() > 0)
    {
        this->Launcher = Cast<USceneComponent>(LauncherComps[0]);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No tag MainCollider found!"));
    }

}

void ATowerPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ATowerPawn, Hp);
    DOREPLIFETIME(ATowerPawn, bIsDead);
}

// Called every frame
void ATowerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATowerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATowerPawn::OnHit(float InDamage, int InHitIdx)
{
    if (HasAuthority())
    {
        if (bIsDead)
        {
            return;
        }

        this->Hp = this->Hp - InDamage;

        UE_LOG(LogTemp, Warning, TEXT("ATowerPawn OnHit!!!"));

        if (this->Hp <= 0)
        {
            this->Hp = 0;

            SetDead();
        }
        else
        {
            //PlayMontageMulticast(HitMontages[InHitIdx]);
        }
    }
}

void ATowerPawn::SetDead()
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("ATowerPawn SetDead!!!"));

        this->bIsDead = true;
        //GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        //PlayMontageMulticast(DeathMontages[InHitIdx]);



        SetLifeSpan(1.0f);
    }
}


void ATowerPawn::SetTarget(AActor* InTarget)
{
    this->Target = InTarget;

    if (this->bIsDead)
    {
        return;
    }

    RunAttack();
}



void ATowerPawn::RunAttack()
{
    if (this->Target->IsValidLowLevel())
    {
        TryRangeAttack(this);

        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
            UnusedHandle, this, &ATowerPawn::RunAttack, 1.0f, false);
    }
    else
    {
        ClearTarget();
    }
}