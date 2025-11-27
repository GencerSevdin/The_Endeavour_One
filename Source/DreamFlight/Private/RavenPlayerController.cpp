// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenPlayerController.h"

// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "RavenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ARavenPlayerController::BeginPlay()
{
	Super::BeginPlay();
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext,0);
		}
		
}

void ARavenPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARavenPlayerController::Move);
		EnhancedInputComponent->BindAction(AccelerationAction, ETriggerEvent::Triggered, this, &ARavenPlayerController::Acceleration);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ARavenPlayerController::Look);
		EnhancedInputComponent->BindAction(SearchPointAction, ETriggerEvent::Triggered, this, &ARavenPlayerController::SearchPearchablePoints);
		EnhancedInputComponent->BindAction(SearchPointAction, ETriggerEvent::Completed, this, &ARavenPlayerController::StopSearchPearchablePoints);
		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Started, this, &ARavenPlayerController::FlyingStart);
		EnhancedInputComponent->BindAction(EatAction, ETriggerEvent::Triggered, this, &ARavenPlayerController::Eat);
		
	}
}



void ARavenPlayerController::Move(const FInputActionValue &Value)
{
	
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	if (Raven != nullptr)
	{
		Raven->Movement(Value);
		
	}
}

void ARavenPlayerController::Look(const FInputActionValue& Value)
{
	
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());

	if (Raven != nullptr)
	{
		Raven->Look(Value);
	}
	
	
}

void ARavenPlayerController::Eat(const FInputActionValue& Value)
{
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	
	if (Raven != nullptr && Raven->GetCharacterMovement()->MovementMode == MOVE_Walking )
		Raven->Eat(Value);
}


void ARavenPlayerController::FlyingStart(const FInputActionValue& Value)
{
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	bAltMode = !bAltMode;
	if (Raven != nullptr)
    {
			Raven->SetFlyMode(bAltMode);
       		//UE_LOG(LogTemp, Log, TEXT("BirdController::Up"));
	}
	
	
}

void ARavenPlayerController::Acceleration(const FInputActionValue& Value)
{
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	bChangeSpeed = !bChangeSpeed;
	if (Raven != nullptr)
	{
		Raven->Acceleration(bChangeSpeed);
	}
}

void ARavenPlayerController::SearchPearchablePoints(const FInputActionValue& Value)
{
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	if (Raven != nullptr)
	{
		Raven->StartToSearch();
	}
	
}

void ARavenPlayerController::StopSearchPearchablePoints(const FInputActionValue& Value)
{
	ARavenCharacter* Raven = Cast<ARavenCharacter>(GetPawn());
	if (Raven != nullptr)
	{
		Raven->StopToSearch();
	}
}


