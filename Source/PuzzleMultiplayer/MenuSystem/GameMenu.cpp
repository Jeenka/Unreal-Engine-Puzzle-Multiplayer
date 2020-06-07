// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

bool UGameMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UGameMenu::CloseMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UGameMenu::QuitGame);

	return true;
}

void UGameMenu::QuitGame()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenuLevel();
	}
}

void UGameMenu::CloseMenu()
{
	Teardown();
}
