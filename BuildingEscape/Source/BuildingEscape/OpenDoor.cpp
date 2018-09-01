// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!PressurePlate_Table || !PressurePlate_Chair)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *GetOwner()->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorOnChairPlate() > TriggerMass_Chair && GetTotalMassOfActorOnTablePlate() > TriggerMass_Table)
	{ 
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}	

float UOpenDoor::GetTotalMassOfActorOnChairPlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	if (!PressurePlate_Chair) { return TotalMass; }
	PressurePlate_Chair->GetOverlappingActors(OUT OverLappingActors);
	// iterate through them adding their masses
	for (const auto* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

float UOpenDoor::GetTotalMassOfActorOnTablePlate()
{
	float TotalMass = 0.f;
	// find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	if (!PressurePlate_Table) { return TotalMass; }
	PressurePlate_Table->GetOverlappingActors(OUT OverLappingActors);
	// iterate through them adding their masses
	for (const auto* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}