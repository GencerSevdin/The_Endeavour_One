// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "LivingCharacters.h"
#include "GameFramework/SpringArmComponent.h"

#include "Bird.generated.h"

//class FInputActionValue;
UCLASS()
class DREAMFLIGHT_API ABird : public ALivingCharacters
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABird();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetFlyMode(bool bEnable);
	void Movement(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fly(const FInputActionValue& Value);
private:
	
	FRotator MeshBaseRot;
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 1800.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRateDeg = 12;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RollReturnDegPerSec = 50;
	float DesiredPitch = 0;
	float CurrentPitch;
	float TargetMeshPitch;
	UMeshComponent* BirdMeshComponent;

	//Camera orbit state

	float CamYaw = 0;
	float CamPitch = 10.f;

	//Settings
	float CamSensitivity = 30.f; //degree /second (fare/gamepad rate)
	float CamPitchMin = -45.f;
	float CamPitchMax = 45.f;
	float CamYawMin = -45.f;
	float CamYawMax = 45.f;
	float CamLagSpeed = 12.f; //RInterpTo speed
};
