// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PDItemDataTable.generated.h"


USTRUCT(BlueprintType)
struct FEqipData : public FTableRowBase
{
	GENERATED_BODY()
public:
	FEqipData() {}
	~FEqipData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 ItemCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AtkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float AtkRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHp;
	
};


/**
 * 
 */
UCLASS()
class PROJECTD_API UPDItemDataTable : public UObject
{
	GENERATED_BODY()
	
};
