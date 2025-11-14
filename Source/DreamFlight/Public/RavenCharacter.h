// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEnumType.h"
#include "GameFramework/Character.h"
#include "LivingCharacters.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "LandscapeNaniteComponent.h"
#include "RavenCharacter.generated.h"

//class FInputActionValue;
class UCameraComponent;
class USplineComponent;
UCLASS()
class DREAMFLIGHT_API ARavenCharacter : public ALivingCharacters
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ARavenCharacter();
	void SetFlyMode(bool bEnable);
	void Movement(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartToSearch();
	void StopToSearch();
	void Eat(const FInputActionValue& Value);
	void Acceleration(bool bSpeedUp);
	bool bValueSpeedUp = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCanFlap;
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState;}
	UPROPERTY(BlueprintReadOnly)
	float DistanceToFloor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	FRotator MeshBaseRot;
	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 1800.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRateDeg = 20;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RollReturnDegPerSec = 50;
	float DesiredPitch = 0;
	float DesiredRoll = 0;
	float CurrentPitch;
	float CurrentRoll;
	float TargetMeshPitch;
	float TargetMeshRoll;
	UMeshComponent* BirdMeshComponent;
	//Camera orbit state
	
	float CamYaw = 0;
	float CamPitch = 10.f;

	//Settings
	float CamSensitivity = 30.f; //degree /second (fare/gamepad rate)
	float CamPitchMin = -50.f;
	float CamPitchMax = 50.f;
	float CamYawMin = -45.f;
	float CamYawMax = 45.f;
	float CamLagSpeed = 12.f; //RInterpTo speed

	UCharacterMovementComponent* RavenCharacterMovement;	
	UPROPERTY(EditAnywhere, Category = "Movement")
	UAnimMontage* Eating;
	ECharacterState CharacterState = ECharacterState::Hover;
	
	//Konma sistemi 
	AController* GetOwnerController() const;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(15.f);
	FHitResult Floor;
	bool  CanPearch(USceneComponent*& OutComponent, FTransform& OutComponentTransform);
	//bool CanPearch();
	bool CalcDistanceToFloor(FHitResult& OutDistance);
	FVector BirdCurrLoc;
	USceneComponent* PearchComponent;
	USceneComponent* PearchComp;
	//FVector ComponentLocation;
	FVector CompLoc;
	FTransform ComponentTransform;
	FTransform PearchTransform;
	FVector DesiredLocation = FVector::ZeroVector;
	FVector Location;
	FRotator Rotation;
	bool bIsLanding = false;
	bool bHasValidPearchPoint;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PearchApproachSpeed = 300;
	TArray<USceneComponent*>SceneComponents;

	float TransformedSin();
	float RunningTime;
	float Amplitude = 1.f;
	float TimeConstant = 10.f;	
	//Kamera Efektleri
	UPROPERTY(EditAnywhere) float BaseFOV = 90.f;      // normal görüş
	UPROPERTY(EditAnywhere) float MinFOV  = 120.f;      // tam hızda genis FOV
	UPROPERTY(EditAnywhere) float SlowSpeed = 900.f;   // bu hızda BaseFOV
	UPROPERTY(EditAnywhere) float FastSpeed = 1800.f;// bu hızda MinFOV
	UPROPERTY(EditAnywhere) float MaxSpeed = 2500;
	UPROPERTY(EditAnywhere) float FOVInterpSpeed = 6.f;
		//Kamera Tunellesme efekt ayarlari
	UPROPERTY(EditAnywhere) float MinVignette = 0.6f;
	UPROPERTY(EditAnywhere) float MaxVignette =1.F;
	float MaxCAIntensity = 50.F;
	float CAInterpSpeed = 5.f;
	// ===== SPLINE LANDING SYSTEM =====
	UPROPERTY()
	class USplineComponent* LandingSpline = nullptr;

	
	
};
