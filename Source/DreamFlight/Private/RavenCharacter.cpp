// Fill out your copyright notice in the Description page of Project Settings.


#include "RavenCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterEnumType.h"
#include "DrawDebugHelpers.h"
#include"Camera/CameraComponent.h"
#include "Components/SplineComponent.h"
#include "Animation/AnimMontage.h"
// Sets default values
ARavenCharacter::ARavenCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
	// SPLINE COMPONENT OLUŞTUR
	LandingSpline = CreateDefaultSubobject<USplineComponent>(TEXT("LandingSpline"));
	LandingSpline->SetupAttachment(RootComponent);
    
	// Development için görünür yap

	LandingSpline->SetDrawDebug(true);
	LandingSpline->SetUnselectedSplineSegmentColor(FLinearColor::Green);
	LandingSpline->SetSelectedSplineSegmentColor(FLinearColor::Yellow);

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
		if (bHasValidPearchPoint)
		{
			BuildLandingSpline();
			UE_LOG(LogTemp, Warning, TEXT("Landing spline created"));
		}
		
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
			if(MovementVector.Z == 0.f && Speed > 500.f)
			{
				CharacterState = ECharacterState::Flapping;
				if (!bValueSpeedUp)
				{   					
					RavenCharacterMovement->MaxFlySpeed =  SlowSpeed;
				}
			}
			 if (MovementVector.Z < 0.f && Speed > 500.f)
			{
				CharacterState = ECharacterState::Gliding;
				RavenCharacterMovement->MaxFlySpeed =  MaxSpeed;
			
			}
			if (Speed < 500.f)
			{
				CharacterState = ECharacterState::Hover;
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
	
	if (CanPearch(PearchComponent))
	{
		PearchComp = PearchComponent;
		CompLoc = PearchComp->GetComponentLocation();
		bHasValidPearchPoint = true;
	}
	
}

void ARavenCharacter::StopToSearch()
{
	bHasValidPearchPoint = false;
}

void ARavenCharacter::Eat(const FInputActionValue& Value)
{
	
	
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && EatMontage)
		{
			AnimInstance->Montage_Play(EatMontage);
		}
		UE_LOG(LogTemp, Warning, TEXT("BirdCharacter::Eat"));
	
	
	
}

void ARavenCharacter::Acceleration(bool bSpeedUp)
{
	bValueSpeedUp = bSpeedUp;
	if (bSpeedUp)
	{
		RavenCharacterMovement->MaxFlySpeed =  FastSpeed;
		//-------------------Hiza bagli kamera blur -------------------------------
		const float TargetBlur = FMath::GetMappedRangeValueClamped(FVector2D(SlowSpeed, FastSpeed),
												FVector2D(0, 1.5f), Speed);
		Camera->PostProcessSettings.MotionBlurMax = TargetBlur;
		
	}
	else
	{
		RavenCharacterMovement->MaxFlySpeed = SlowSpeed;
	}
}


void ARavenCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	static int FrameCounter = 0;
	FrameCounter++;
	/*FVector Mid = ComputeStageMid(400.f, 80.f);
	DrawDebugSphere(GetWorld(), Mid, 20.f, 12, FColor::Yellow, false, 1.f);
	DrawDebugLine(GetWorld(), GetActorLocation(), Mid, FColor::Cyan, false, 1.f, 0, 1.f);*/
	RunningTime += DeltaTime;
	//Otomatik mesh roll duzeltme 
	FVector VelocityOfBird =  RavenCharacterMovement->Velocity;
	Speed = VelocityOfBird.Size2D();

	UE_LOG(LogTemp, Display, TEXT("Speed: %f"), Speed);
	
	CurrentPitch = BirdMeshComponent->GetRelativeRotation().Pitch;
	CurrentRoll = BirdMeshComponent->GetRelativeRotation().Roll;
	TargetMeshPitch  = FMath::FInterpTo(CurrentPitch, MeshBaseRot.Pitch, DeltaTime, 5);
	TargetMeshRoll = FMath::FInterpTo(CurrentRoll, MeshBaseRot.Roll, DeltaTime, 5);
	BirdMeshComponent->SetRelativeRotation(FRotator(TargetMeshPitch, MeshBaseRot.Yaw, TargetMeshRoll));

	//Controlling Camera View
	
	
	const FRotator Target(CamPitch, CamYaw, 0.f);

	
	const FRotator Current = SpringArm->GetRelativeRotation(); // veya GetRelativeRotation()
	const FRotator Smoothed = FMath::RInterpTo(Current, Target, DeltaTime, CamLagSpeed);
	SpringArm->SetRelativeRotation(Smoothed);
	//--------------------Dala konma sistemi -----------------------------------------------------------
	if (bIsLanding && bHasValidPearchPoint)
	{
		if (bHasLandingSpline && LandingSpline)
		{
			const float Splinelength = LandingSpline->GetSplineLength();
			LandingSplineDistance += LandingSplineSpeed * DeltaTime;
			LandingSplineDistance = FMath::Clamp(LandingSplineDistance,0.f,Splinelength);
			//--------------
			const FVector TargetLocation = LandingSpline->GetLocationAtDistanceAlongSpline(LandingSplineDistance,ESplineCoordinateSpace::World);
			const FRotator SplineRotation = LandingSpline->GetRotationAtDistanceAlongSpline(LandingSplineDistance, ESplineCoordinateSpace::World);
			SetActorLocation(TargetLocation);
			
			SetActorRotation(SplineRotation);
		
			UE_LOG(LogTemp, Display, TEXT("Landing along spline: Dist=%.1f"),
			LandingSplineDistance);
			const float RemaningDist = Splinelength - LandingSplineDistance;

			if (RemaningDist <= 10.f)
			{
				if (PearchComp)	AttachToComponent(PearchComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				
				if (RavenCharacterMovement) RavenCharacterMovement->SetMovementMode(MOVE_Falling);
				//Clean States
				bIsLanding = false;
				bHasValidPearchPoint = false;
				bHasLandingSpline = false;
				LandingSplineDistance  =0.f;
				PearchComp = nullptr;
				CompLoc = FVector::ZeroVector;
			}

		}
	}
	//---------------------Calculate Distance To Floor -------------------------------------------
	if (FrameCounter % 10 == 0)
	{
		CalcDistanceToFloor(Floor);
	
	}
	DistanceToFloor = Floor.Distance;
	UE_LOG(LogTemp, Warning, TEXT("distance: %f"), DistanceToFloor);
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
	
	
	

	

}

void ARavenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

bool ARavenCharacter::CanPearch(USceneComponent*& OutComponent)
{
	FVector LineStart = Camera->GetComponentLocation();
	FVector LineEnd = LineStart + Camera->GetForwardVector() * 2000.f;
	
	FHitResult HitResult;
	FCollisionQueryParams TraceParam;
	TraceParam.AddIgnoredActor(this);
	//DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red);
	TArray<USceneComponent*>SceneComponents;
	const bool HasHit = GetWorld()-> SweepSingleByChannel(HitResult, LineStart, LineEnd, FQuat::Identity,
												ECC_GameTraceChannel1, Sphere, TraceParam);
	AActor* HitActor = HitResult.GetActor();
	if (HasHit)
	{
		HitActor->GetComponents<USceneComponent>(SceneComponents);
		for (USceneComponent* Comp : SceneComponents)
		{
			if (Comp)
			{
				if(Comp->ComponentHasTag("Perchable"))
				{
					OutComponent = Comp;
					
					return true;
				}
			}
		}
	
	}

	return false;
}

bool ARavenCharacter::CalcDistanceToFloor(FHitResult& OutDistance)
{
	FVector LineStart = GetActorLocation() - FVector(0, 0, 20);
	FVector LineEnd = LineStart - FVector(0,0,5000);
	FCollisionQueryParams TraceParam;
	TraceParam.AddIgnoredActor(this);
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 0.5f);

	bool HasHit = GetWorld()->SweepSingleByChannel(OutDistance, LineStart, LineEnd, FQuat::Identity,
													ECC_GameTraceChannel1, Sphere, TraceParam);
	return HasHit;
}

float ARavenCharacter::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);

}
void ARavenCharacter::BuildLandingSpline()
{
	if (!PearchComp || !LandingSpline) return;
	const FVector Start = GetActorLocation();
	const FVector End = PearchComp->GetComponentLocation();
	const float MidDistance = 100.f;
	const float SideOffset = 80.f;

	LandingMid = ComputeStageMid(MidDistance, SideOffset);

	//Clean Spline Points
	LandingSpline->ClearSplinePoints();
	//Add three points to world space
	LandingSpline->AddSplinePoint(Start, ESplineCoordinateSpace::World);
	LandingSpline->AddSplinePoint(LandingMid, ESplineCoordinateSpace::World);
	LandingSpline->AddSplinePoint(End, ESplineCoordinateSpace::World);
	LandingSpline->SetClosedLoop(false);
	LandingSpline->UpdateSpline();
	LandingSpline->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	//State Flags
	bHasLandingSpline = true;
	LandingSplineDistance = 0.f;
	UE_LOG(LogTemp, Warning, TEXT("BuildLandingSpline: Start=%s, Mid=%s, End=%s"),*Start.ToString(), *LandingMid.ToString(), *End.ToString());
}


FVector ARavenCharacter::ComputeStageMid(float Distance, float SideOffset)
{
	const FVector BirdPos = GetActorLocation();
	const FVector BirdForward = GetActorForwardVector();
	const FVector BirdRight = GetActorRightVector();
	const FVector BirdUp = GetActorUpVector();
	
	FVector BaseMid = BirdPos + BirdForward * Distance;
	FVector MidPoint0 = BaseMid;
	FVector MidPoint1 = MidPoint0 + BirdRight * SideOffset;
	FVector MidPoint2  = MidPoint0 - BirdRight * SideOffset;
	FVector MidPoint3  = MidPoint0 +BirdUp * SideOffset;
	FVector MidPoint4  = MidPoint0 - BirdUp * SideOffset;
	if (IsPathClear(BirdPos, MidPoint0))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: Using Mid0 (forward)"));
		return MidPoint0;
	}
	
	if (IsPathClear(BirdPos, MidPoint1))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: Using Mid1 (right)"));
		return MidPoint1;
	}
	if (IsPathClear(BirdPos, MidPoint2))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: Using Mid2 (left)"));
		return MidPoint2;
	}
	if (IsPathClear(BirdPos, MidPoint3))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: Using Mid3 (Up)"));
		return MidPoint3;
	}
	
	if (IsPathClear(BirdPos, MidPoint4))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: Using Mid4 (down)"));
		return MidPoint4;
	}
		
	UE_LOG(LogTemp, Warning, TEXT("ComputeStageMid: fallback Mid0"));
	return MidPoint0;
}

bool ARavenCharacter::IsPathClear(const FVector& Start, const FVector& End)
{
	FCollisionQueryParams TraceParam;
	
	TraceParam.AddIgnoredActor(this);
	FHitResult HitResult;
	
	bool bIsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, TraceParam);
	if (bIsHit)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("ARavenCharacter::IsPathClear %s"), *HitActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IsPathClear Hit but no actor"));
		}
		return false;
	}
	
		UE_LOG(LogTemp, Warning, TEXT("IsPathClear NO HIT"));
		return  true;

}
