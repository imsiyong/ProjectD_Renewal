// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../PDEnum.h"
#include "GameFramework/Actor.h"
#include "PDItemBase.generated.h"

UCLASS()
class PROJECTD_API APDItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APDItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	class ACharacterBase* Player;
	class UPDGameInstance* PDGameInstance;

	void Init(int32 index);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 ItemCode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTexture2D* Texture;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ItemMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "Item Box")
		class UBoxComponent* ItemBox;
	
};
