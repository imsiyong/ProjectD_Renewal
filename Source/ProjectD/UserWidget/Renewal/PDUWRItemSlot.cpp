// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWRItemSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "../../DataStruct/PDBagData.h"
#include "../../DataStruct/PDEquipData.h"
#include "../../DataStruct/PDStorageData.h"
#include "../../PDGameInstance.h"
#include "../../Character/PDCharacterBase.h"

void UPDUWRItemSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	Refresh();
}

FReply UPDUWRItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		switch (SlotType)
		{
		case ESlotType::None:
			return reply.NativeReply;
			break;
		case ESlotType::Inventory:
			return reply.NativeReply;
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
	return reply.NativeReply;
}

FReply UPDUWRItemSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GetReferencePointer();

	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	FString name = "";
	int32 itemCode = -1;
	UTexture2D* texture = nullptr;
	EInventoryType inventoryType = EInventoryType::None;
	EEquipType equipType = EEquipType::None;

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		switch (SlotType)
		{
		case ESlotType::None:
			return reply.NativeReply;
			break;
		case ESlotType::Inventory:
			return reply.NativeReply;
			break;
		case ESlotType::Equip:
			name = PDGameInstance->EquipData->EquipData[SlotNum].Name;
			itemCode = PDGameInstance->EquipData->EquipData[SlotNum].ItemCode;
			texture = PDGameInstance->EquipData->EquipData[SlotNum].Texture;
			inventoryType = PDGameInstance->EquipData->EquipData[SlotNum].InventoryType;
			equipType = PDGameInstance->EquipData->EquipData[SlotNum].EquipType;
			if (inventoryType == EInventoryType::None)
			{
				break;
			}
			if (PDGameInstance->StorageData->AddItem(name, itemCode, texture, inventoryType, equipType)!= 0)
			{
				return reply.NativeReply;
			}
			PDGameInstance->EquipData->RemoveItemByIndex(SlotNum);
			break;
		case ESlotType::Storage:
			name = PDGameInstance->StorageData->StorageData[SlotNum].Name;
			itemCode = PDGameInstance->StorageData->StorageData[SlotNum].ItemCode;
			texture = PDGameInstance->StorageData->StorageData[SlotNum].Texture;
			inventoryType = PDGameInstance->StorageData->StorageData[SlotNum].InventoryType;
			equipType = PDGameInstance->StorageData->StorageData[SlotNum].EquipType;
			if (inventoryType == EInventoryType::None)
			{
				break;
			}
			if (PDGameInstance->EquipData->AddItem(name, itemCode, texture, inventoryType, equipType) != 0)
			{
				return reply.NativeReply;
			}
			PDGameInstance->StorageData->RemoveItemByIndex(SlotNum);
			break;
		case ESlotType::Bag:
			name = PDGameInstance->BagData->BagData[SlotNum].Name;
			itemCode = PDGameInstance->BagData->BagData[SlotNum].ItemCode;
			texture = PDGameInstance->BagData->BagData[SlotNum].Texture;
			inventoryType = PDGameInstance->BagData->BagData[SlotNum].InventoryType;
			equipType = PDGameInstance->BagData->BagData[SlotNum].EquipType;
			if (inventoryType == EInventoryType::None)
			{
				break;
			}
			if (PDGameInstance->StorageData->AddItem(name, itemCode, texture, inventoryType, equipType) != 0)
			{
				return reply.NativeReply;
			}
			PDGameInstance->BagData->RemoveItemByIndex(SlotNum);
			break;
		default:
			break;
		}
	}
	return reply.NativeReply;
}

void UPDUWRItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UPDUWRItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

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
		return;
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
	{
		if (!Player || !Player->EquipData->EquipData.IsValidIndex(SlotNum))return;
		FEquipSlotData EquipData = Player->EquipData->EquipData[SlotNum];
		Texture = EquipData.Texture;
		Count = EquipData.Count;
		break;
	}
		break;
	case ESlotType::Storage:
	{
		if (!Player || !Player->StorageData->StorageData.IsValidIndex(SlotNum))return;
		FStorageData StorageData = Player->StorageData->StorageData[SlotNum];
		Texture = StorageData.Texture;
		Count = StorageData.Count;
		break;
	}
		break;
	case ESlotType::Bag:
	{
		if (!Player || !Player->BagData->BagData.IsValidIndex(SlotNum))return;
		FBagData BagData = Player->BagData->BagData[SlotNum];
		Texture = BagData.Texture;
		Count = BagData.Count;
		break;
	}
	default:
		return;
		break;
	}
	//Set Texture
	if (Texture != nullptr)
	{
		SetTexture(Texture);
	}
	//Set Count
	if (Count <= 1)
	{
		TB_Count->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TB_Count->SetVisibility(ESlateVisibility::Visible);
		TB_Count->SetText(FText::FromString(FString::FromInt(Count)));
	}
}

void UPDUWRItemSlot::SetTexture(UTexture2D* texture)
{
	if (texture == nullptr)
		return;
	Img_Icon->SetBrushFromTexture(texture);
}


