// Fill out your copyright notice in the Description page of Project Settings.


#include "PDManCharacter.h"
#include "PDCharacter1AInstance.h"

APDManCharacter::APDManCharacter()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/DownloadAsset/ImportAsset/Character/Man2/Man2.Man2"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	}
	ConstructorHelpers::FClassFinder<UPDCharacter1AInstance> AM(TEXT("AnimBlueprint'/Game/BluePrint/AnimInstance/Man/BPMan2Instance.BPMan2Instance_C'"));
	if (AM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AM.Class);
	}
	IsAttacking = false;
	MaxCombo = 3;
	ComboStateReset();
}

void APDManCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APDManCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APDManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APDManCharacter::PlayAttack);
}

void APDManCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PDCharacter1AInstance = Cast<UPDCharacter1AInstance>(GetMesh()->GetAnimInstance());

	PDCharacter1AInstance->OnMontageEnded.AddDynamic(this, &APDManCharacter::OnAttackMontageEnded);
	PDCharacter1AInstance->OnAttackHit.AddUObject(this, &APDManCharacter::AttackCheck);

	PDCharacter1AInstance->OnAttackNext.AddLambda([this]()->void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			NextComboStartState();
			PDCharacter1AInstance->JumpToSAttackMontageSectionName(ComboCount);
		}
		});
}

void APDManCharacter::PlayAttack()
{
	if (!MouseInputValid)
	{
		return;
	}

	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		IsAttacking = true;
		NextComboStartState();
		PDCharacter1AInstance->PlaySAttackMontage(1.0f);
		PDCharacter1AInstance->JumpToSAttackMontageSectionName(ComboCount);
	}
}

void APDManCharacter::NextComboStartState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ComboCount = FMath::Clamp<int32>(ComboCount + 1, 1, MaxCombo);
}

void APDManCharacter::ComboStateReset()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	ComboCount = 0;
}

void APDManCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruped)
{
	IsAttacking = false;
	ComboStateReset();
}
