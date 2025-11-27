// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HungryComponent.generated.h"

class UCharacterMovementComponent;
class ARavenCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DREAMFLIGHT_API UHungryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHungryComponent();
	float RecoverHunger(float feed);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(BluePrintReadOnly)
	float Hunger = 100;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	float CalculateHunger();
	
	UCharacterMovementComponent*  RavenMovementComp ;
	ARavenCharacter* RavenCharacter;
	float MinSpeed;
	float MaxSpeed;
	float MinHunger = 0;
	float MaxHunger= 100;
	
};
