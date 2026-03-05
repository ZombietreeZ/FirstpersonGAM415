// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firstperson415PickUpComponent.h"

UFirstperson415PickUpComponent::UFirstperson415PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UFirstperson415PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UFirstperson415PickUpComponent::OnSphereBeginOverlap);
}

void UFirstperson415PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AFirstperson415Character* Character = Cast<AFirstperson415Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
