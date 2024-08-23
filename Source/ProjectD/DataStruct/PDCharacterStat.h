// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "UObject/NoExportTypes.h"
#include "PDCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStat
{
	GENERATED_USTRUCT_BODY()
public:
	FCharacterStat();
	FCharacterStat(int32 index);

	FCharacterStat& operator=(const FCharacterStat& ref)
	{
		Index = ref.Index;
		MaxHp = ref.MaxHp;
		MaxHpWeight = ref.MaxHpWeight;
		MaxHpPoint = ref.MaxHpPoint;
		CurHp = ref.CurHp;
		Atk = ref.Atk;
		AtkWeight = ref.AtkWeight;
		AtkPoint = ref.AtkPoint;
		Def = ref.Def;
		DefWeight = ref.DefWeight;
		DefPoint = ref.DefPoint;
		Speed = ref.Speed;
		SpeedWeight = ref.SpeedWeight;
		SpeedPoint = ref.SpeedPoint;
		Jump = ref.Jump;
		JumpWeight = ref.JumpWeight;
		JumpPoint = ref.JumpPoint;
		AtkRange = ref.AtkRange;
		AtkRangeWeight = ref.AtkRangeWeight;
		AtkRangePoint = ref.AtkRangePoint;
		AtkSpeed = ref.AtkSpeed;
		AtkSpeedWeight = ref.AtkSpeedWeight;
		AtkSpeedPoint = ref.AtkSpeedPoint;
		Sight = ref.Sight;
		SightWeight = ref.SightWeight;
		SightPoint = ref.SightPoint;
		return *this;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHpWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxHpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Atk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AtkPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Def;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DefPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SpeedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 JumpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkRangeWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AtkRangePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AtkSpeedWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AtkSpeedPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Sight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SightWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SightPoint;

};

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDCharacterStat : public UObject
{
	GENERATED_BODY()
public:
	UPDCharacterStat();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FCharacterStat Stat;
	
	float GetMaxHp() { return Stat.MaxHp + Stat.MaxHpPoint * Stat.MaxHpWeight; }
	float GetAtk() { return Stat.Atk + Stat.AtkPoint * Stat.AtkWeight; }
	float GetDef() { return Stat.Def + Stat.DefPoint * Stat.DefWeight; }
	float GetSpeed() { return Stat.Speed + Stat.SpeedPoint * Stat.SpeedWeight; }
	float GetJump() { return Stat.Jump + Stat.JumpPoint * Stat.JumpWeight; }
	float GetAtkRange() { return Stat.AtkRange + Stat.AtkRangePoint * Stat.AtkRangeWeight; }
	float GetAtkSpeed() { return Stat.AtkSpeed + Stat.AtkSpeedPoint * Stat.AtkSpeedWeight; }
	float GetSight() { return Stat.Sight + Stat.SightPoint * Stat.SightWeight; }

};
