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
		UE_LOG(LogTemp, Error, TEXT("No Trigger Volume!"));
		return;
	}
}

void UOpenDoor::DoorOpen()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::DoorClose()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto m = GetTotalMassOnPlate();
	if (m > 30.f)
	{
		DoorOpen();
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}

	FString TheFloatStr = FString::SanitizeFloat(m);
	UE_LOG(LogTemp, Warning, TEXT("Mass on plate is %s"), *TheFloatStr)

	// check if it is time to close the door
	if ((GetWorld()->GetTimeSeconds() - DoorLastOpen) > DoorDelayTime)
	DoorClose();
}

float UOpenDoor::GetTotalMassOnPlate()
{
	TArray<AActor*> OverlappingActors;
	float totalMass = 0.f;
	// Find all overlapping actors
	if (!PressurePlate)
		return;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* pActor : OverlappingActors)
	{
		totalMass += pActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on plate"), *pActor->GetName())
	}
	return totalMass;
}

