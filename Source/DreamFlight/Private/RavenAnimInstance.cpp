// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenAnimInstance.h"
#include "RavenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void URavenAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	RavenCharacter = Cast<ARavenCharacter>(TryGetPawnOwner());
    RavenCharacterMovement = RavenCharacter ? RavenCharacter->GetCharacterMovement() : nullptr;
}

void URavenAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (RavenCharacter )
	{
		LocomotionDirection = RavenCharacter->GetLocomotionDirection();
		LocomotionState  = RavenCharacter->GetLocomotionState();
		
		
	}

	
}

void URavenAnimInstance::AnimNotify_StartJump()
{
	FVector CurrentLocation = RavenCharacter->GetActorLocation();
	FVector DesiredLoc = CurrentLocation + FVector::UpVector * 15;
	RavenCharacter->SetActorLocation(DesiredLoc);
	
}

void URavenAnimInstance::AnimNotify_Land()
{
	if (RavenCharacter && RavenCharacterMovement)
		RavenCharacterMovement->SetMovementMode(MOVE_Walking);
}
