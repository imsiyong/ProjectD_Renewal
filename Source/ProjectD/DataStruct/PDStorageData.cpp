// Fill out your copyright notice in the Description page of Project Settings.


#include "PDStorageData.h"

FStorageData::FStorageData()
{
	Index = 0;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

FStorageData::FStorageData(int32 index)
{
	Index = index;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

UPDStorageData::UPDStorageData()
{
	DefaultTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	MaxCount = 12;
	StorageData.Empty();
	for (int i = 0; i < MaxCount; i++)
	{
		StorageData.Emplace(FStorageData(i));
	}
}

int UPDStorageData::AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	int32 index = 0;
	while (1)
	{
		if (index >= MaxCount)
		{
			return -1;
		}
		if (StorageData[index].InventoryType == EInventoryType::None)
		{
			break;
		}
		index++;
	}
	StorageData[index].Name = name;
	StorageData[index].ItemCode = itemCode;
	StorageData[index].Texture = texture;
	StorageData[index].InventoryType = type;
	StorageData[index].EquipType = equiptype;
	return 0;
}

int32 UPDStorageData::AddItemByIndex(int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	if (index < 0 || index >= MaxCount)
	{
		return -1;
	}
	StorageData[index].Name = name;
	StorageData[index].ItemCode = itemCode;
	StorageData[index].Texture = texture;
	StorageData[index].InventoryType = type;
	StorageData[index].EquipType = equiptype;
	return 0;
}

void UPDStorageData::RemoveItemByIndex(int32 index)
{
	if (index<0 || index >MaxCount)
	{
		return;
	}
	if (StorageData[index].InventoryType != EInventoryType::None)
	{
		StorageData[index].Name = FString(TEXT(""));
		StorageData[index].ItemCode = -1;
		StorageData[index].Texture = DefaultTexture;
		StorageData[index].InventoryType = EInventoryType::None;
		StorageData[index].EquipType = EEquipType::None;
	}
}

void UPDStorageData::SwapItemByIndex(int32 indexA, int32 indexB)
{
}
