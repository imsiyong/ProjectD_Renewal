// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectDCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PDInstanceCharacter.h"
#include "PDItem.h"
#include "PDSword.h"
#include "PDMace.h"
#include "PDPlayerController.h"
#include "UserWidget/PDUWBattleStatus.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include "PDMonsterManager.h"
#include "PDMonsterAIController.h"
#include "PDMonsterSample.h"
#include "PDCharacterItemInventory.h"
#include "UserWidget/PDItemInventory.h"
#include "PDCharacterEquip.h"
#include "UserWidget/PDUWEquip.h"
#include "ItemData.h"
#include "PDGameInstance.h"
#include "Manager/PDNormalMonsterManager.h"
#include "Monster/PDMonster1.h"
#include "Manager/PDSpawnManager.h"

//////////////////////////////////////////////////////////////////////////
// AProjectDCharacter

AProjectDCharacter::AProjectDCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	MouseInputValid = true;

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
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}

	//Animation
	ConstructorHelpers::FClassFinder<UPDInstanceCharacter> AM(TEXT("AnimBlueprint'/Game/BluePrint/AnimInstance/BPCharacter1Anim.BPCharacter1Anim_C'"));
	if (AM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AM.Class);
	}
	
	//Dash
	ConstructorHelpers::FObjectFinder<UCurveFloat> Curve(TEXT("/Game/BluePrint/DashCurve.DashCurve"));
	if (Curve.Succeeded())
	{
		DashCurve = Curve.Object;
	}

	Tags.Add(FName("Player"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PDCharacter"));
	CharacterWeaponType = EWeaponType::None;

	//equip set
	Equip = NewObject<UPDCharacterEquip>();
	ItemArray.Empty();
	for (int i = 0; i < 5; i++)
	{
		ItemArray.Emplace(nullptr);
	}

	//test
	Inventory22 = NewObject<UPDCharacterItemInventory>();
	UTexture2D* texture2 = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("/Game/DownloadAsset/MyTexture/Texture_Mace.Texture_Mace")));
	if (texture2)
	{
		Inventory22->AddItemByIndex(1, FString(TEXT("Mace")),2, texture2, EInventoryType::Weapon, EEquipType::Right);
	}
}

void AProjectDCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProjectDCharacter::BeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AProjectDCharacter::EndOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AProjectDCharacter::Hit);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PDPlayerController = Cast<APDPlayerController>(PlayerController);

	//Dash
	if (DashCurve != nullptr)
	{
		DashTimeLineUpdateDelegate.BindUFunction(this, FName("DashStart"));
		DashTimeLineFinishDelegate.BindUFunction(this, FName("DashEnd"));
		DashTimeline.AddInterpFloat(DashCurve, DashTimeLineUpdateDelegate);
		DashTimeline.SetTimelineFinishedFunc(DashTimeLineFinishDelegate);
	}
}

void AProjectDCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DashTimeline.TickTimeline(DeltaTime);
}

void AProjectDCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//stat initialize
	if (UGameInstance* GameInstace = GetGameInstance())
	{
		PDGameInstance = Cast<UPDGameInstance>(GameInstace);
		if (PDGameInstance)
		{
			if (PDGameInstance->GetPlayerStatArray().Num() == 0)
			{
				PDGameInstance->PlayerStatArrayAdd();
			}
			Stat = PDGameInstance->GetPlayerStatArray().Top();
		}
	}
	if (Stat)
	{
		FinalStat = *Stat;
	}

	UPDInstanceCharacter* AnimInstance = Cast<UPDInstanceCharacter>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AProjectDCharacter::AttackCheck);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AProjectDCharacter::BasicAttackStart);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AProjectDCharacter::BasicAttackEnd);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AProjectDCharacter::StartDash);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AProjectDCharacter::StopDash);
	PlayerInputComponent->BindAction("ActionKey", IE_Pressed, this, &AProjectDCharacter::SpawnMonsterSample);


	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectDCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectDCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AProjectDCharacter::MyTurnAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectDCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &AProjectDCharacter::MyLookUpAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectDCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectDCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectDCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProjectDCharacter::OnResetVR);
}

void AProjectDCharacter::BasicAttackStart()
{
	auto AnimInstance = Cast<UPDInstanceCharacter>(GetMesh()->GetAnimInstance());
	if (MouseInputValid && AnimInstance)
	{
		AnimInstance->PlayAttackMontage(FinalStat.AtkSpeed);
	}
}

void AProjectDCharacter::BasicAttackEnd()
{
}

bool AProjectDCharacter::SwapInventory(int32 index1, int32 index2)
{
	Inventory22->SwapItemByIndex(index1, index2);
	PDPlayerController->ItemInventory->Refresh();
	return true;
}

bool AProjectDCharacter::EquipItem(int32 index1)
{
	if (!Inventory22->Inventory.IsValidIndex(index1) || Inventory22->Inventory[index1].InventoryType == EInventoryType::None)
		return false;
	FItemInventory ref = Inventory22->Inventory[index1];
	if (Equip->Equipments[static_cast<int32>(ref.EquipType)].Has)
	{
		UnEquipItem(ref.EquipType);
		//return false;
	}
		
	
	Equip->AddEquipByType(ref.Name, ref.ItemCode, ref.Texture, ref.EquipType);
	Inventory22->RemoveItemByIndex(index1);
	PDPlayerController->ItemInventory->Refresh();
	PDPlayerController->Equip->Refresh();
	return true;
}

bool AProjectDCharacter::UnEquipItem(EEquipType equipType)
{
	if (!Equip->Equipments[static_cast<int32>(equipType)].Has)
		return false;
	FEquip ref = Equip->Equipments[static_cast<int32>(equipType)];
	Inventory22->AddItem(ref.Name,ref.ItemCode, ref.Texture, EInventoryType::Weapon, ref.EquipType);
	Equip->RemoveEquipByType(equipType);
	PDPlayerController->ItemInventory->Refresh();
	PDPlayerController->Equip->Refresh();
	return false;
}

float AProjectDCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ChangeCurHp(Stat->CurHp - DamageAmount);
	UE_LOG(LogTemp, Log, TEXT("Player code[%d] CurHp : %f"), Stat->ActorCode, Stat->CurHp);
	return 0.0f;
}

void AProjectDCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = FinalStat.AtkRange;
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
		HitResult.Actor->TakeDamage(FinalStat.Atk, DamageEvent, GetController(), this);
	}
}

void AProjectDCharacter::StartDash()
{
	GetCharacterMovement()->MaxWalkSpeed = 2000.f;
	GetCharacterMovement()->MaxAcceleration = 20000.f;
	DashTimeline.PlayFromStart();
}

void AProjectDCharacter::StopDash()
{
}

void AProjectDCharacter::DashStart(float OutPut)
{
	AddMovementInput(GetCapsuleComponent()->GetForwardVector(), 1.0f, false);
}

void AProjectDCharacter::DashEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
}

void AProjectDCharacter::SpawnMonsterSample()
{
	FActorSpawnParameters spawnparam;
	UPDMonsterManager::Get()->FactoryMonsterSpawn(GetWorld(), FVector(-400.f, 1100.f, 350.f),FRotator(0.f,0.f,0.f), spawnparam);
	//UPDNormalMonsterManager::Get()->FactoryMonsterSpawn(GetWorld(), FVector(-400.f, 1100.f, 250.f), FRotator(0.f, 0.f, 0.f), spawnparam);
}


void AProjectDCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectDCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectDCharacter::Hit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AProjectDCharacter::WeaponMount(EWeaponType WT)
{
	if (CharacterWeaponType != EWeaponType::None)
	{
		if (ItemInfo)
		{
			ItemInfo->ItemBasicState = EItemBasicState::None;
			FinalStat.Atk -= ItemInfo->ItemStat->Atk;
			FinalStat.AtkRange -= ItemInfo->ItemStat->AtkRange;
			FinalStat.AtkSpeed -= ItemInfo->ItemStat->AtkSpeed;
			ItemInfo->Destroy();
		}
	}
	switch (WT)
	{
	case EWeaponType::None:
		break;
	case EWeaponType::Sword:
		ItemInfo = GetWorld()->SpawnActor <APDSword>();
		ItemInfo->ItemBasicState = EItemBasicState::Obtain;
		ItemInfo->ItemBox->SetCollisionProfileName("NoCollision");
		ItemInfo->ItemBasicState = EItemBasicState::Mount;
		FinalStat.Atk += ItemInfo->ItemStat->Atk;
		FinalStat.AtkRange += ItemInfo->ItemStat->AtkRange;
		FinalStat.AtkSpeed += ItemInfo->ItemStat->AtkSpeed;
		CharacterWeaponType = EWeaponType::Sword;
		break;
	case EWeaponType::Mace:
		ItemInfo = GetWorld()->SpawnActor <APDMace>();
		ItemInfo->ItemBasicState = EItemBasicState::Obtain;
		ItemInfo->ItemBox->SetCollisionProfileName("NoCollision");
		ItemInfo->ItemBasicState = EItemBasicState::Mount;
		FinalStat.Atk += ItemInfo->ItemStat->Atk;
		FinalStat.AtkRange += ItemInfo->ItemStat->AtkRange;
		FinalStat.AtkSpeed += ItemInfo->ItemStat->AtkSpeed;
		CharacterWeaponType = EWeaponType::Mace;
		break;
	default:
		break;
	}

	if (ItemInfo)
	{
		/*APDSword* ref = Cast<APDSword>(ItemInfo);
		ref->EquipmentMount();*/

		/*FName WeaponSocket(TEXT("hand_r_socket"));
		if (GetMesh()->DoesSocketExist(WeaponSocket))
		{
			ItemInfo->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		}*/
	}
}

void AProjectDCharacter::WeaponMountByEquipment()
{
	//���â 5 ���� �¿��� ���� ����. for�� �̿�
	for (int i = 0; i < 5; i++)
	{
		//������ �ڵ带 ���ؼ� csv���� �ε��ؿ���
		//���� ������ �ʿ��� ������ �� ��������
		if (Equip->Equipments[i].Has)
		{
			int32 TempItemCode = Equip->Equipments[i].ItemCode;
			FItemData* rowData = PDGameInstance->ItemData->FindRow<FItemData>(*FString::FromInt(TempItemCode), TEXT(""));
			if (ItemArray[i] != nullptr)
			{
				//���â�� �����۰� ĳ���Ͱ� �������� �������� ������ �Ǻ�
				//������ �ڵ� ���ؼ� ������ �Ǵ�
				//���� ������ ItemArray[i]�� ���� �� ���ο� ������ ����
				if (TempItemCode != ItemArray[i]->ItemCode)
				{
					ItemArray[i]->ItemBasicState = EItemBasicState::None;
					FinalStat.Atk -= ItemArray[i]->ItemStat->Atk;
					FinalStat.AtkRange -= ItemArray[i]->ItemStat->AtkRange;
					FinalStat.AtkSpeed -= ItemArray[i]->ItemStat->AtkSpeed;
					ItemArray[i]->Destroy();
					ItemArray[i] = nullptr;
					EquipmentMountByItemCode(TempItemCode, i);
				}
			}
			else
			{
				EquipmentMountByItemCode(TempItemCode, i);
				//������ ����
			}
		}
		else
		{
			if (ItemArray[i] != nullptr)
			{
				//���� ���� ������ ����
				ItemArray[i]->ItemBasicState = EItemBasicState::None;
				FinalStat.Atk -= ItemArray[i]->ItemStat->Atk;
				FinalStat.AtkRange -= ItemArray[i]->ItemStat->AtkRange;
				FinalStat.AtkSpeed -= ItemArray[i]->ItemStat->AtkSpeed;
				ItemArray[i]->Destroy();
				ItemArray[i] = nullptr;
			}
		}
	}
}

void AProjectDCharacter::EquipmentMountByItemCode(int32 itemCode, int32 EquipNum)
{
	switch (itemCode)
	{
	case 1:
		ItemArray[EquipNum] = GetWorld()->SpawnActor <APDSword>();
		ItemArray[EquipNum]->ItemBasicState = EItemBasicState::Obtain;
		ItemArray[EquipNum]->ItemBox->SetCollisionProfileName("NoCollision");
		ItemArray[EquipNum]->ItemBasicState = EItemBasicState::Mount;
		FinalStat.Atk += ItemArray[EquipNum]->ItemStat->Atk;
		FinalStat.AtkRange += ItemArray[EquipNum]->ItemStat->AtkRange;
		FinalStat.AtkSpeed += ItemArray[EquipNum]->ItemStat->AtkSpeed;
		CharacterWeaponType = EWeaponType::Sword;
		if (ItemArray[EquipNum])
		{
			APDSword* ref = Cast<APDSword>(ItemArray[EquipNum]);
			ref->EquipmentMount();
		}
		break;
	case 2:
		ItemArray[EquipNum] = GetWorld()->SpawnActor <APDMace>();
		ItemArray[EquipNum]->ItemBasicState = EItemBasicState::Obtain;
		ItemArray[EquipNum]->ItemBox->SetCollisionProfileName("NoCollision");
		ItemArray[EquipNum]->ItemBasicState = EItemBasicState::Mount;
		FinalStat.Atk += ItemArray[EquipNum]->ItemStat->Atk;
		FinalStat.AtkRange += ItemArray[EquipNum]->ItemStat->AtkRange;
		FinalStat.AtkSpeed += ItemArray[EquipNum]->ItemStat->AtkSpeed;
		CharacterWeaponType = EWeaponType::Mace;
		if (ItemArray[EquipNum])
		{
			APDMace* ref = Cast<APDMace>(ItemArray[EquipNum]);
			ref->EquipmentMount();
		}
		break;
	default:
		break;
	}
}

void AProjectDCharacter::ChangeCurHp(float curHp)
{
	if (curHp < 0)
	{
		Stat->CurHp = 0;
		PDPlayerController->BattleStatusWidget->SetHpBar(0.0f);
	}
	else if (curHp > Stat->MaxHp)
	{
		Stat->CurHp = Stat->MaxHp;
		PDPlayerController->BattleStatusWidget->SetHpBar(1.f);
	}
	else
	{
		Stat->CurHp = curHp;
		PDPlayerController->BattleStatusWidget->SetHpBar(Stat->CurHp / Stat->MaxHp);
	}
	
}

void AProjectDCharacter::OnResetVR()
{
	// If ProjectD is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ProjectD.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProjectDCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AProjectDCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProjectDCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (MouseInputValid)
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectDCharacter::MyTurnAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerYawInput(Rate);
}

void AProjectDCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (MouseInputValid)
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectDCharacter::MyLookUpAtRate(float Rate)
{
	if (MouseInputValid)
		AddControllerPitchInput(Rate);
}

void AProjectDCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AProjectDCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
