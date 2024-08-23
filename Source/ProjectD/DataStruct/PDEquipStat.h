// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "UObject/NoExportTypes.h"
#include "PDEquipStat.generated.h"

USTRUCT(BlueprintType)
struct FEquipStat
{
	GENERATED_USTRUCT_BODY()
public:
	FEquipStat();
	FEquipStat(int32 index);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;

};

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDEquipStat : public UObject
{
	GENERATED_BODY()
public:
	UPDEquipStat();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FEquipStat Stat;
};
