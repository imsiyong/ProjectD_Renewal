// Fill out your copyright notice in the Description page of Project Settings.


#include "PDCharacter1AInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPDCharacter1AInstance::UPDCharacter1AInstance()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAM(TEXT("/Game/BluePrint/AnimInstance/Man/Man2Montage/MT_SAttack.MT_SAttack"));
	if (AttackAM.Succeeded())
	{
		AttackMontage = AttackAM.Object;
	}
}

void UPDCharacter1AInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Pawn = TryGetPawnOwner();
	if (Pawn == nullptr)
		return;
	Character = Cast<ACharacter>(Pawn);
	if (Character == nullptr)
		return;
}

void UPDCharacter1AInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Pawn == nullptr)
		return;
	CharacterSpeed = Pawn->GetVelocity().Size();
	IsInAir = Character->GetCharacterMovement()->IsFalling();
}

void UPDCharacter1AInstance::PlaySAttackMontage(float atkSpeed)
{
	if (AttackMontage && !Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, atkSpeed);
	}
}

void UPDCharacter1AInstance::JumpToSAttackMontageSectionName(int32 section)
{
	Montage_JumpToSection(GetAttackMontageSectionName(section), AttackMontage);
}

FName UPDCharacter1AInstance::GetAttackMontageSectionName(int32 num)
{
	return FName(*FString(TEXT("Attack")), num+1);
}

void UPDCharacter1AInstance::AnimNotify_AttackHitCheck()
{
	if (OnAttackHit.IsBound())
	{
		OnAttackHit.Broadcast();
	}
}

void UPDCharacter1AInstance::AnimNotify_AttackNextCheck()
{
	if (OnAttackNext.IsBound())
	{
		OnAttackNext.Broadcast();
	}
}

