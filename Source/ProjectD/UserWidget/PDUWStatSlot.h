// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../PDEnum.h"
#include "PDUWStatSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWStatSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void Init();
	void Refresh();

	UFUNCTION()
		void OnButtonClickedPlus();

	class UPDGameInstance* PDGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* BT_Plus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TB_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TB_Value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TB_Point;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* IMG_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterStatType")
		ECharacterStatType CharacterStatType;
};
