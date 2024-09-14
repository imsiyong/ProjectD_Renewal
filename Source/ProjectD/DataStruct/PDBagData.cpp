// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBagData.h"

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

void UPDBagData::AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	int32 index = 0;
	while (1)
	{
		if (index >= MaxCount)return;
		if (BagData[index].InventoryType == EInventoryType::None)break;
		index++;
	}
	BagData[index].Name = name;
	BagData[index].ItemCode = itemCode;
	BagData[index].Texture = texture;
	BagData[index].InventoryType = type;
	BagData[index].EquipType = equiptype;
}

void UPDBagData::AddItemByIndex(int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	if (index<0 || index >=MaxCount)return;
	BagData[index].Name = name;
	BagData[index].ItemCode = itemCode;
	BagData[index].Texture = texture;
	BagData[index].InventoryType = type;
	BagData[index].EquipType = equiptype;
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
