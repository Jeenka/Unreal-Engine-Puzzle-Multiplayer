// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"

#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
//#include "MenuSystem/GameMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	
	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* CurrentOnlineSubsystem = IOnlineSubsystem::Get();

	if (CurrentOnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem exists %s"), *CurrentOnlineSubsystem->GetSubsystemName().ToString());

		SessionInterface = CurrentOnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem not found"));
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenuInterface()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;	

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* NewMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(NewMenu != nullptr)) return;

	NewMenu->Setup();
	NewMenu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting session search"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenuLevel()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool IsSessionCreated)
{
	if (!IsSessionCreated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 3.f, FColor::Yellow, TEXT("Host command"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool IsSessionDeleted)
{
	if (IsSessionDeleted)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool IsSessionFound)
{
	if (IsSessionFound && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished session search"));

		TArray<FServerData> ServerDatas;

		for (FOnlineSessionSearchResult& SessionSearchReault : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Session ID: %s"), *SessionSearchReault.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayersNum = SessionSearchReault.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayersNum = Data.MaxPlayersNum - SessionSearchReault.Session.NumOpenPublicConnections;
			Data.HostUserName = SessionSearchReault.Session.OwningUserName;

			FString SessionSettingsSearchResult;
			if (SessionSearchReault.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, SessionSettingsSearchResult))
			{
				Data.Name = SessionSettingsSearchResult;
			}
			else
			{
				Data.Name = "Could not find name.";
			}

			ServerDatas.Add(Data);
		}

		Menu->SetServerList(ServerDatas);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	FString AddressToConnect;

	if (SessionInterface->GetResolvedConnectString(SessionName, AddressToConnect))
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;

		Engine->AddOnScreenDebugMessage(0, 3.f, FColor::Green, FString::Printf(TEXT("Joining to: %s"), *AddressToConnect));

		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		PlayerController->ClientTravel(AddressToConnect, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Get Resolved Connect String"));
	}
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettiings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettiings.bIsLANMatch = true;
		}
		else
		{
			SessionSettiings.bIsLANMatch = false;
		}

		SessionSettiings.NumPublicConnections = 5;
		SessionSettiings.bShouldAdvertise = true;
		SessionSettiings.bUsesPresence = true;
		SessionSettiings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, EName::NAME_GameSession, SessionSettiings);
	}
}

void UPuzzlePlatformsGameInstance::Host(const FString& ServerName)
{
	if (SessionInterface.IsValid())
	{	
		DesiredServerName = ServerName;

		if (FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(EName::NAME_GameSession))
		{
			SessionInterface->DestroySession(EName::NAME_GameSession);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(const uint32& ServerIndex)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	if (!SessionSearch.IsValid())
	{
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, EName::NAME_GameSession, SessionSearch->SearchResults[ServerIndex]);
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(EName::NAME_GameSession);
	}
}
