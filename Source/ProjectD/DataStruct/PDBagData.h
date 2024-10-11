// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "UObject/NoExportTypes.h"
#include "PDBagData.generated.h"

USTRUCT(BlueprintType)
struct FBagData
{
	GENERATED_USTRUCT_BODY()
public:
	FBagData();
	FBagData(int32 index);

	FBagData& operator=(const FBagData& ref)
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
class PROJECTD_API UPDBagData : public UObject
{
	GENERATED_BODY()
public:
	UPDBagData();

	class UPDGameInstance* PDGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FBagData> BagData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* DefaultTexture;
public:
	UFUNCTION()
		int32 AddItem(FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype);
	UFUNCTION()
		void AddItemByIndex(int32 index, FString name, int32 itemCode, UTexture2D* texture, EInventoryType type, EEquipType equiptype);
	UFUNCTION()
		void AddItemByItemcode(int32 itemCode);
	UFUNCTION()
		void RemoveItemByIndex(int32 index);
	UFUNCTION()
		void SwapItemByIndex(int32 indexA, int32 indexB);
};
