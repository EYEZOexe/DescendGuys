
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

UCLASS()
class DESCENDGUYS_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerBase();

protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// COMPONENTS ___________________________________
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// MOVEMENT _____________________________________
	void MoveForward(float Value);

	void MoveRight(float Value);

	virtual void Jump() override;


};
