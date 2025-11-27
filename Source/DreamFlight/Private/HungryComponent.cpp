// Fill out your copyright notice in the Description page of Project Settings.


#include "HungryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RavenCharacter.h"

// Sets default values for this component's properties
UHungryComponent::UHungryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHungryComponent::BeginPlay()
{
	Super::BeginPlay();

	RavenCharacter = Cast<ARavenCharacter>(GetOwner());
	if (RavenCharacter)
	{
		 RavenMovementComp = RavenCharacter->GetCharacterMovement();
	}
	
}


// Called every frame
void UHungryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CalculateHunger();
	
	
}

float UHungryComponent::CalculateHunger()
{
	if ( RavenMovementComp->MovementMode == MOVE_Flying )
	{
		Hunger -= GetWorld()->GetDeltaSeconds() * 2;
	}
	return FMath::Clamp(Hunger, MinHunger, MaxHunger);
}

float UHungryComponent::RecoverHunger(float feed)
{
	Hunger += feed;
	return Hunger;
}

