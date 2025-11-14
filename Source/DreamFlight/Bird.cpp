// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABird::ABird()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();
	BirdMeshComponent = GetMesh();
	MeshBaseRot = BirdMeshComponent->GetRelativeRotation();
	SpringArm = FindComponentByClass<USpringArmComponent>();
}

void ABird::SetFlyMode(bool bEnable)
{
	UCharacterMovementComponent* BirdCharacterMovement = GetCharacterMovement();
	if (bEnable)
	{
		BirdCharacterMovement ->SetMovementMode(MOVE_Flying);
		BirdCharacterMovement ->GravityScale = 5;
		//BirdCharacterMovement->MaxFlySpeed = 1800.f;
		UE_LOG(LogTemp, Warning, TEXT("BirdCharacterMovement Move_Flying"));
	}
	else
	{
		BirdCharacterMovement ->GravityScale = 0.18;
		BirdCharacterMovement ->SetMovementMode(MOVE_Falling);
		UE_LOG(LogTemp, Warning, TEXT("BirdCharacterMovement Move_Falling"));
	}
	
}

void ABird::Movement(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const float DT = GetWorld()->GetDeltaSeconds();
	FVector ForwardVector  = GetActorForwardVector();
	float SmoothTurnRateDeg = FMath::FInterpConstantTo(0, MovementVector.X, DT, TurnRateDeg);
	AddMovementInput(ForwardVector,  MovementVector.Y * MovementSpeed);
	

	AddControllerYawInput(SmoothTurnRateDeg);

	float CurrentMeshPitch = GetMesh()->GetRelativeRotation().Pitch;
	
	DesiredPitch = FMath::FInterpTo(DesiredPitch, MovementVector.X, DT, RollReturnDegPerSec);
	FRotator NewRot(CurrentMeshPitch + DesiredPitch, MeshBaseRot.Yaw, MeshBaseRot.Roll );

	GetMesh()->SetRelativeRotation(NewRot);
	UE_LOG(LogTemp, Warning, TEXT("BirdController::MovementVector Y Input = %f"), MovementVector.Y);
	UE_LOG(LogTemp, Warning, TEXT("BirdController::MovementVector X Input = %f"), MovementVector.X)
	
}

void ABird::Look(const FInputActionValue& Value)
{
	FVector2D Look = Value.Get<FVector2D>();
	 const float DT = GetWorld()->GetDeltaSeconds();

	//Anlik input birikimli hedef acisi

	CamYaw += Look.X * CamSensitivity * DT;  
	CamYaw = FMath::Clamp(CamYaw,CamYawMin,CamYawMax);
	CamPitch += Look.Y * CamSensitivity * DT;
	CamPitch = FMath::Clamp(CamPitch, CamPitchMin, CamPitchMax);
	
	
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------

	
}

void ABird::Fly(const FInputActionValue& Value)
{
	float InputFly = Value.Get<float>();
	AddMovementInput(GetActorUpVector() , InputFly * 500);

}


// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Otomatik ilerleme
	AddMovementInput(GetActorForwardVector(),  100);
	//Otomatik mesh roll duzeltme
	
	CurrentPitch = BirdMeshComponent->GetRelativeRotation().Pitch;
	TargetMeshPitch  = FMath::FInterpTo(CurrentPitch, MeshBaseRot.Pitch, DeltaTime, 5);
	BirdMeshComponent->SetRelativeRotation(FRotator(TargetMeshPitch, MeshBaseRot.Yaw, MeshBaseRot.Roll));

	//Kamera bakis kontrolu
	
	// Hedef rotasyon look fonksiyonunda birikimli
	
	const FRotator Target(CamPitch, CamYaw, 0.f);

	// Mevcut -> hedefe interpolasyon
	const FRotator Current = SpringArm->GetRelativeRotation(); // veya GetRelativeRotation()
	const FRotator Smoothed = FMath::RInterpTo(Current, Target, DeltaTime, CamLagSpeed);
	SpringArm->SetRelativeRotation(Smoothed);

	
}

// Called to bind functionality to input

