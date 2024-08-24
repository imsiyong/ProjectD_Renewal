// Fill out your copyright notice in the Description page of Project Settings.


#include "PDEquipStat.h"

FEquipStat::FEquipStat()
{
	Index = 0;
	MaxHp = 0.0f;
	Atk = 0.0f;
	Def = 0.0f;
	Speed = 0.0f;
	Jump = 0.0f;
	AtkRange = 0.0f;
	AtkSpeed = 0.0f;
}

UPDEquipStat::UPDEquipStat()
{
	Stat = FEquipStat();
}