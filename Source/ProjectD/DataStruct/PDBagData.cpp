// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBagData.h"
#include "../PDGameInstance.h"
#include "Kismet/GameplayStatics.h"

FBagData::FBagData()
{
	Index = 0;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

FBagData::FBagData(int32 index)
{
	Index = index;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

UPDBagData::UPDBagData()
{
	DefaultTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	MaxCount = 5;
	BagData.Empty();
	for (int i = 0; i < MaxCount; i++)
	{
		BagData.Emplace(FBagData(i));
	}
}

int32 UPDBagData::AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	int32 index = 0;
	while (1)
	{
		if (index >= MaxCount)return -1;
		if (BagData[index].InventoryType == EInventoryType::None)break;
		index++;
	}
	BagData[index].Name = name;
	BagData[index].ItemCode = itemCode;
	BagData[index].Texture = texture;
	BagData[index].InventoryType = type;
	BagData[index].EquipType = equiptype;
	return 0;
}

int32 UPDBagData::AddItemByIndex(int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	if (index<0 || index >=MaxCount)return -1;
	BagData[index].Name = name;
	BagData[index].ItemCode = itemCode;
	BagData[index].Texture = texture;
	BagData[index].InventoryType = type;
	BagData[index].EquipType = equiptype;
	return 0;
}

void UPDBagData::AddItemByItemcode(int32 itemCode)
{
	if (PDGameInstance == nullptr)
	{
		PDGameInstance = Cast<UPDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	FString Name = PDGameInstance->GetEquipRowData(itemCode, "Name");
	FString TexturePath = PDGameInstance->GetEquipRowData(itemCode, "Texture");
	UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *TexturePath));
	
	//EInventoryType and EEquipType have to check after add another item
	//Fix Weapon, and Right -> change other type more flexible
	AddItem(Name, itemCode, texture, EInventoryType::Weapon, EEquipType::Right);
}

void UPDBagData::RemoveItemByIndex(int32 index)
{
	if (index<0 || index >MaxCount)return;
	if (BagData[index].InventoryType != EInventoryType::None)
	{
		BagData[index].Name = FString(TEXT(""));
		BagData[index].ItemCode = -1;
		BagData[index].Texture = DefaultTexture;
		BagData[index].InventoryType = EInventoryType::None;
		BagData[index].EquipType = EEquipType::None;
	}
}

void UPDBagData::SwapItemByIndex(int32 indexA, int32 indexB)
{
	FBagData ref = FBagData(0);
	ref = BagData[indexA];
	BagData[indexA] = BagData[indexB];
	BagData[indexB] = ref;
}
