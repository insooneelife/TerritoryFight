// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "../MenuSystem/MenuInterface.h"
#include "TerritoryFightGameInstance.generated.h"

/**
 *
 */
UCLASS()
class TERRITORYFIGHT_API UTerritoryFightGameInstance : public UGameInstance, public IMenuInterface
{
    GENERATED_BODY()

public:
    UTerritoryFightGameInstance(const FObjectInitializer& ObjectInitializer);

    virtual void Init();

    UFUNCTION(BlueprintCallable)
        void LoadMenuWidget();

    UFUNCTION(BlueprintCallable)
        void InGameLoadMenu();

    UFUNCTION(Exec)
        void Host(FString ServerName) override;

    UFUNCTION(Exec)
        void Join(uint32 Index) override;

    void StartSession();

    virtual void LoadMainMenu() override;

    void RefreshServerList() override;

private:
    TSubclassOf<class UUserWidget> MenuClass;
    TSubclassOf<class UUserWidget> InGameMenuClass;

    UPROPERTY()
    class UMainMenu* MainMenu;

    IOnlineSessionPtr SessionInterface;
    TSharedPtr<class FOnlineSessionSearch> SessionSearch;

    void OnCreateSessionComplete(FName SessionName, bool Success);
    void OnDestroySessionComplete(FName SessionName, bool Success);
    void OnFindSessionsComplete(bool Success);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

    FString DesiredServerName;
    void CreateSession();
};
