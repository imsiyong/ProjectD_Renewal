// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWRItemSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../../PDGameInstance.h"
#include "../../Character/PDCharacterBase.h"

void UPDUWRItemSlot::GetReferencePointer()
{
	if (PDGameInstance == nullptr)
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance)
		{
			PDGameInstance = Cast<UPDGameInstance>(GameInstance);
		}
	}
	if (Player == nullptr)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PlayerController)
		{
			Player = Cast<APDCharacterBase>(PlayerController->GetPawn());
		}
	}
}

void UPDUWRItemSlot::Refresh()
{
	GetReferencePointer();
	switch (SlotType)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		break;
	case ESlotType::Storage:
		break;
	case ESlotType::Bag:
		break;
	default:
		break;
	}
}

void UPDUWRItemSlot::SetTexture(UTexture2D* texture)
{
}
