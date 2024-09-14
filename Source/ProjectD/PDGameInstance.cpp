// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameInstance.h"
#include "ItemData.h"
#include "Manager/PDSpawnManager.h"
#include "DataStruct/PDSpawnPoint.h"
#include "PDCharacterItemInventory.h"
#include "PDCharacterEquip.h"
#include "DataStruct/PDCharacterStat.h"
#include "DataStruct/PDItemDataTable.h"
#include "DataStruct/PDBagData.h"
#include "DataStruct/PDEquipData.h"
#include "DataStruct/PDStorageData.h"

UPDGameInstance::UPDGameInstance()
{
	PlayerCode = 0;
	MonsterCode = 0;
	static ConstructorHelpers::FObjectFinder<UDataTable> IT(TEXT("DataTable'/Game/CsvData/ItemData.ItemData'"));
	if (IT.Succeeded())
	{
		ItemData = IT.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ET(TEXT("DataTable'/Game/CsvData/EquipData.EquipData'"));
	if (ET.Succeeded())
	{
		EquipmentData = ET.Object;
	}
}

void UPDGameInstance::PlayerStatArrayAdd()
{
	class FStat* ref = new FStat(PlayerCode++);
	PlayerStatArray.Add(ref);
}

void UPDGameInstance::MonsterStatArrayAdd()
{
	class FStat* ref = new FStat(MonsterCode++);
	MonsterStatArray.Add(ref);
}

FString UPDGameInstance::GetEquipRowData(int32 index, FString rowName)
{
	if (EquipmentData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error : GameInstance - GetEquipRowData : EquipData is nullptr"));
		return FString();
	}
	FEquipData* rowData = EquipmentData->FindRow<FEquipData>(*FString::FromInt(index), TEXT(""));
	if (rowData)
	{
		if (rowName == "Name")
		{
			return rowData->Name;
		}
		else if (rowName == "Mesh")
		{
			return rowData->Mesh;
		}
		else if (rowName == "Texture")
		{
			return rowData->Texture;
		}
		else if (rowName == "Atk")
		{
			return FString::SanitizeFloat(rowData->Atk);
		}
		else if (rowName == "AtkSpeed")
		{
			return FString::SanitizeFloat(rowData->AtkSpeed);
		}
		else if (rowName == "AtkRange")
		{
			return FString::SanitizeFloat(rowData->AtkRange);
		}
		else if (rowName == "Speed")
		{
			return FString::SanitizeFloat(rowData->Speed);
		}
		else if (rowName == "Jump")
		{
			return FString::SanitizeFloat(rowData->Jump);
		}
		else if (rowName == "MaxHp")
		{
			return FString::SanitizeFloat(rowData->MaxHp);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error : GameInstance - GetEquipRowData : rowData has not valid value"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error : GameInstance - GetEquipRowData : rowData is nullptr"));
	}
	return FString();
}

UPDCharacterStat* UPDGameInstance::GetPlayerStat()
{
	if (PlayerStat == nullptr)
	{
		PlayerStat = NewObject<UPDCharacterStat>(this, UPDCharacterStat::StaticClass());
		PlayerStat->Stat.Index = 0;
	}
	return PlayerStat;
}

UPDBagData* UPDGameInstance::GetBagData()
{
	if (BagData == nullptr)
		BagData = NewObject<UPDBagData>(this, UPDBagData::StaticClass());
	return BagData;
}

UPDEquipData* UPDGameInstance::GetEquipData()
{
	if (EquipData == nullptr)
		EquipData = NewObject<UPDEquipData>(this, UPDEquipData::StaticClass());
	return EquipData;
}

UPDStorageData* UPDGameInstance::GetStorageData()
{
	if (StorageData == nullptr)
		StorageData = NewObject<UPDStorageData>(this, UPDStorageData::StaticClass());
	return StorageData;
}

UPDCharacterItemInventory* UPDGameInstance::GetPlayerInventory()
{
	if (Inventory == nullptr)
		Inventory = NewObject<UPDCharacterItemInventory>(this, UPDCharacterItemInventory::StaticClass());
	return Inventory;
}

UPDCharacterEquip* UPDGameInstance::GetPlayerEquip()
{
	if (Equip == nullptr)
		Equip = NewObject<UPDCharacterEquip>(this, UPDCharacterEquip::StaticClass());
	return Equip;
}
