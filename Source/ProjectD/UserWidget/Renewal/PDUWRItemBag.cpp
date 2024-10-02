// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWRItemBag.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "../../PDGameInstance.h"
#include "../../Character/PDCharacterBase.h"
#include "PDUWRItemSlot.h"

void UPDUWRItemBag::Init()
{
	GetReferencePointer();
	Slots.Empty();
	TArray<UWidget*> Widgets;

	UPDUWRItemSlot* ItemSlot;
	for (auto Widget : Widgets)
	{
		ItemSlot = Cast<UPDUWRItemSlot>(Widget);
		if (!ItemSlot)continue;
		ItemSlot->SlotType = ESlotType::Inventory;
		Slots.Emplace(ItemSlot);
	}
}

void UPDUWRItemBag::Refresh()
{
	GetReferencePointer();
	for (int i = 0; i < MaxCount; i++)
	{
		Slots[i]->Refresh();
	}
}

void UPDUWRItemBag::GetReferencePointer()
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

void UPDUWRItemBag::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Init();
	MaxCount = 5;
}
