// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayBeamParticle.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "ParticleHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "../GameCore/MinionCharacter.h"

void UAnimNotify_PlayBeamParticle::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
    // Don't call super to avoid unnecessary call in to blueprints
    UParticleSystemComponent* ParticleComp = SpawnParticleSystem(MeshComp, Animation);

    if (ParticleComp->IsValidLowLevel())
    {
        AMinionCharacter* Character = Cast<AMinionCharacter>(MeshComp->GetOwner());

        if (Character->IsValidLowLevel())
        {
            AActor* Target = Character->GetTarget();
            if (Target->IsValidLowLevel())
            {
                ParticleComp->SetBeamTargetPoint(EmitterIndex, Target->GetActorLocation(), TargetIndex);
            }
        }
    }
}