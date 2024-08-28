// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerBase.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerBase>(TryGetPawnOwner());

}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacter == nullptr) 
	{

		PlayerCharacter = Cast<APlayerBase>(TryGetPawnOwner());

	}

	if (PlayerCharacter == nullptr) return;

	// VELOCITY _____________________________________

	FVector Velocity = PlayerCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

}
