// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "TestUtils.h"

// Sets default values
AMinionCharacter::AMinionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    this->PrimaryActorTick.bStartWithTickEnabled = false;
    this->bUseControllerRotationYaw = false;

    this->AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();

    this->StartLocation = GetActorLocation();

    SetAIMovementType(EAIMovementType::EAM_Walking);

    FTimerHandle UnusedHandle;
    GetWorldTimerManager().SetTimer(
        UnusedHandle, this, &AMinionCharacter::SetRoaming, 1.0f, false);

    this->Hp = 100;
}

void AMinionCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMinionCharacter, Hp);
    DOREPLIFETIME(AMinionCharacter, IsDead);
}

// Called every frame
void AMinionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (this->IsDead)
    {
        return;
    }

    if (this->IsCombat)
    {
        if (this->Target->IsValidLowLevel())
        {
            FVector Loc = GetActorLocation();
            FVector TargetLoc = this->Target->GetActorLocation();
            FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Loc, TargetLoc);
            
            FRotator TargetRot = FRotator(0.0f, LookAtRot.Yaw, 0.0f);
            FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 5.0f);

            SetActorRotation(NewRot);
        }
    }
}

// Called to bind functionality to input
void AMinionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinionCharacter::PlayMontageMulticast_Implementation(UAnimMontage* Montage)
{
    PlayAnimMontage(Montage, 1.0f);
}

void AMinionCharacter::SetRagdollMulticast_Implementation()
{
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    FCollisionResponseContainer CollisionResponses = GetMesh()->GetCollisionResponseToChannels();
    CollisionResponses.SetResponse(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
    CollisionResponses.SetResponse(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    CollisionResponses.SetResponse(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

    GetMesh()->SetAllBodiesBelowSimulatePhysics(FName(TEXT("pelvis")), true, true);
    GetMesh()->SetCollisionResponseToChannels(CollisionResponses);

    //GetMesh()->AddImpulse(-GetActorForwardVector() * 150000.0f, FName(TEXT("pelvis")));

    SetLifeSpan(3.0f);
}

void AMinionCharacter::OnHit(float InDamage, int InHitIdx)
{
    if (HasAuthority())
    {
        if (IsDead)
        {
            return;
        }

        this->Hp = this->Hp - InDamage;

        if (this->Hp <= 0)
        {
            this->Hp = 0;

            SetDead();
        }
        else
        {
            PlayMontageMulticast(HitMontages[InHitIdx]);
        }
    }
}

void AMinionCharacter::SetDead()
{
    if (HasAuthority())
    {
        this->IsDead = true;

        if (this->OwnerSpawner->IsValidLowLevel())
        {
            ISpawner* Spawner = Cast<ISpawner>(this->OwnerSpawner);
            Spawner->OnSpawnableDead(this);
        }

        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        int DeathIndex = FMath::Rand() % DeathMontages.Num();

        UAnimMontage* DeathMontage = DeathMontages[0];
        PlayMontageMulticast(DeathMontage);


        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
            UnusedHandle, this, &AMinionCharacter::SetRagdollMulticast, DeathMontage->GetPlayLength() * 0.8f, false);
    }
}

void AMinionCharacter::SetRoaming()
{
    if (this->IsDead)
    {
        return;
    }

    if (!this->IsCombat)
    {
        AAIController* AICon = Cast<AAIController>(GetController());
        if (AICon != nullptr)
        {
            FVector RandomLoc;
            if (GetRandomPointInRadius(this->StartLocation, 1000.0f, RandomLoc))
            {
                if (AICon->MoveToLocation(RandomLoc, 70, false))
                {
                    float WaitTime = 5.0f;

                    FTimerHandle UnusedHandle;
                    GetWorldTimerManager().SetTimer(
                        UnusedHandle, this, &AMinionCharacter::SetRoaming, WaitTime, false);
                }
                else
                {
                    ClearTarget();
                }
            }
        }
    }
}

void AMinionCharacter::SetTarget(AActor* InTarget)
{
    this->Target = InTarget;

    if (this->IsDead)
    {
        return;
    }

    if (this->Target->IsValidLowLevel())
    {
        this->IsCombat = true;
        SetActorTickEnabled(true);
        this->StartCombatLocation = GetActorLocation();

        SetAIMovementType(EAIMovementType::EAM_Running);
        SetAgro();
    }
    else
    {
        this->IsCombat = false;
        SetActorTickEnabled(false);
        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
            UnusedHandle, this, &AMinionCharacter::SetRoaming, 1.0f, false);
    }
}

void AMinionCharacter::SetAgro()
{
    GetController()->StopMovement();
    ChaseTarget();
}

void AMinionCharacter::ChaseTarget()
{
    if (this->IsDead)
    {
        return;
    }

    if (this->IsCombat)
    {
        AAIController* AICon = Cast<AAIController>(GetController());
        if (AICon != nullptr)
        {
            if (AICon->MoveToActor(this->Target, this->MeleeAttackDistance * 0.5f))
            {
                RunAttack();
            }
            else
            {
                ClearTarget();
            }
        }
    }
}

void AMinionCharacter::RunAttack()
{
    if (this->IsDead)
    {
        return;
    }

    if (this->Target->IsValidLowLevel())
    {
        float Distance = GetDistanceTo(this->Target);
        float WaitLength = PickAttack(Distance) + 0.5f;

        FTimerHandle UnusedHandle;
        GetWorldTimerManager().SetTimer(
            UnusedHandle, this, &AMinionCharacter::ChaseTarget, WaitLength, false);
    }
    else
    {
        ClearTarget();
    }
}

void AMinionCharacter::SetAIMovementType(EAIMovementType MoveType)
{
    if (MoveType == EAIMovementType::EAM_Idle)
    {
        GetCharacterMovement()->MaxWalkSpeed = 0.0f;
    }
    else if (MoveType == EAIMovementType::EAM_Walking)
    {
        GetCharacterMovement()->MaxWalkSpeed = this->WalkSpeed;
    }
    else if (MoveType == EAIMovementType::EAM_Running)
    {
        GetCharacterMovement()->MaxWalkSpeed = this->WalkSpeed * this->RunSpeedMultiplier;
    }
}


float AMinionCharacter::PickAttack(float DistanceToPlayer)
{
    if (DistanceToPlayer <= this->MeleeAttackDistance)
    {
        UAnimMontage* PlayMontage = this->AttackMontages[0];
        PlayMontageMulticast(PlayMontage);

        GetController()->StopMovement();

        return PlayMontage->GetPlayLength();
    }
    return 0.0f;
}


bool AMinionCharacter::GetRandomPointInRadius(const FVector& Origin, float Radius, FVector& OutResult)
{
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSys)
    {
        return false;
    }

    FNavLocation Result;

    //This is a wrapper for UE4 version
    bool bSuccess = NavSys->GetRandomReachablePointInRadius(Origin, Radius, Result);

    //Out
    OutResult = Result.Location;
    return bSuccess;
}

void AMinionCharacter::ClearTarget()
{
    this->Target = nullptr;
}



