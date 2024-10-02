// Fill out your copyright notice in the Description page of Project Settings.


#include "PDBattlePlayerController.h"
#include "../UserWidget/PDUWBattleStatus.h"

APDBattlePlayerController::APDBattlePlayerController()
{
	ConstructorHelpers::FClassFinder<UPDUWBattleStatus> BS(TEXT("WidgetBlueprint'/Game/UMG/BattleStatus.BattleStatus_C'"));
	if (BS.Succeeded())
	{
		PDUWBattleStatus = BS.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> BB(TEXT("WidgetBlueprint'/Game/UMG/Renewal/UWBattleBag.UWBattleBag_C'"));
	if (BB.Succeeded())
	{
		PDUWBattleBag = BB.Class;
	}
}

void APDBattlePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void APDBattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (PDUWBattleStatus)
	{
		BattleStatusWidget = CreateWidget<UPDUWBattleStatus>(this, PDUWBattleStatus);
		if (BattleStatusWidget)
		{
			BattleStatusWidget->AddToViewport();
		}
	}
	if (PDUWBattleBag)
	{
		BattleBag = CreateWidget<UUserWidget>(this, PDUWBattleBag);
		if (BattleBag)
		{
			BattleBag->AddToViewport();
		}
	}
}
