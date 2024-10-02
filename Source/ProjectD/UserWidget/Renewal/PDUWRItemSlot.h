// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PDEnum.h"
#include "Blueprint/UserWidget.h"
#include "PDUWRItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWRItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void GetReferencePointer();
	void Refresh();
	void SetTexture(UTexture2D* texture);
public:
	class APDCharacterBase* Player;
	class UPDGameInstance* PDGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 SlotNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 Count;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTexture2D* Texture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UPDUWRItemSlot> DragSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* Img_Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TB_Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotType")
		ESlotType SlotType;
};
