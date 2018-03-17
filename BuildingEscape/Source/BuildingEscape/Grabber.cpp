// Copyright Notice Arun Vish

#include "Grabber.h"
#include "Engine/World.h" // needed for UWorld type
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::Grab()
{
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto hitResult = GetFirstPhysicsBodyWithinReach();
	auto ComponentToGrab = hitResult.GetComponent(); //Get the mesh in our case
	auto ActorHit = hitResult.GetActor();

	// if we hit something attach physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());
	}
	
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

/// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not contain a Physics Handler!"), *(GetOwner()->GetName()))
	}
}

/// Look for attached Input Component(appears at runtime only)
void UGrabber::SetupInputComponent()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle)
	{
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not contain an Input Handler!"), *(GetOwner()->GetName()))
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyWithinReach()
{
	/// Draw a red trace in the world to visualize
	/*DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0),
	false, 0, 0, 20.f);*/

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Line Tracing (AKA ray-cast) out to reach distance
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT hitResult, GetReachLineStart(), GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	return hitResult;
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached move the object we are holding	
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart()
{
	// perform line tracing to detect physics body type object
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	/*UE_LOG(LogTemp, Warning, TEXT("Player at %s reporting for duty at %s!"),
	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString());*/
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	// perform line tracing to detect physics body type object
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	/*UE_LOG(LogTemp, Warning, TEXT("Player at %s reporting for duty at %s!"),
	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString());*/
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

