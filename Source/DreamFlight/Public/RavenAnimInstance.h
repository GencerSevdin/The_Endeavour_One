// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterEnumType.h"
#include "RavenAnimInstance.generated.h"


//class ARavenPlayerController;

UCLASS()
class DREAMFLIGHT_API URavenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ARavenCharacter* RavenCharacter;
	UPROPERTY(BlueprintReadOnly)
	class UCharacterMovementComponent* RavenCharacterMovement;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;
	UFUNCTION()
	void AnimNotify_StartJump();
	UFUNCTION()
	void AnimNotify_Land();
};
