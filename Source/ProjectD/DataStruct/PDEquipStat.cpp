// Fill out your copyright notice in the Description page of Project Settings.


#include "PDEquipStat.h"
#include "../PDGameInstance.h"
#include "Kismet/GameplayStatics.h"

FEquipStat::FEquipStat()
{
	Index = 0;
	MaxHp = 0.0f;
	Atk = 0.0f;
	Def = 0.0f;
	Speed = 0.0f;
	Jump = 0.0f;
	AtkRange = 0.0f;
	AtkSpeed = 0.0f;
}

UPDEquipStat::UPDEquipStat()
{
	Stat = FEquipStat();
}


void FEquipStat::SetStat(int32 itemCode, UWorld* world)
{
	UPDGameInstance* PDGameInstance = Cast<UPDGameInstance>(UGameplayStatics::GetGameInstance(world));
	if (PDGameInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Error : PDEquipSTat-SetSTat : PDGameInstance is nullptr"));
		return;
	}
	Index = itemCode;
	MaxHp = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "MaxHp"));
	Atk = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "Atk"));
	Speed = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "Speed"));
	Jump = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "Jump"));
	AtkRange = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "AtkRange"));
	AtkSpeed = FCString::Atof(*PDGameInstance->GetEquipRowData(itemCode, "AtkSpeed"));
}

void FEquipStat::ResetStat()
{
	Index = 0;
	MaxHp = 0.0f;
	Atk = 0.0f;
	Def = 0.0f;
	Speed = 0.0f;
	Jump = 0.0f;
	AtkRange = 0.0f;
	AtkSpeed = 0.0f;
}
