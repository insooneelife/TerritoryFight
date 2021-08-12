// Fill out your copyright notice in the Description page of Project Settings.

#include "TerritoryFightGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");
const static FString MAIN_MENU_MAP_PATH = TEXT("/Game/Maps/MainMenu");
const static FString LOBBY_MAP_PATH = TEXT("/Game/Maps/Lobby");


UTerritoryFightGameInstance::UTerritoryFightGameInstance(const FObjectInitializer& ObjectInitializer)
{
    ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
    if (!ensure(MenuBPClass.Class != nullptr)) return;

    MenuClass = MenuBPClass.Class;

    ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
    if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

    InGameMenuClass = InGameMenuBPClass.Class;
}

void UTerritoryFightGameInstance::Init()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem != nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
        SessionInterface = Subsystem->GetSessionInterface();
        if (SessionInterface.IsValid()) {
            SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
                this, &UTerritoryFightGameInstance::OnCreateSessionComplete);

            SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(
                this, &UTerritoryFightGameInstance::OnCreateSessionComplete);

            SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
                this, &UTerritoryFightGameInstance::OnFindSessionsComplete);

            SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
                this, &UTerritoryFightGameInstance::OnJoinSessionComplete);
        }
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
    }
}

void UTerritoryFightGameInstance::LoadMenuWidget()
{
    if (!ensure(MenuClass != nullptr)) return;

    MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
    if (!ensure(MainMenu != nullptr)) return;

    MainMenu->Setup();

    MainMenu->SetMenuInterface(this);
}

void UTerritoryFightGameInstance::InGameLoadMenu()
{
    if (!ensure(InGameMenuClass != nullptr)) return;

    UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
    if (!ensure(InGameMenu != nullptr)) return;

    InGameMenu->Setup();
    InGameMenu->SetMenuInterface(this);
    
}


void UTerritoryFightGameInstance::Host(FString ServerName)
{
    DesiredServerName = ServerName;
    if (SessionInterface.IsValid())
    {
        auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
        if (ExistingSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession();
        }
    }
}

void UTerritoryFightGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
    if (Success) {
        CreateSession();
    }
}

void UTerritoryFightGameInstance::CreateSession()
{
    if (SessionInterface.IsValid()) {
        FOnlineSessionSettings SessionSettings;
        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            SessionSettings.bIsLANMatch = true;
        }
        else
        {
            SessionSettings.bIsLANMatch = false;
        }
        SessionSettings.NumPublicConnections = 5;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.Set(
            SERVER_NAME_SETTINGS_KEY, DesiredServerName, 
            EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
    }
}

void UTerritoryFightGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
        return;
    }

    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

    UWorld* World = GetWorld();
    if (!ensure(World != nullptr)) return;

    const FString ListenTerm = TEXT("?listen");

    const FString ServerTravelPath = LOBBY_MAP_PATH + ListenTerm;

    World->ServerTravel(ServerTravelPath);
}

void UTerritoryFightGameInstance::RefreshServerList()
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        //SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}

void UTerritoryFightGameInstance::OnFindSessionsComplete(bool Success)
{
    if (Success && SessionSearch.IsValid() && MainMenu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

        TArray<FServerData> ServerNames;
        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
            FServerData Data;
            Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
            Data.HostUsername = SearchResult.Session.OwningUserName;
            FString ServerName;
            if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
            {
                Data.Name = ServerName;
            }
            else
            {
                Data.Name = "Could not find name.";
            }
            ServerNames.Add(Data);
        }

        MainMenu->SetServerList(ServerNames);
    }
}

void UTerritoryFightGameInstance::Join(uint32 Index)
{
    if (!SessionInterface.IsValid()) return;
    if (!SessionSearch.IsValid()) return;

    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UTerritoryFightGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (!SessionInterface.IsValid()) return;

    FString Address;
    if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
        UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
        return;
    }

    UEngine* Engine = GetEngine();
    if (!ensure(Engine != nullptr)) return;

    Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UTerritoryFightGameInstance::StartSession()
{
    if (SessionInterface.IsValid())
    {
        SessionInterface->StartSession(SESSION_NAME);
    }
}

void UTerritoryFightGameInstance::LoadMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel(MAIN_MENU_MAP_PATH, ETravelType::TRAVEL_Absolute);
}
