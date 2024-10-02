// Fill out your copyright notice in the Description page of Project Settings.


#include "PDEquipData.h"

FEquipSlotData::FEquipSlotData()
{
	Index = 0;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

FEquipSlotData::FEquipSlotData(int32 index)
{
	Index = index;
	Count = 0;
	ItemCode = -1;
	Name = "";
	Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	EquipType = EEquipType::None;
	InventoryType = EInventoryType::None;
}

UPDEquipData::UPDEquipData()
{
	DefaultTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/emptytexture.emptytexture")));
	MaxCount = 5;
	EquipData.Empty();
	for (int i = 0; i < MaxCount; i++)
	{
		EquipData.Emplace(FEquipSlotData(i));
	}
}

void UPDEquipData::AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype)
{
	int32 index = 0;
	switch (equiptype)
	{
	case EEquipType::Head:
		index = 0;
		break;
	case EEquipType::Body:
		index = 2;
		break;
	case EEquipType::Foot:
		index = 4;
		break;
	case EEquipType::Left:
		index = 1;
		break;
	case EEquipType::Right:
		index = 3;
		break;
	case EEquipType::None:
		break;
	default:
		break;
	}
	if (EquipData[index].ItemCode != -1)
	{
		UE_LOG(LogTemp, Log, TEXT("Already Have Equipment : PDEQuipData.cpp : AddItem()"));
		return;
	}
	EquipData[index].Name = name;
	EquipData[index].ItemCode = itemCode;
	EquipData[index].Texture = texture;
	EquipData[index].InventoryType = type;
	EquipData[index].EquipType = equiptype;
}

void UPDEquipData::RemoveItemByIndex(int32 index)
{
	if (index < 0 || index>MaxCount)return;
	if (EquipData[index].InventoryType != EInventoryType::None)
	{
		EquipData[index].Name = FString(TEXT(""));
		EquipData[index].ItemCode = -1;
		EquipData[index].Texture = DefaultTexture;
		EquipData[index].InventoryType = EInventoryType::None;
		EquipData[index].EquipType = EEquipType::None;
	}
}
