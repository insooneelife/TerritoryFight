// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYFIGHT_API UObjectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void UpdateHealth(float InPercent);

private:
    UPROPERTY(meta = (BindWidget))
        class UProgressBar* HealthBar;
};
