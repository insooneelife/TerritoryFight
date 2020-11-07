// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../TerritoryFightGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 *
 */
UCLASS()
class TERRITORYFIGHT_API ALobbyGameMode : public ATerritoryFightGameMode
{
    GENERATED_BODY()

public:

    virtual void BeginPlay() override;

    void PostLogin(APlayerController* NewPlayer) override;

    void Logout(AController* Exiting) override;

private:

    void StartGame();

    uint32 NumberOfPlayers = 0;

    FTimerHandle GameStartTimer;
};
