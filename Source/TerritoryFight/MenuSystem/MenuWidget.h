// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MenuWidget.generated.h"

/**
 *
 */
class IMenuInterface;
UCLASS()
class TERRITORYFIGHT_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void Setup();
    void Teardown();

    void SetMenuInterface(IMenuInterface* InMenuInterface);

protected:
    // Weak Ref?
    IMenuInterface* MenuInterface;
};
