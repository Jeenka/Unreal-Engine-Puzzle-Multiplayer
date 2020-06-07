// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "GameMenu.generated.h"

/**
 * In-game "escape" menu
 */
UCLASS()
class PUZZLEMULTIPLAYER_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void CloseMenu();
};
