// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PDEnum.h"
#include "Blueprint/DragDropOperation.h"
#include "PDUWDrag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API UPDUWDrag : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 index;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EEquipType equiptype;

};
