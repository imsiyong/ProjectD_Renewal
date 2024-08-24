// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacterStat.h"
#include <Kismet/GameplayStatics.h>
#include "../Character/PDCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

FCharacterStat::FCharacterStat()
{
	Index = 0;
	MaxHp = 1000.f;
	MaxHpWeight = 100.f;
	MaxHpPoint = 0;
	CurHp = MaxHp;
	Atk = 500.f;
	AtkWeight=100.f;
	AtkPoint=0;
	Def=0.f;
	DefWeight=50.f;
	DefPoint=0;
	Speed=600.f;
	SpeedWeight=20.f;
	SpeedPoint=0;
	Jump=600.f;
	JumpWeight=10.f;
	JumpPoint=0;
	AtkRange=100.f;
	AtkRangeWeight=10.f;
	AtkRangePoint=0;
	AtkSpeed=1.0f;
	AtkSpeedWeight=0.1f;
	AtkSpeedPoint=0;
	Sight=600.f;
	SightWeight=100.f;
	SightPoint=0;
}

FCharacterStat::FCharacterStat(int32 index)
{
	Index = index;
	MaxHp = 1000.f;
	MaxHpWeight = 100.f;
	MaxHpPoint = 0;
	CurHp = MaxHp;
	Atk = 500.f;
	AtkWeight = 100.f;
	AtkPoint = 0;
	Def = 0.f;
	DefWeight = 50.f;
	DefPoint = 0;
	Speed = 600.f;
	SpeedWeight = 20.f;
	SpeedPoint = 0;
	Jump = 600.f;
	JumpWeight = 10.f;
	JumpPoint = 0;
	AtkRange = 100.f;
	AtkRangeWeight = 10.f;
	AtkRangePoint = 0;
	AtkSpeed = 1.0f;
	AtkSpeedWeight = 0.1f;
	AtkSpeedPoint = 0;
	Sight = 600.f;
	SightWeight = 100.f;
	SightPoint = 0;
}

UPDCharacterStat::UPDCharacterStat()
{
	Stat = FCharacterStat();
}

void UPDCharacterStat::AddSpeedPoint()
{
	ACharacter* TempCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TempCharacter)
	{
		APDCharacterBase* Player = Cast<APDCharacterBase>(TempCharacter);
		if (Player)
		{
			Stat.SpeedPoint++;
			Player->GetCharacterMovement()->MaxWalkSpeed = GetSpeed();
		}
	}
}

void UPDCharacterStat::AddJumpPoint()
{
	ACharacter* TempCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TempCharacter)
	{
		APDCharacterBase* Player = Cast<APDCharacterBase>(TempCharacter);
		if (Player)
		{
			Stat.JumpPoint++;
			Player->GetCharacterMovement()->JumpZVelocity = GetJump();
		}
	}
}
