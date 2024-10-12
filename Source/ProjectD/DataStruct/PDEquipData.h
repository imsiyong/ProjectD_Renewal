// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "PDEquipStat.h"
#include "UObject/NoExportTypes.h"
#include "PDEquipData.generated.h"

USTRUCT(BlueprintType)
struct FEquipSlotData
{
	GENERATED_USTRUCT_BODY()
public:
	FEquipSlotData();
	FEquipSlotData(int32 index);

	FEquipSlotData& operator=(const FEquipSlotData& ref)
	{
		Index = ref.Index;
		Count = ref.Count;
		ItemCode = ref.ItemCode;
		Name = ref.Name;
		Texture = ref.Texture;
		EquipType = ref.EquipType;
		InventoryType = ref.InventoryType;
		CheckMount = ref.CheckMount;
		return *this;
	}

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CheckMount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FEquipStat EquipStat;
};

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDEquipData : public UObject
{
	GENERATED_BODY()
public:
	UPDEquipData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FEquipSlotData> EquipData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* DefaultTexture;
public:
	UFUNCTION()
		int32 AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype);
	UFUNCTION()
		void RemoveItemByIndex(int32 index);
};
