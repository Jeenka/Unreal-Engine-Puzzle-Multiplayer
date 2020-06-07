// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));

	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(const TArray<FServerData> ServerDatas)
{
	if (!ensure(ServerRowClass != nullptr)) return;

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 RowIndex = 0;
	for (const FServerData& ServerData : ServerDatas)
	{
		UServerRow* CurrentCreatedRow = CreateWidget<UServerRow>(World, ServerRowClass);

		if (!ensure(CurrentCreatedRow != nullptr)) return;

		CurrentCreatedRow->ServerName->SetText(FText::FromString(ServerData.Name));
		CurrentCreatedRow->HostName->SetText(FText::FromString(ServerData.HostUserName));

		FString NumberOfPlayers = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayersNum, ServerData.MaxPlayersNum);

		CurrentCreatedRow->NumberOfPlayers->SetText(FText::FromString(NumberOfPlayers));
		CurrentCreatedRow->Setup(this, RowIndex);

		++RowIndex;

		ServerList->AddChild(CurrentCreatedRow);
	}
}

void UMainMenu::SetServerIndex(const uint32 SelectedIndex)
{
	SelectedServerIndex = SelectedIndex;

	UpdateChildrenRows();
}

void UMainMenu::UpdateChildrenRows()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* CurrentRow = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (CurrentRow)
		{
			CurrentRow->IsSelected = (SelectedServerIndex.IsSet() && SelectedServerIndex.GetValue() == i);
			CurrentRow->SetupColors();
		}
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(StartHostButton != nullptr)) return false;
	StartHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(BackFromJoinMenuButton != nullptr)) return false;
	BackFromJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(BackFromHostButton != nullptr)) return false;
	BackFromHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinServerButton != nullptr)) return false;
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGamePressed);

	return true;
}

void UMainMenu::HostServer()
{
	if (!ensure(MenuInterface != nullptr)) return;
	FString ServerName = ServerHostNameTextBox->Text.ToString();
	MenuInterface->Host(ServerName);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::JoinServer()
{
	if (SelectedServerIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index: %d"), SelectedServerIndex.GetValue());

		MenuInterface->Join(SelectedServerIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index is not set"));
	}
}

void UMainMenu::QuitGamePressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}
