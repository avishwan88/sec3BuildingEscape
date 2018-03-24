// Copyright Notice Arun Vish

#include "OpenDoor.h"
#include "Engine/World.h" // needed for UWorld type
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

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
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No Pressure Plate for %s!"), *Owner->GetName());
		return;
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
	
}

float UOpenDoor::GetTotalMassOnPlate()
{
	TArray<AActor*> OverlappingActors;
	float totalMass = 0.f;
	// Find all overlapping actors
	if (!PressurePlate)
		return totalMass;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* pActor : OverlappingActors)
	{
		totalMass += pActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *pActor->GetName())
	}
	return totalMass;
}

