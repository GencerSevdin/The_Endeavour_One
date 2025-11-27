// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterEnumType.h"
#include "GameFramework/Character.h"
#include "LivingCharacters.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "RavenCharacter.generated.h"

//class FInputActionValue;
class UCameraComponent;
class USplineComponent;
class UCharacterStatComponent;
class AItems;
class AEatableItems;
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
	void AddUpOffset();
	bool bValueSpeedUp = false;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	FVector VelocityOfCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCanFlap;
	FORCEINLINE ELocomotionState GetLocomotionState() const { return LocomotionState;}
	FORCEINLINE ELocomotionDirection GetLocomotionDirection()const {return LocomotionDirection;} 
	FORCEINLINE EGroundActions GetGroundActionState() const { return GroundActionState;}
	UPROPERTY(BlueprintReadOnly)
	float DistanceToFloor;
	
	FORCEINLINE void SetOverlappingItem(AItems* Item){OverlappingItem = Item;}
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
	float RollReturnDegPerSec = 15;
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
	UCharacterStatComponent* RavenStatComp;
	
	ELocomotionState LocomotionState;
	ELocomotionDirection LocomotionDirection;
	EGroundActions GroundActionState;
	
	//Konma sistemi 
	FCollisionShape Sphere = FCollisionShape::MakeSphere(15.f);
	FHitResult Floor;
	bool  CanPearch(USceneComponent*& OutComponent);
	//bool CanPearch();
	bool CalcDistanceToFloor(FHitResult& OutDistance);
	FVector BirdCurrLoc;
	USceneComponent* PearchComponent;
	USceneComponent* PearchComp;
	//FVector ComponentLocation;
	FVector CompLoc;
	FVector DesiredLocation = FVector::ZeroVector;
	bool bIsLanding = false;
	bool bHasValidPearchPoint;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float PearchApproachSpeed = 300;
	

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
	UPROPERTY(VisibleAnywhere, Category = "Landing")
	USplineComponent* LandingSpline = nullptr;

	//Find Spline Way
	FVector ComputeStageMid(float Distance, float SideOffset);
	bool IsPathClear(const FVector& Start, const FVector& End);
	FVector LandingMid;
	bool bHasLandingSpline = false;
	float LandingSplineDistance = 0.f;
	UPROPERTY(EditAnywhere, Category = "Landing")
	float LandingSplineSpeed = 500;
	void BuildLandingSpline();
	//-----------EatActionMontage------------------
	UPROPERTY(EditAnywhere)
	UAnimMontage* EatMontage;
	void PlayEatMontage();
	UFUNCTION(BlueprintCallable)
	void EndEatMontage();
	UFUNCTION(BlueprintCallable)
	void OnEatNotify();
	AItems* OverlappingItem;
	AEatableItems* EatingTarget;
	
};
