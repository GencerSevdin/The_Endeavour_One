// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "RavenPlayerController.generated.h"



class UInputAction;
class UInputMappingContext;
class ABird;
/**
 * 
 */
UCLASS()
class DREAMFLIGHT_API ARavenPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* BirdMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TurnAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* UpDownAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EatAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AccelerationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SearchPointAction;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	//UInputAction* Transform();
	void SetupInputComponent() override ;
	
	
private:
	void Move(const FInputActionValue &Value);
	
	void Look(const FInputActionValue &Value);
	void Eat(const FInputActionValue &Value);
	void FlyingStart(const FInputActionValue &Value);
	void Flying(const FInputActionValue &Value);
	void FlyingEnd(const FInputActionValue &Value);
	void Acceleration(const FInputActionValue &Value);
	void SearchPearchablePoints(const FInputActionValue &Value);
	void StopSearchPearchablePoints(const FInputActionValue &Value);
	bool bAltMode;
	bool bChangeSpeed;
	
};
