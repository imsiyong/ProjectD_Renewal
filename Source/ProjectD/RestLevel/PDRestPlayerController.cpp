// Fill out your copyright notice in the Description page of Project Settings.


#include "PDRestPlayerController.h"
#include "../UserWidget/PDUWCharacterStat.h"

APDRestPlayerController::APDRestPlayerController()
{
	InteractionType = EInteractionType::None;
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
	}
}

void APDRestPlayerController::ToggleInteractionWidget()
{
	switch (InteractionType)
	{
	case EInteractionType::None:
		break;
	case EInteractionType::Stat:
		if (CharacterStat != nullptr)
		{
			if (CharacterStat->IsInViewport())
			{
				CharacterStat->RemoveFromViewport();
				bShowMouseCursor = false;
			}
			else
			{
				CharacterStat->AddToViewport();
				bShowMouseCursor = true;
			}
		}
		break;
	case EInteractionType::Inventory:
		break;
	case EInteractionType::Equip:
		break;
	default:
		break;
	}
}

void APDRestPlayerController::RemoveAllInteractionWidget()
{
	bShowMouseCursor = false;
	if (CharacterStat->IsInViewport())
	{
		CharacterStat->RemoveFromViewport();
	}
}

