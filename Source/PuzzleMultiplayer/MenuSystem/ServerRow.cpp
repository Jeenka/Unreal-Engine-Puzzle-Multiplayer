// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;

	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::SetupColors()
{
	if (IsSelected)
	{
		RowButton->WidgetStyle.Hovered.TintColor = SelectedHoveredColor;
		RowButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::Box;
	}
	else
	{
		RowButton->WidgetStyle.Hovered.TintColor = UnselectedHoveredColor;
		RowButton->WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::NoDrawType;
	}
}

void UServerRow::OnClicked()
{
	Parent->SetServerIndex(Index);
}
