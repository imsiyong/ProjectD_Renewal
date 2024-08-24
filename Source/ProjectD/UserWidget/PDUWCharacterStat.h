// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUWCharacterStat.generated.h"


class UPDUWStatSlot;
/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWCharacterStat : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void Init();
	void Refresh();

	TArray<UPDUWStatSlot*> StatSlots;


};
