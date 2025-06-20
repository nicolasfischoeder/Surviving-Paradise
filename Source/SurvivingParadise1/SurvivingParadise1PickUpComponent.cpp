// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivingParadise1PickUpComponent.h"

USurvivingParadise1PickUpComponent::USurvivingParadise1PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void USurvivingParadise1PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &USurvivingParadise1PickUpComponent::OnSphereBeginOverlap);
}

void USurvivingParadise1PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ASurvivingParadise1Character* Character = Cast<ASurvivingParadise1Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
