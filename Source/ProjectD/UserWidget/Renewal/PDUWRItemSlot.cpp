// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWRItemSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PDUWRDragDrop.h"
#include "../../DataStruct/PDBagData.h"
#include "../../DataStruct/PDEquipData.h"
#include "../../DataStruct/PDStorageData.h"
#include "../../PDGameInstance.h"
#include "../../Character/PDCharacterBase.h"
#include "../../RestLevel/PDRestPlayerController.h"

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
			if (PDGameInstance->EquipData->EquipData[SlotNum].InventoryType == EInventoryType::None)
				return reply.NativeReply;
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			break;
		case ESlotType::Storage:
			if (PDGameInstance->StorageData->StorageData[SlotNum].InventoryType == EInventoryType::None)
				return reply.NativeReply;
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			break;
		case ESlotType::Bag:
			if (PDGameInstance->BagData->BagData[SlotNum].InventoryType == EInventoryType::None)
				return reply.NativeReply;
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
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
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (OutOperation == nullptr)
	{
		UPDUWRDragDrop* drag = NewObject<UPDUWRDragDrop>();
		OutOperation = drag;
		SetDragData(drag);
		if (DragSlot != nullptr)
		{
			UPDUWRItemSlot* Visual = CreateWidget<UPDUWRItemSlot>(Cast<APlayerController>(Player->Controller), DragSlot);
			Visual->SlotType = this->SlotType;
			Visual->SlotNum = this->SlotNum;
			Visual->Player = this->Player;
			Visual->PDGameInstance = this->PDGameInstance;
			drag->DefaultDragVisual = Visual;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("drag failed"));
	}
}

bool UPDUWRItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UPDUWRDragDrop* Drag = Cast<UPDUWRDragDrop>(InOperation);
	if (Drag == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("NativeOnDrop : error"));
		return false;
	}

	switch (SlotType)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		if (PDGameInstance->EquipData->EquipData[SlotNum].ItemCode != -1)
		{
			return false;
		}
		SwapData(Drag->SlotType, Drag->Index, SlotType, SlotNum);
		/*if (PDGameInstance->EquipData->AddItem(Drag->Name, Drag->ItemCode, Drag->Texture, Drag->InventoryType, Drag->EquipType) != 0)
		{
			return false;
		}
		RemoveDataByIndex(Drag->SlotType, Drag->Index);*/
		break;
	case ESlotType::Storage:
		SwapData(Drag->SlotType, Drag->Index, SlotType, SlotNum);
		/*if (PDGameInstance->StorageData->AddItemByIndex(SlotNum, Drag->Name, Drag->ItemCode, Drag->Texture, Drag->InventoryType, Drag->EquipType) != 0)
		{
			return false;
		}
		RemoveDataByIndex(Drag->SlotType, Drag->Index);*/
		break;
	case ESlotType::Bag:
		SwapData(Drag->SlotType, Drag->Index, SlotType, SlotNum);
		/*if (PDGameInstance->BagData->AddItemByIndex(SlotNum, Drag->Name, Drag->ItemCode, Drag->Texture, Drag->InventoryType, Drag->EquipType) != 0)
		{
			return false;
		}
		RemoveDataByIndex(Drag->SlotType, Drag->Index);*/
		break;
	default:
		break;
	}
	return true;
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
	IMG_Icon->SetBrushFromTexture(texture);
}

void UPDUWRItemSlot::RemoveDataByIndex(ESlotType slotType, int32 index)
{
	switch (slotType)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		PDGameInstance->EquipData->RemoveItemByIndex(index);
		break;
	case ESlotType::Storage:
		PDGameInstance->StorageData->RemoveItemByIndex(index);
		break;
	case ESlotType::Bag:
		PDGameInstance->BagData->RemoveItemByIndex(index);
		break;
	default:
		break;
	}
}

void UPDUWRItemSlot::AddDataByIndex(ESlotType slotType, int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType inventoryType, EEquipType equipType)
{
	GetReferencePointer();
	switch (slotType)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		PDGameInstance->EquipData->AddItemByIndex(index, name, itemCode, texture, inventoryType, equipType);
		break;
	case ESlotType::Storage:
		PDGameInstance->StorageData->AddItemByIndex(index, name, itemCode, texture, inventoryType, equipType);
		break;
	case ESlotType::Bag:
		PDGameInstance->BagData->AddItemByIndex(index, name, itemCode, texture, inventoryType, equipType);
		break;
	default:
		break;
	}
}

void UPDUWRItemSlot::SetDragData(UPDUWRDragDrop* Drag)
{
	int32 index = this->SlotNum;
	int32 count = this->Count;
	int32 itemCode = -1;
	FString name = "";
	UTexture2D* texture = nullptr;
	EEquipType equipType = EEquipType::None;
	EInventoryType inventoryType = EInventoryType::None;
	switch (SlotType)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		name = PDGameInstance->EquipData->EquipData[SlotNum].Name;
		itemCode = PDGameInstance->EquipData->EquipData[SlotNum].ItemCode;
		texture = PDGameInstance->EquipData->EquipData[SlotNum].Texture;
		inventoryType = PDGameInstance->EquipData->EquipData[SlotNum].InventoryType;
		equipType = PDGameInstance->EquipData->EquipData[SlotNum].EquipType;
		break;
	case ESlotType::Storage:
		name = PDGameInstance->StorageData->StorageData[SlotNum].Name;
		itemCode = PDGameInstance->StorageData->StorageData[SlotNum].ItemCode;
		texture = PDGameInstance->StorageData->StorageData[SlotNum].Texture;
		inventoryType = PDGameInstance->StorageData->StorageData[SlotNum].InventoryType;
		equipType = PDGameInstance->StorageData->StorageData[SlotNum].EquipType;
		break;
	case ESlotType::Bag:
		name = PDGameInstance->BagData->BagData[SlotNum].Name;
		itemCode = PDGameInstance->BagData->BagData[SlotNum].ItemCode;
		texture = PDGameInstance->BagData->BagData[SlotNum].Texture;
		inventoryType = PDGameInstance->BagData->BagData[SlotNum].InventoryType;
		equipType = PDGameInstance->BagData->BagData[SlotNum].EquipType;
		break;
	default:
		break;
	}
	Drag->Index = index;
	Drag->Count = count;
	Drag->ItemCode = itemCode;
	Drag->Name = name;
	Drag->Texture = texture;
	Drag->InventoryType = inventoryType;
	Drag->EquipType = equipType;
	Drag->SlotType = this->SlotType;
}

void UPDUWRItemSlot::SwapData(ESlotType preSlot, int32 preIndex, ESlotType nextSlot, int32 nextIndex)
{
	int32 itemCode[2];
	FString name[2];
	UTexture2D* texture[2];
	EEquipType equipType[2];
	EInventoryType inventoryType[2];
	switch (preSlot)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		itemCode[0] = PDGameInstance->EquipData->EquipData[preIndex].ItemCode;
		name[0] = PDGameInstance->EquipData->EquipData[preIndex].Name;
		texture[0] = PDGameInstance->EquipData->EquipData[preIndex].Texture;
		equipType[0] = PDGameInstance->EquipData->EquipData[preIndex].EquipType;
		inventoryType[0] = PDGameInstance->EquipData->EquipData[preIndex].InventoryType;
		break;
	case ESlotType::Storage:
		itemCode[0] = PDGameInstance->StorageData->StorageData[preIndex].ItemCode;
		name[0] = PDGameInstance->StorageData->StorageData[preIndex].Name;
		texture[0] = PDGameInstance->StorageData->StorageData[preIndex].Texture;
		equipType[0] = PDGameInstance->StorageData->StorageData[preIndex].EquipType;
		inventoryType[0] = PDGameInstance->StorageData->StorageData[preIndex].InventoryType;
		break;
	case ESlotType::Bag:
		itemCode[0] = PDGameInstance->BagData->BagData[preIndex].ItemCode;
		name[0] = PDGameInstance->BagData->BagData[preIndex].Name;
		texture[0] = PDGameInstance->BagData->BagData[preIndex].Texture;
		equipType[0] = PDGameInstance->BagData->BagData[preIndex].EquipType;
		inventoryType[0] = PDGameInstance->BagData->BagData[preIndex].InventoryType;
		break;
	default:
		break;
	}
	switch (nextSlot)
	{
	case ESlotType::None:
		break;
	case ESlotType::Inventory:
		break;
	case ESlotType::Equip:
		itemCode[1] = PDGameInstance->EquipData->EquipData[nextIndex].ItemCode;
		name[1] = PDGameInstance->EquipData->EquipData[nextIndex].Name;
		texture[1] = PDGameInstance->EquipData->EquipData[nextIndex].Texture;
		equipType[1] = PDGameInstance->EquipData->EquipData[nextIndex].EquipType;
		inventoryType[1] = PDGameInstance->EquipData->EquipData[nextIndex].InventoryType;
		break;
	case ESlotType::Storage:
		itemCode[1] = PDGameInstance->StorageData->StorageData[nextIndex].ItemCode;
		name[1] = PDGameInstance->StorageData->StorageData[nextIndex].Name;
		texture[1] = PDGameInstance->StorageData->StorageData[nextIndex].Texture;
		equipType[1] = PDGameInstance->StorageData->StorageData[nextIndex].EquipType;
		inventoryType[1] = PDGameInstance->StorageData->StorageData[nextIndex].InventoryType;
		break;
	case ESlotType::Bag:
		itemCode[1] = PDGameInstance->BagData->BagData[nextIndex].ItemCode;
		name[1] = PDGameInstance->BagData->BagData[nextIndex].Name;
		texture[1] = PDGameInstance->BagData->BagData[nextIndex].Texture;
		equipType[1] = PDGameInstance->BagData->BagData[nextIndex].EquipType;
		inventoryType[1] = PDGameInstance->BagData->BagData[nextIndex].InventoryType;
		break;
	default:
		break;
	}
	AddDataByIndex(preSlot, preIndex, name[1], itemCode[1], texture[1],inventoryType[1], equipType[1]);
	AddDataByIndex(nextSlot, nextIndex, name[0], itemCode[0], texture[0], inventoryType[0], equipType[0]);
}



