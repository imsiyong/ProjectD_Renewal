// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacterBase.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "../PDGameInstance.h"
#include "../BattleLevel/PDBattlePlayerController.h"
#include "../UserWidget/PDUWBattleStatus.h"
#include "../RestLevel/PDRestPlayerController.h"
#include "../Manager/PDNormalMonsterManager.h"
#include "../Object/Item/PDItemBase.h"
#include "../DataStruct/PDBagData.h"
#include "../DataStruct/PDEquipData.h"

// Sets default values
APDCharacterBase::APDCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PDCharacter"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	MouseInputValid = true;
	MovementInputValid = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	//
	//
	Tags.Add(FName("Player"));
}

// Called when the game starts or when spawned
void APDCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APDCharacterBase::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APDCharacterBase::EndOverlap);

	if (PDGameInstance)
	{
		CharacterStat = PDGameInstance->GetPlayerStat();
		if (CharacterStat)
		{
			GetCharacterMovement()->MaxWalkSpeed = CharacterStat->GetSpeed();
			GetCharacterMovement()->JumpZVelocity = CharacterStat->GetJump();
		}
		BagData = PDGameInstance->GetBagData();
		EquipData = PDGameInstance->GetEquipData();
		StorageData = PDGameInstance->GetStorageData();
	}
	PDPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ChangeCurHp(CharacterStat->GetMaxHp());
	UE_LOG(LogTemp, Log, TEXT("Test Character BeginPlay STart Cur Hp : %f"), CharacterStat->Stat.CurHp);
}

// Called every frame
void APDCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APDCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APDCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APDCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APDCharacterBase::MyTurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &APDCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APDCharacterBase::MyLookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APDCharacterBase::LookUpAtRate);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APDCharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APDCharacterBase::StopJumping);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &APDCharacterBase::ToggleInteractionWidget);
}

void APDCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PDGameInstance = Cast<UPDGameInstance>(GetGameInstance());
}

void APDCharacterBase::MoveForward(float Value)
{
	if (MovementInputValid && (Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APDCharacterBase::MoveRight(float Value)
{
	if (MovementInputValid && (Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APDCharacterBase::Jump()
{
	if (MovementInputValid)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void APDCharacterBase::StopJumping()
{
	if (MovementInputValid)
	{
		bPressedJump = false;
		ResetJumpState();
	}
}

void APDCharacterBase::TurnAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APDCharacterBase::MyTurnAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerYawInput(Rate);
}

void APDCharacterBase::LookUpAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APDCharacterBase::MyLookUpAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerPitchInput(Rate);
}

float APDCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ChangeCurHp(CharacterStat->Stat.CurHp - DamageAmount);
	UE_LOG(LogTemp, Log, TEXT("Player code[%d] CurHp : %f"), CharacterStat->Stat.Index, CharacterStat->Stat.CurHp);
	return 0.0f;
}

void APDCharacterBase::ChangeCurHp(float curHp)
{
	if (PDPlayerController)
	{
		auto TempPlayerController = Cast<APDBattlePlayerController>(PDPlayerController);
		if (TempPlayerController==nullptr)
			return;
		if (curHp <= 0)
		{
			CharacterStat->Stat.CurHp = 0;
			TempPlayerController->BattleStatusWidget->SetHpBar(0.0f);
		}
		else if (curHp >= CharacterStat->GetMaxHp())
		{
			CharacterStat->Stat.CurHp = CharacterStat->GetMaxHp();
			TempPlayerController->BattleStatusWidget->SetHpBar(1.f);
		}
		else
		{
			CharacterStat->Stat.CurHp = curHp;
			TempPlayerController->BattleStatusWidget->SetHpBar(CharacterStat->Stat.CurHp / CharacterStat->GetMaxHp());
		}
	}
}

void APDCharacterBase::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = CharacterStat->GetAtkRange();
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	if (bResult && HitResult.Actor.IsValid() && HitResult.Actor->ActorHasTag("Monster"))
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(CharacterStat->GetAtk(), DamageEvent, GetController(), this);
	}
}

void APDCharacterBase::ToggleInteractionWidget()
{
	auto PlayerController = Cast<APDRestPlayerController>(PDPlayerController);
	if (PlayerController == nullptr)
		return;
	PlayerController->ToggleInteractionWidget();
	if (PlayerController->InteractionType != EInteractionType::None)
	{
		if (MovementInputValid)
		{
			MovementInputValid = false;
		}
		else
		{
			MovementInputValid = true;
		}

		if (MouseInputValid)
		{
			MouseInputValid = false;
		}
		else
		{
			MouseInputValid = true;
		}
	}
}

void APDCharacterBase::WeaponMount()
{
	for (int32 i = 0; i < EquipData->MaxCount; i++)
	{
		if (!EquipData->EquipData[i].CheckMount)
		{
			if (EquipData->EquipData[i].ItemCode == -1)
			{
				//아이템 메시 장착 해제
				//캐릭터 스탯 복구
			}
			else
			{
				//아이템 메시 장착
				//캐릭터 스탯 변경
			}
		}
	}
}

void APDCharacterBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Test")))
	{
		auto PlayerController = Cast<APDRestPlayerController>(PDPlayerController);
		if (PlayerController == nullptr)
			return;
		PlayerController->InteractionType = EInteractionType::Stat;
	}
	else if (OtherActor->ActorHasTag(FName("Test2")))
	{
		auto PlayerController = Cast<APDRestPlayerController>(PDPlayerController);
		if (PlayerController == nullptr)
			return;
		PlayerController->InteractionType = EInteractionType::ItemManager;
	}
}

void APDCharacterBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Test")))
	{
		MovementInputValid = true;
		MouseInputValid = true;
		auto PlayerController = Cast<APDRestPlayerController>(PDPlayerController);
		if (PlayerController == nullptr)
			return;
		PlayerController->InteractionType = EInteractionType::None;
		PlayerController->RemoveAllInteractionWidget();
	}
	else if (OtherActor->ActorHasTag(FName("Test2")))
	{
		MovementInputValid = true;
		MouseInputValid = true;
		auto PlayerController = Cast<APDRestPlayerController>(PDPlayerController);
		if (PlayerController == nullptr)
			return;
		PlayerController->InteractionType = EInteractionType::None;
		PlayerController->RemoveAllInteractionWidget();
	}
}

