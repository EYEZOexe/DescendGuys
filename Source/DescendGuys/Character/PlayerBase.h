
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "PlayerBase.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class DESCENDGUYS_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerBase();

protected:

	virtual void BeginPlay() override;

	// Input ________________________________________

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerBaseContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpingAction;

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

	virtual void Jump() override;

	void Move(const FInputActionValue& Value);

};
