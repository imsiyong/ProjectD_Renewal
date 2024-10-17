// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "PDUWRDragDrop.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWRDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESlotType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Texture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEquipType EquipType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SlotType")
		EInventoryType InventoryType;
};
