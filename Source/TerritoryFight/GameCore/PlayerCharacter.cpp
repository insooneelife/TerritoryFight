// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "TerritoryFightGameInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    //bUseControllerRotationPitch = false;
    //bUseControllerRotationYaw = false;
    //bUseControllerRotationRoll = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    this->RHand = GetMesh()->GetBodyInstance("hand_r");
    this->LHand = GetMesh()->GetBodyInstance("hand_l");

    const FVector Location = FVector(0, 0, 0);
    const FRotator Rotation = FRotator::ZeroRotator;

    this->Hp = 100;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APlayerCharacter, Hp);
    DOREPLIFETIME(APlayerCharacter, LastAttackSide);
}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("OpenMenu", IE_Pressed, this, &APlayerCharacter::OpenMenu);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);


    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::OnAttackPress);


}

void APlayerCharacter::OpenMenu()
{
    UTerritoryFightGameInstance* GameInst = Cast<UTerritoryFightGameInstance>(GetGameInstance());
    if (GameInst != nullptr)
    {
        GameInst->InGameLoadMenu();
    }
}

void APlayerCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::OnAttackPress()
{
    AttackRPC();
}

void APlayerCharacter::OnBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
}

void APlayerCharacter::OnEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}

void APlayerCharacter::OnHit(float InDamage, int Side)
{
    SetHp(GetHp() - InDamage);
    LastAttackSide = Side;

    OnResetCombo_Implementation();

    PlayMontageMulticast();
}

void APlayerCharacter::OnRep_Hp()
{
    UE_LOG(LogTemp, Warning, TEXT("OnRep_Hp !!!!!!!!!!!!!!!!!!!!!  %d"), LastAttackSide);

}


void APlayerCharacter::AttackRPC_Implementation()
{
    AttackMulticast();
}

void APlayerCharacter::AttackMulticast_Implementation()
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

void APlayerCharacter::PlayMontageMulticast_Implementation()
{
    PlayAnimMontage(HitMontages[LastAttackSide], 1.0f);
}


void APlayerCharacter::OnAttackStart_Implementation()
{
    FTransform T;

    if (this->AttackIdx == 1 || this->AttackIdx == 3)
    {
        T = this->RHand->GetUnrealWorldTransform();
    }
    else
    {
        T = this->LHand->GetUnrealWorldTransform();
    }
    this->SaveAttackStartPos = T.GetLocation();
}


void APlayerCharacter::OnAttackEnd_Implementation()
{
    FTransform T;
    if (this->AttackIdx == 1 || this->AttackIdx == 3)
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


void APlayerCharacter::OnResetCombo_Implementation()
{
    this->AttackIdx = 0;
    this->SaveAttack = false;
    this->IsAttacking = false;
}

void APlayerCharacter::OnSaveAttack_Implementation()
{
    if (SaveAttack)
    {
        SaveAttack = false;

        PlayNextAttackAnimMontage();
    }
}


void APlayerCharacter::PlayNextAttackAnimMontage()
{
    PlayAnimMontage(AttackMontages[AttackIdx], 1.0f);
    AttackIdx = (AttackIdx + 1) % 4;
}


void APlayerCharacter::SphereSweep(FVector Start, FVector End, float Radius)
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

        IHittable* HitCharacter = Cast<IHittable>(OutHit.GetActor());
        if (HitCharacter)
        {
            HitCharacter->OnHit(10, 1);
        }
    }

    GetWorld()->SpawnActor<AActor>(AttackClass, End, FQuat::Identity.Rotator());
}




