// Fill out your copyright notice in the Description page of Project Settings.


#include "Berry.h"
#include "RavenCharacter.h"
#include "HungryComponent.h"

void ABerry::Eat()
{
	Super::Eat();
	UE_LOG(LogTemp, Warning, TEXT("Berry yedi de ne oldu"));
	UHungryComponent* HungryComponent= Cast<UHungryComponent>( RavenCharacter->FindComponentByTag<UActorComponent>("Hungry"));
	HungryComponent->RecoverHunger(10);
	
}

void ABerry::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	RavenCharacter = Cast<ARavenCharacter>(OtherActor);

}

void ABerry::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
