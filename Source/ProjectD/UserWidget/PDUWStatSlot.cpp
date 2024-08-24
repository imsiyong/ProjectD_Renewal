// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWStatSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../DataStruct/PDCharacterStat.h"
#include "../PDGameInstance.h"

void UPDUWStatSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (UGameInstance* GameInstace = GetGameInstance())
	{
		PDGameInstance = Cast<UPDGameInstance>(GameInstace);
	}
	BT_Plus = Cast<UButton>(GetWidgetFromName(TEXT("BT_Plus")));
	if (BT_Plus)
	{
		BT_Plus->OnClicked.AddDynamic(this, &UPDUWStatSlot::OnButtonClickedPlus);
	}
	TB_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Name")));
	TB_Value = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Value")));
	TB_Point = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Point")));
	IMG_Icon = Cast<UImage>(GetWidgetFromName(TEXT("IMG_Icon")));
	Init();
}

void UPDUWStatSlot::Init()
{
	Refresh();
}

void UPDUWStatSlot::Refresh()
{
	if (PDGameInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Error : PDUWStatSlot[Refresh()] GameInstance -> nullptr"));
		return;
	}
	UPDCharacterStat* Temp = PDGameInstance->GetPlayerStat();
	switch (CharacterStatType)
	{
	case ECharacterStatType::None:
		break;
	case ECharacterStatType::Atk:
		TB_Name->SetText(FText::FromString("Atk"));
		TB_Value->SetText(FText::AsNumber(Temp->GetAtk()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.AtkPoint));
		break;
	case ECharacterStatType::AtkSpeed:
		TB_Name->SetText(FText::FromString("AtkSpeed"));
		TB_Value->SetText(FText::AsNumber(Temp->GetAtkSpeed()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.AtkSpeedPoint));
		break;
	case ECharacterStatType::AtkRange:
		TB_Name->SetText(FText::FromString("AtkRange"));
		TB_Value->SetText(FText::AsNumber(Temp->GetAtkRange()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.AtkRangePoint));
		break;
	case ECharacterStatType::Def:
		TB_Name->SetText(FText::FromString("Def"));
		TB_Value->SetText(FText::AsNumber(Temp->GetDef()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.DefPoint));
		break;
	case ECharacterStatType::Hp:
		TB_Name->SetText(FText::FromString("MaxHp"));
		TB_Value->SetText(FText::AsNumber(Temp->GetMaxHp()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.MaxHpPoint));
		break;
	case ECharacterStatType::Speed:
		TB_Name->SetText(FText::FromString("Speed"));
		TB_Value->SetText(FText::AsNumber(Temp->GetSpeed()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.SpeedPoint));
		break;
	case ECharacterStatType::Jump:
		TB_Name->SetText(FText::FromString("Jump"));
		TB_Value->SetText(FText::AsNumber(Temp->GetJump()));
		TB_Point->SetText(FText::AsNumber(Temp->Stat.JumpPoint));
		break;
	default:
		break;
	}
}

void UPDUWStatSlot::OnButtonClickedPlus()
{
	if (PDGameInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Error : PDUWStatSlot[OnButtonclickedPlus()] GameInstance -> nullptr"));
		return;
	}
	UPDCharacterStat* Temp = PDGameInstance->GetPlayerStat();
	switch (CharacterStatType)
	{
	case ECharacterStatType::None:
		break;
	case ECharacterStatType::Atk:
		Temp->Stat.AtkPoint++;
		break;
	case ECharacterStatType::AtkSpeed:
		Temp->Stat.AtkSpeedPoint++;
		break;
	case ECharacterStatType::AtkRange:
		Temp->Stat.AtkRangePoint++;
		break;
	case ECharacterStatType::Def:
		Temp->Stat.DefPoint++;
		break;
	case ECharacterStatType::Hp:
		Temp->Stat.MaxHpPoint++;
		break;
	case ECharacterStatType::Speed:
		Temp->AddSpeedPoint();
		break;
	case ECharacterStatType::Jump:
		Temp->AddJumpPoint();
		break;
	default:
		break;
	}
	Refresh();
}
