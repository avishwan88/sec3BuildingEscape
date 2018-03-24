// Copyright Notice Arun Vish

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDoorRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void DoorOpen();
	void DoorClose();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOpenDoorRequest OpenDoorRequest;
	
private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.0f;

	UPROPERTY(EditAnywhere)
	float DoorDelayTime = 1.f;

	float DoorLastOpen;

	UPROPERTY(EditAnywhere)
	ATriggerVolume * PressurePlate = nullptr;

	// Actor that affects trigger volume
	//UPROPERTY(EditAnywhere)
	//AActor* ActorThatOpens;
	AActor* Owner = nullptr;

	float GetTotalMassOnPlate();
	
};
