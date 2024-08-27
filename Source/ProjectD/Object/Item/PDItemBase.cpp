// Fill out your copyright notice in the Description page of Project Settings.


#include "PDItemBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Character/PDCharacterBase.h"
#include "../../PDGameInstance.h"
// Sets default values
APDItemBase::APDItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemCode = -1;
	Texture = nullptr;
	Name = "";
	ItemMeshComponent = nullptr;
	ItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	ItemBox->SetCollisionProfileName("Item");
	ItemBox->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	ItemBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APDItemBase::BeginPlay()
{
	Super::BeginPlay();
	ItemBox->OnComponentBeginOverlap.AddDynamic(this, &APDItemBase::BeginOverlap);
}

// Called every frame
void APDItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDItemBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void APDItemBase::Init(int32 index)
{
}

