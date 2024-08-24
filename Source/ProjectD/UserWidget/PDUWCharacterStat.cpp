// Fill out your copyright notice in the Description page of Project Settings.


#include "PDUWCharacterStat.h"
#include "PDUWStatSlot.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../PDGameInstance.h"

void UPDUWCharacterStat::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Init();
}

void UPDUWCharacterStat::Init()
{
	TArray<UWidget*> Widgets;
	WidgetTree->GetAllWidgets(Widgets);

	UPDUWStatSlot* StatSlot;
	for (auto Widget : Widgets)
	{
		StatSlot = Cast<UPDUWStatSlot>(Widget);
		if (StatSlot == nullptr)continue;
		StatSlots.Add(StatSlot);
	}
}

void UPDUWCharacterStat::Refresh()
{
}
