// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PDUWRItemBag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWRItemBag : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init();
	void Refresh();
	void GetReferencePointer();
	virtual void NativeOnInitialized() override;

public:
	class APDCharacterBase* Player;
	class UPDGameInstance* PDGameInstance;

	TArray<class UPDUWRItemSlot*> Slots;
	int32 MaxCount;
};
