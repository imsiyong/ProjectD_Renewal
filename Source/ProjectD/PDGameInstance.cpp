// Fill out your copyright notice in the Description page of Project Settings.


#include "PDGameInstance.h"
#include "ItemData.h"
#include "Manager/PDSpawnManager.h"
#include "DataStruct/PDSpawnPoint.h"
#include "PDCharacterItemInventory.h"
#include "PDCharacterEquip.h"
#include "DataStruct/PDCharacterStat.h"

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
		EquipData = ET.Object;
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

UPDCharacterStat* UPDGameInstance::GetPlayerStat()
{
	if (PlayerStat == nullptr)
	{
		PlayerStat = NewObject<UPDCharacterStat>(this, UPDCharacterStat::StaticClass());
		PlayerStat->Stat.Index = 0;
	}
	return PlayerStat;
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
