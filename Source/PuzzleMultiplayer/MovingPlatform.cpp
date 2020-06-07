// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) // Setting object replication if the owner is server
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation(); // Getting global start location
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation); // Converting target location to global space
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0) ActiveTriggers--;
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ActiveTriggers > 0)
	{
		// Moving object on the server side
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			float PathLength = (GlobalTargetLocation - GlobalStartLocation).Size(); // Get the length of path from start to end point
			float TravelledPathLength = (Location - GlobalStartLocation).Size(); // Get the length of path from start to current location

			if (TravelledPathLength > PathLength) // Swap directions if object has passed the end point to travel backwards
			{
				FVector Temp = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Temp;
			}

			FVector Direction = GlobalTargetLocation - GlobalStartLocation; // Acquiring direction to desired location
			Direction.Normalize(); // Vector length normalization
			Location += Direction * Speed * DeltaTime;
			SetActorLocation(Location);
		}
	}
}