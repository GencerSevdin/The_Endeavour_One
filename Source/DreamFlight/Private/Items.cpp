// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"

#include "RavenCharacter.h"
#include "Components/SphereComponent.h"
// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	RootComponent = ItemMesh;
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItems::EndSphereOverlap);
}

void AItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("berry1 begin"));
	ARavenCharacter* RavenCharacter = Cast<ARavenCharacter>(OtherActor);
	if (RavenCharacter)
	{
		RavenCharacter->SetOverlappingItem(this);
	}
	
}

void AItems::EndSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("berry1 ended"));
	ARavenCharacter* RavenCharacter = Cast<ARavenCharacter>(OtherActor);
	if (RavenCharacter)
	{
		RavenCharacter->SetOverlappingItem(nullptr);
	}
	

}


// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

