// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEMULTIPLAYER_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	AMovingPlatform();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

private:

	UPROPERTY(EditAnywhere, Category = "Setup")
	float Speed {25};

	// Meta = (MakeEditWidget = true) makes TargetLocation visible in editor as gizmo
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true), Category = "Setup")
	FVector TargetLocation;

	UPROPERTY(EditAnywhere)
	int ActiveTriggers = 1;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;
};
