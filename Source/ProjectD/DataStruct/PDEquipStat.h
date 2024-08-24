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
	FEquipStat& operator=(const FEquipStat& ref)
	{
		Index = ref.Index;
		MaxHp = ref.MaxHp;
		Atk = ref.Atk;
		Def = ref.Def;
		Speed = ref.Speed;
		Jump = ref.Jump;
		AtkRange = ref.AtkRange;
		AtkSpeed = ref.AtkSpeed;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Def;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkSpeed;
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
