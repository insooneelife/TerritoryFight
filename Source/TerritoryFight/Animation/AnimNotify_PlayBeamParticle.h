// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "AnimNotify_PlayBeamParticle.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYFIGHT_API UAnimNotify_PlayBeamParticle : public UAnimNotify_PlayParticleEffect
{
	GENERATED_BODY()
	
public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
        int EmitterIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
        int TargetIndex;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
