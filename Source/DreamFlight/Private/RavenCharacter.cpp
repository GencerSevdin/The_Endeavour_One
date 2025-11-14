// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterEnumType.h"
#include "DrawDebugHelpers.h"
#include"Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
// Sets default values
ARavenCharacter::ARavenCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	// SPLINE COMPONENT OLUŞTUR
	LandingSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LandingSpline"));
	LandingSpline->SetupAttachment(RootComponent);
    
	// Development için görünür yap
#if WITH_EDITOR
	LandingSpline->SetDrawDebug(true);
	LandingSpline->SetUnselectedSplineSegmentColor(FLinearColor::Green);
	LandingSpline->SetSelectedSplineSegmentColor(FLinearColor::Yellow);
#endif
}

// Called when the game starts or when spawned
void ARavenCharacter::BeginPlay()
{
	Super::BeginPlay();
	BirdMeshComponent = GetMesh();
	MeshBaseRot = BirdMeshComponent->GetRelativeRotation();
	SpringArm = FindComponentByClass<USpringArmComponent>();
	Camera = FindComponentByClass<UCameraComponent>();
	RavenCharacterMovement = GetCharacterMovement();
	if (RavenCharacterMovement)
	{
		RavenCharacterMovement->MaxFlySpeed = SlowSpeed;
	}
}


void ARavenCharacter::SetFlyMode(bool bEnable)
{
	if (bEnable)
	{
		bIsLanding = false;
		
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (RavenCharacterMovement)
		RavenCharacterMovement ->SetMovementMode(MOVE_Flying);
		
		
	}
	else
	{
		bIsLanding = true;
		RavenCharacterMovement ->SetMovementMode(MOVE_Falling);
	}

	
}
void ARavenCharacter::Movement(const FInputActionValue& Value)
{
	FVector MovementVector = Value.Get<FVector>();
	const float DT = GetWorld()->GetDeltaSeconds();
	FVector ForwardVector  = GetActorForwardVector();
	FVector UpVector = GetActorUpVector();
	if (!bHasValidPearchPoint  )
	{
		AddMovementInput(ForwardVector,  MovementVector.Y);
		if (RavenCharacterMovement->IsFlying())
		{
			AddMovementInput(UpVector, MovementVector.Z);
			if (MovementVector.Z > 0.f && Speed > 500.f )
			{
				CharacterState = ECharacterState::Flapping;
				AddActorWorldOffset(FVector(0, 0,  TransformedSin()));
			
			
			}
			else if (MovementVector.Z == 0.f)
			{
				CharacterState = ECharacterState::Flapping;
				if (!bValueSpeedUp)
				{   					
					RavenCharacterMovement->MaxFlySpeed =  SlowSpeed;
				}
			}
			else if (MovementVector.Z < 0.f && Speed > 500.f)
			{
				CharacterState = ECharacterState::Gliding;
				RavenCharacterMovement->MaxFlySpeed =  MaxSpeed;
			
			}
			float CurrentMeshPitch = GetMesh()->GetRelativeRotation().Pitch;
			float CurrentMeshRoll = GetMesh()->GetRelativeRotation().Roll;
			DesiredRoll = FMath::FInterpConstantTo(DesiredRoll, MovementVector.Z * 2, DT, RollReturnDegPerSec);
			DesiredPitch = FMath::FInterpTo(DesiredPitch, MovementVector.X * 2, DT, RollReturnDegPerSec);
			FRotator NewRot = FRotator(CurrentMeshPitch + DesiredPitch, MeshBaseRot.Yaw, CurrentMeshRoll - DesiredRoll);
			if (Speed > 500)
				GetMesh()->SetRelativeRotation(NewRot);
	
		}
		AddControllerYawInput(MovementVector.X*DT*TurnRateDeg);
	}
}
void ARavenCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Look = Value.Get<FVector2D>();
	const float DT = GetWorld()->GetDeltaSeconds();

	//Anlik input birikimli hedef acisi

	CamYaw += Look.X * CamSensitivity * DT;  
	CamYaw = FMath::Clamp(CamYaw,CamYawMin,CamYawMax);
	CamPitch += Look.Y * CamSensitivity * DT;
	CamPitch = FMath::Clamp(CamPitch, CamPitchMin, CamPitchMax);
	
}
void ARavenCharacter::StartToSearch()
{
	
	if (CanPearch(PearchComponent,ComponentTransform))
	{
		
		CompLoc = ComponentTransform.GetLocation();
		PearchComp = PearchComponent;
		PearchTransform = ComponentTransform;
		bHasValidPearchPoint = true;
	}
}

void ARavenCharacter::StopToSearch()
{
	bHasValidPearchPoint = false;
}

void ARavenCharacter::Eat(const FInputActionValue& Value)
{
	float InputEat = Value.Get<float>();
	
	//UE_LOG(LogTemp, Warning, TEXT("BirdCharacter::Eat"));
	PlayAnimMontage(Eating);
}

void ARavenCharacter::Acceleration(bool bSpeedUp)
{
	bValueSpeedUp = bSpeedUp;
	if (bSpeedUp)
	{
		RavenCharacterMovement->MaxFlySpeed =  FastSpeed;
		
	}
	else
	{
		RavenCharacterMovement->MaxFlySpeed = SlowSpeed;
	}
}


void ARavenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	//Otomatik mesh roll duzeltme
	FVector VelocityOfBird =  RavenCharacterMovement->Velocity;
	Speed = VelocityOfBird.Size2D();
	if (Speed < 500)
	{
		CharacterState = ECharacterState::Hover;
	}
	UE_LOG(LogTemp, Display, TEXT("Speed: %f"), Speed);

	CurrentPitch = BirdMeshComponent->GetRelativeRotation().Pitch;
	CurrentRoll = BirdMeshComponent->GetRelativeRotation().Roll;
	TargetMeshPitch  = FMath::FInterpTo(CurrentPitch, MeshBaseRot.Pitch, DeltaTime, 5);
	TargetMeshRoll = FMath::FInterpTo(CurrentRoll, MeshBaseRot.Roll, DeltaTime, 5);
	BirdMeshComponent->SetRelativeRotation(FRotator(TargetMeshPitch, MeshBaseRot.Yaw, TargetMeshRoll));

	//Kamera bakis kontrolu
	
	// Hedef rotasyon look fonksiyonunda birikimli
	
	const FRotator Target(CamPitch, CamYaw, 0.f);

	// Mevcut -> hedefe interpolasyon
	const FRotator Current = SpringArm->GetRelativeRotation(); // veya GetRelativeRotation()
	const FRotator Smoothed = FMath::RInterpTo(Current, Target, DeltaTime, CamLagSpeed);
	SpringArm->SetRelativeRotation(Smoothed);
	//--------------------Dala konma sistemi -----------------------------------------------------------
	if (bIsLanding && bHasValidPearchPoint)
	{
		BirdCurrLoc= GetActorLocation();
		CharacterState = ECharacterState::Flapping;
		//FRotator BirdCurrRotation = GetActorRotation();
		
		float DistanceToTarget = FVector::Distance(BirdCurrLoc, CompLoc);
		UE_LOG(LogTemp, Display, TEXT("Distance: %f"), DistanceToTarget);
		DesiredLocation = FMath:: VInterpConstantTo(BirdCurrLoc, CompLoc, DeltaTime,
													PearchApproachSpeed);
		AController* RavenController = GetController();
		if (RavenController)
		RavenController->GetPlayerViewPoint(Location, Rotation);
		//FRotator DesiredRotation = FMath::RInterpTo(BirdCurrRotation, Rotation, DeltaTime, 100);
		if (DistanceToTarget < 1000)
		{
			SetActorLocation(DesiredLocation);
			//SetActorRotation(DesiredRotation);
			/*if (DistanceToTarget < 50)
			{
				bIsLanding = true;
				bHasValidPearchPoint = false;
				//SetActorRotation(PearchTransform.Rotator());
				AttachToComponent(PearchComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				RavenCharacterMovement->SetMovementMode(MOVE_Falling);
			}*/
		}
		
	
	}
	
	//---------------------Calculate Distance To Floor -------------------------------------------
	
	CalcDistanceToFloor(Floor);
	DistanceToFloor = Floor.Distance;
	UE_LOG(LogTemp, Display, TEXT("distance: %f"), DistanceToFloor);
	//-----------Setting Field Of View Relative To Speed -------------------------------------
	const float TargetFOV = FMath::GetMappedRangeValueClamped(FVector2D(SlowSpeed, FastSpeed),
				FVector2D(BaseFOV, MinFOV), Speed);
	
	// Yumuşat ve uygula
	const float CurrentFOV = Camera->FieldOfView;
	const float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
	Camera->SetFieldOfView(NewFOV);

	// tünelleşme: hızlandıkça vignette artsın
	Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
	const float TargetVignette = FMath::GetMappedRangeValueClamped(
		FVector2D(SlowSpeed, FastSpeed),
		FVector2D(MinVignette, MaxVignette),
		Speed
	);
	Camera->PostProcessSettings.VignetteIntensity = FMath::FInterpTo(
		Camera->PostProcessSettings.VignetteIntensity, TargetVignette, DeltaTime, FOVInterpSpeed);
	//---------------------------------Chromatic Aberration-----------------------------------------------
	/**/
	//-------------------Hiza bagli kamera blur -------------------------------
	const float TargetBlur = FMath::GetMappedRangeValueClamped(FVector2D(SlowSpeed, FastSpeed),
												FVector2D(0, 1.5f), Speed);
	Camera->PostProcessSettings.MotionBlurMax = TargetBlur;

	

}

void ARavenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

bool ARavenCharacter::CanPearch(USceneComponent*& OutComponent, FTransform& OutComponentTransform)
{
	
		FVector LineStart = Camera->GetComponentLocation();
		FVector LineEnd = LineStart + Camera->GetForwardVector() * 2000.f;
	
		FHitResult HitResult;
		FCollisionQueryParams TraceParam;
		TraceParam.AddIgnoredActor(this);
		DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red);

		const bool HasHit = GetWorld()-> SweepSingleByChannel(HitResult, LineStart, LineEnd, FQuat::Identity,
													ECC_GameTraceChannel1, Sphere, TraceParam);
		AActor* HitActor = HitResult.GetActor();
		if (HasHit)
		{
			HitActor->GetComponents<USceneComponent>(SceneComponents);
			for (USceneComponent* Comp : SceneComponents)
			{
				if (Comp && Comp->GetName()== "PearchablePoint")
				{
					FString CompName = Comp->GetName();
 					OutComponent = Comp;
					//OutComponentLoc = Comp->GetComponentLocation();
					OutComponentTransform = Comp->GetComponentTransform();
					return true;
				}
			}
		}
	
	return false;
}

/*bool ARavenCharacter::CanPearch()
{
	FHitResult HitResult;
	const bool HasPearchableActor = GetPearchPoint(HitResult);
	if (!HasPearchableActor)
	return false;
	AActor* HitActor = HitResult.GetActor();
	
	if (!HitActor) return false;
	HitActor->GetComponents<USceneComponent>(SceneComponents);
	for ( USceneComponent* Comp : SceneComponents)
	{
		if (Comp && Comp->GetName() == "PearchablePoint")
		{
			FString CompName = Comp->GetName();
			UE_LOG(LogTemp, Display, TEXT("CompName: %s"), *CompName);
			ComponentLocation = Comp->GetComponentLocation();
			return true;
		
		}
	}
	return false; 
}
*/
bool ARavenCharacter::CalcDistanceToFloor(FHitResult& OutDistance)
{
	FVector LineStart = GetActorLocation() - FVector(0, 0, 20);
	FVector LineEnd = LineStart - FVector(0,0,5000);
	FCollisionQueryParams TraceParam;
	TraceParam.AddIgnoredActor(this);
	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 1);

	bool HasHit = GetWorld()->SweepSingleByChannel(OutDistance, LineStart, LineEnd, FQuat::Identity,
													ECC_GameTraceChannel1, Sphere, TraceParam);
	return HasHit;
}

float ARavenCharacter::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);

}

// ===== SPLINE LANDING SYSTEM IMPLEMENTATION =====








//float InputValue = Value.Get<float>();
	
/*float TargetCA = FMath::GetMappedRangeValueClamped(
FVector2D(SlowSpeed, FastSpeed),
FVector2D(0.f, MaxCAIntensity),
Speed);
Camera->PostProcessSettings.SceneFringeIntensity = MaxCAIntensity;*//*FMath::FInterpTo(
	Camera->PostProcessSettings.SceneFringeIntensity,
	MaxVignette,
	GetWorld()->GetDeltaSeconds(),
	CAInterpSpeed
);*/                   