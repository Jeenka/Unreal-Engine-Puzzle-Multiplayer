// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UUserWidget;
class UMainMenu;
class FOnlineSessionSearch;

/**
 * 
 */
UCLASS()
class PUZZLEMULTIPLAYER_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

private:

	TSubclassOf<UUserWidget> MenuClass;
	TSubclassOf<UUserWidget> InGameMenuClass;
	UMainMenu* Menu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;

public:

	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMainMenuInterface();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(BlueprintCallable)
	void RefreshServerList();

	UFUNCTION(Exec)
	virtual void Host(const FString& ServerName) override;

	UFUNCTION(Exec)
	virtual void Join(const uint32& ServerIndex) override;

	void StartSession();

private:

	UFUNCTION()
	virtual void LoadMainMenuLevel() override;

	void OnCreateSessionComplete(FName SessionName, bool IsSessionCreated);
	void OnDestroySessionComplete(FName SessionName, bool IsSessionDeleted);
	void OnFindSessionComplete(bool IsSessionFound);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

};
