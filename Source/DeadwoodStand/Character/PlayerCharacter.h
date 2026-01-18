// @ 2026 Brevis. Development Build. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class DEADWOODSTAND_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void ToggleCrouch();

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* CrouchAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* Camera;

private:
	float WalkSpeed = 600.0f;
	float SprintSpeed = 900.0f;

	bool bIsSprinting;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float StaminaDrainRate = 25.0f;

	UPROPERTY(EditAnywhere, Category="Stamina")
	float StaminaRecoveryRate = 15.0f;

	FTimerHandle StaminaRecoveryTimerHandle;

	bool bCanRecoverStamina = true;

	UPROPERTY(EditDefaultsOnly, Category="Stamina")
	float StaminaRecoveryDelay = 10.0f;

	void EnableStaminaRecovery();
};
