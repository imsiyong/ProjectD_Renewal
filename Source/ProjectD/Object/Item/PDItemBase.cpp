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
	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMeshComponent;
	ItemCode = -1;
	Texture = nullptr;
	Name = "";
	ItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	ItemBox->SetCollisionProfileName(TEXT("Item"));
	ItemBox->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	ItemBox->SetupAttachment(RootComponent);
	Tags.Add(FName("Item"));
}

// Called when the game starts or when spawned
void APDItemBase::BeginPlay()
{
	Super::BeginPlay();
	PDGameInstance = Cast<UPDGameInstance>(GetGameInstance());
	ItemBox->OnComponentBeginOverlap.AddDynamic(this, &APDItemBase::BeginOverlap);
}

// Called every frame
void APDItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APDItemBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Begin Overlap"));
}

void APDItemBase::Init(int32 index)
{
	if (PDGameInstance == nullptr)
	{
		PDGameInstance = Cast<UPDGameInstance>(GetGameInstance());
	}
	ItemCode = index;
	
	FString TexturePath = PDGameInstance->GetEquipRowData(index, "Texture");
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));

	FString MeshPath = PDGameInstance->GetEquipRowData(index, "Mesh");
	UStaticMesh* NewMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (NewMesh)
	{
		ItemMeshComponent->SetStaticMesh(NewMesh);
		ItemMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}

	Name = PDGameInstance->GetEquipRowData(index, "Name");
}

