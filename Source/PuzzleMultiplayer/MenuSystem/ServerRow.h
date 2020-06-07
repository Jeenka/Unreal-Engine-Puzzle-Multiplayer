// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"
#include "ServerRow.generated.h"

class UTextBlock;
class UMainMenu;
class UButton;

/**
 * 
 */
UCLASS()
class PUZZLEMULTIPLAYER_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HostName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NumberOfPlayers;

	UPROPERTY(BlueprintReadOnly)
	bool IsSelected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor SelectedHoveredColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor UnselectedHoveredColor;

public:

	void Setup(UMainMenu* InParent, uint32 InIndex);

	void SetupColors();

private:

	UPROPERTY(meta = (BindWidget))
	UButton* RowButton;

	UPROPERTY()
	UMainMenu* Parent;

	uint32 Index;

private:

	UFUNCTION()
	void OnClicked();

};
