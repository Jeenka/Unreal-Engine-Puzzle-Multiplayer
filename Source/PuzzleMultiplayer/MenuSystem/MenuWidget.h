// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * Menu widget container with construction and destruction functionality for "Menu Interface"
 */
UCLASS()
class PUZZLEMULTIPLAYER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetMenuInterface(IMenuInterface* MenuInterface);

	// Set standard parameters values for menu widget
	void Setup();

	// Removing menu widget
	void Teardown();

protected:

	IMenuInterface* MenuInterface;
	
};
