// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UPanelWidget;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_USTRUCT_BODY()

	FString Name;
	uint16 CurrentPlayersNum;
	uint16 MaxPlayersNum;
	FString HostUserName;

};

/**
 * Main menu that holds all buttons and multiple layers
 */
UCLASS()
class PUZZLEMULTIPLAYER_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(const TArray<FServerData> ServerDatas);

	void SetServerIndex(const uint32 SelectedIndex);

	void UpdateChildrenRows();

protected:

	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackFromJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackFromHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* StartHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerHostNameTextBox;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	TSubclassOf<UUserWidget> ServerRowClass;

	TOptional<uint32> SelectedServerIndex;

private:

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGamePressed();

};
