// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    this->RootCapsule = Cast<UCapsuleComponent>(this->RootComponent);

    this->RHand = GetMesh()->GetBodyInstance("hand_r");
    this->LHand = GetMesh()->GetBodyInstance("hand_l");

    const FVector Location = FVector(0, 0, 0);
    const FRotator Rotation = FRotator::ZeroRotator;

    this->Hp = 100;
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyCharacter, Hp);
    DOREPLIFETIME(AMyCharacter, LastAttackSide);
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyCharacter::OnAttackPress);
}



void AMyCharacter::OnAttackPress()
{
    AttackRPC();
}

void AMyCharacter::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
}

void AMyCharacter::OnEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}

void AMyCharacter::OnAttacked(float InDamage, int Side)
{
    UE_LOG(LogTemp, Warning, TEXT("OnAttacked !!!!!!!!!!!!!!!!!!!!! "));
    SetHp(GetHp() - InDamage);
    LastAttackSide = Side;
}

void AMyCharacter::OnRep_Hp()
{
    UE_LOG(LogTemp, Warning, TEXT("OnRep_Hp !!!!!!!!!!!!!!!!!!!!!  %d"), LastAttackSide);
    PlayAnimMontage(HitMontages[LastAttackSide], 1.0f);
}


void AMyCharacter::AttackRPC_Implementation()
{
    AttackMulticast();
}

void AMyCharacter::AttackMulticast_Implementation()
{
    if (IsAttacking)
    {
        SaveAttack = true;
    }
    else
    {
        IsAttacking = true;
        PlayNextAttackAnimMontage();
    }
}


void AMyCharacter::AttackStart()
{
    FTransform T;

    if (this->AttackCount == 1 || this->AttackCount == 3)
    {
        T = this->RHand->GetUnrealWorldTransform();
    }
    else
    {
        T = this->LHand->GetUnrealWorldTransform();
    }
    this->SaveAttackStartPos = T.GetLocation();
}


void AMyCharacter::AttackEnd()
{
    FTransform T;
    if (this->AttackCount == 1 || this->AttackCount == 3)
    {
        T = this->RHand->GetUnrealWorldTransform();
    }
    else
    {
        T = this->LHand->GetUnrealWorldTransform();
    }

    // logic must be called at server
    if (HasAuthority())
    {
        SphereSweep(this->SaveAttackStartPos, T.GetLocation(), 50.0f);
    }
}


void AMyCharacter::ResetCombo()
{
    this->AttackCount = 0;
    this->SaveAttack = false;
    this->IsAttacking = false;
}

void AMyCharacter::ComboAttackSave()
{
    if (SaveAttack)
    {
        SaveAttack = false;

        PlayNextAttackAnimMontage();
    }
}


void AMyCharacter::PlayNextAttackAnimMontage()
{
    PlayAnimMontage(AttackMontages[AttackCount++], 1.0f);
    AttackCount = AttackCount % 4;
}


void AMyCharacter::SphereSweep(FVector Start, FVector End, float Radius)
{
    FCollisionQueryParams CollParam;
    FCollisionObjectQueryParams CollObjParam(ECollisionChannel::ECC_Pawn);
    CollParam.AddIgnoredActor(this);

    FHitResult OutHit;

    if (GetWorld()->SweepSingleByObjectType(
        OutHit,
        Start,
        End,
        FQuat::Identity,
        CollObjParam,
        FCollisionShape::MakeSphere(Radius),
        CollParam))
    {

        //UE_LOG(LogTemp, Warning, TEXT("Collision !!!!!!!!!!!!!!!!!!!!!  %s"),
        //    *OutHit.GetActor()->GetFullName(),
        //    *OutHit.GetComponent()->GetFullName());

        AMyCharacter* HitCharacter = Cast<AMyCharacter>(OutHit.GetActor());
        if (HitCharacter)
        {
            HitCharacter->OnAttacked(10, 1);
            //HitCharacter->SetHp(HitCharacter->GetHp() - 10);
        }
    }

    GetWorld()->SpawnActor<AActor>(AttackClass, End, FQuat::Identity.Rotator());
}