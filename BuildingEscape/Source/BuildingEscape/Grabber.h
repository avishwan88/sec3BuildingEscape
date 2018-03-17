// Copyright Notice Arun Vish

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far ahead to look in cm?
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputHandle = nullptr;
	//Ray cast and grab what is in reach
	void Grab();
	// called when grab is released
	void Release();
	// Find (assumed) attached physics component
	void FindPhysicsHandleComponent();
	// setup (assumed) attached input component
	void SetupInputComponent();
	// Return hit for first physics body within reach
	FHitResult GetFirstPhysicsBodyWithinReach();
	// calculate start of line
	FVector GetReachLineStart();
	// calculate current end of reach line
	FVector GetReachLineEnd();
};
