// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class EMonsterBasicState
{
	LIVE,
	DEATH
};

enum class EPlayerBasicState
{
	LIVE,
	DEATH
};

enum class EMonsterActionState
{
	PATROL,
	MoveToPlayer,
	Attack,
	RESET,
	SetDest,
	MoveToDest,
	CheckDist,
	CheckField,
	MoveToCenter
};

enum class ENormalMonsterActionState
{
	None,
	Move,
	Find,
	Attack,
	Spawn,
	Patrol,
	Tracking
};

enum class EItemBasicState
{
	None,
	Drop,
	Obtain,
	Mount
};

enum class EWeaponType
{
	None,
	Sword,
	Mace
};

UENUM(BlueprintType)
enum class ECharacterStatType:uint8
{
	None		UMETA(DisplayName = "None"),
	Atk			UMETA(DisplayName = "Atk"),
	AtkSpeed	UMETA(DisplayName = "AtkSpeed"),
	AtkRange	UMETA(DisplayName = "AtkRange"),
	Def			UMETA(DisplayName = "Def"),
	Hp			UMETA(DisplayName = "Hp"),
	Speed		UMETA(DisplayName = "Speed"),
	Jump		UMETA(DisplayName = "Jump")
};

UENUM(BlueprintType)
enum class ESlotType :uint8
{
	None		UMETA(DisplayName ="None"),
	Inventory	UMETA(DisplayName="Weapon"),
	Equip		UMETA(DisplayName="Equip"),
	Storage		UMETA(DisplayName="Storage"),
	Bag			UMETA(DisplayName="Bag")
};

UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	None		UMETA(DisplayName = "None"),
	Weapon		UMETA(DisplayName = "Weapon")
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	Head		UMETA(DisplayName = "Weapon"),
	Body		UMETA(DisplayName = "Body"),
	Foot		UMETA(DisplayName = "Foot"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right"),
	None		UMETA(DisplayName = "None")
};

UENUM(BlueprintType)
enum class EMonsterGrade : uint8
{
	None		UMETA(DisplayName = "None"),
	Normal		UMETA(DisplayName = "Normal"),
	Boss		UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None		UMETA(DisplayName = "None"),
	Stat		UMETA(DisplyaName = "Stat"),
	ItemManager	UMETA(DisplayName = "ItemManager")
};