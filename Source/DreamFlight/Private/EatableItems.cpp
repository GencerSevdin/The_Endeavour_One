// Fill out your copyright notice in the Description page of Project Settings.


#include "EatableItems.h"
#include "RavenCharacter.h"

void AEatableItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Log, TEXT("berry2 begin"));

}

void AEatableItems::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Log, TEXT("berry2 ended"));

	
}

void AEatableItems::Eat()
{
	UE_LOG(LogTemp, Warning, TEXT("yediler beni"));
	Destroy();
}
