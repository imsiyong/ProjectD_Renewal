// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PDRestPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTD_API APDRestPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APDRestPlayerController();

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<class UPDUWCharacterStat> UWCharacterStat;
	class UPDUWCharacterStat* CharacterStat;
};
