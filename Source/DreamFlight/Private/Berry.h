// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EatableItems.h"
#include "Berry.generated.h"
class ARavenCharacter;
/**
 * 
 */
UCLASS()
class ABerry : public AEatableItems
{
	GENERATED_BODY()
	protected:
	virtual void Eat() override;
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult) override;
	
	virtual void EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override ;
	ARavenCharacter* RavenCharacter;
	
};
