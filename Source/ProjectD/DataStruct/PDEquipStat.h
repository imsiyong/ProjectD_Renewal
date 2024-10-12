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

	void SetStat(int32 itemCode, UWorld* world);
	void ResetStat();

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

	FEquipStat operator+(const FEquipStat& ref) const
	{
		FEquipStat result = FEquipStat();
		result.MaxHp = this->MaxHp + ref.MaxHp;
		result.Atk = this->Atk + ref.Atk;
		result.Def = this->Def + ref.Def;
		result.Speed = this->Speed + ref.Speed;
		result.Jump = this->Jump + ref.Jump;
		result.AtkRange = this->AtkRange + ref.AtkRange;
		result.AtkSpeed = this->AtkSpeed + ref.AtkSpeed;
		return result;
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
