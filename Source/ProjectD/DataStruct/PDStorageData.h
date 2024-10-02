// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "UObject/NoExportTypes.h"
#include "PDStorageData.generated.h"

USTRUCT(BlueprintType)
struct FStorageData
{
	GENERATED_USTRUCT_BODY()
public:
	FStorageData();
	FStorageData(int32 index);

	FStorageData& operator=(const FStorageData& ref)
	{
		Index = ref.Index;
		Count = ref.Count;
		ItemCode = ref.ItemCode;
		Name = ref.Name;
		Texture = ref.Texture;
		EquipType = ref.EquipType;
		InventoryType = ref.InventoryType;
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
};

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDStorageData : public UObject
{
	GENERATED_BODY()
public:
	UPDStorageData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FStorageData> StorageData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* DefaultTexture;
public:
	UFUNCTION()
		void AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype);
	UFUNCTION()
		void AddItemByIndex(int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype);
	UFUNCTION()
		void RemoveItemByIndex(int32 index);
	UFUNCTION()
		void SwapItemByIndex(int32 indexA, int32 indexB);
};
