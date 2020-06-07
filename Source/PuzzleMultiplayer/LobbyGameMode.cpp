// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		// Set timer to start game after 10 seconds if 2 players connected
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if (!GameInstance)
	{
		return;
	}

	bUseSeamlessTravel = true;
	GameInstance->StartSession();
	World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
}
