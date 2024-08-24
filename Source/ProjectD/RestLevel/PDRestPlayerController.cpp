// Fill out your copyright notice in the Description page of Project Settings.


#include "PDRestPlayerController.h"
#include "../UserWidget/PDUWCharacterStat.h"

APDRestPlayerController::APDRestPlayerController()
{
	bShowMouseCursor = true;

	ConstructorHelpers::FClassFinder<UPDUWCharacterStat> CS(TEXT("WidgetBlueprint'/Game/UMG/Renewal/UWCharacterStat.UWCharacterStat_C'"));
	if (CS.Succeeded())
	{
		UWCharacterStat = CS.Class;
	}
}

void APDRestPlayerController::SetupInputComponent()
{
	if (UWCharacterStat != nullptr)
	{
		CharacterStat = CreateWidget<UPDUWCharacterStat>(this, UWCharacterStat);
		if (CharacterStat != nullptr)
		{
			CharacterStat->AddToViewport();
		}
	}
}
