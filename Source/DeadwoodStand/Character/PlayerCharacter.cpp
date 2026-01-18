// @ 2026 Brevis. Development Build. All rights reserved.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	// GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting && CurrentStamina > 0.0f)
	{
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		if (CurrentStamina < 0.0f)
		{
			CurrentStamina = 0.0f;
			StopSprint();
		}
	}
	else if (bCanRecoverStamina)
	{
		CurrentStamina = FMath::Clamp(CurrentStamina + StaminaRecoveryRate * DeltaTime, 0.0f, MaxStamina);
	}

	if (GEngine)
	{
		const FString StaminaString = FString::Printf(TEXT("Stamina: %.1f"), CurrentStamina);
		GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Green, StaminaString);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::StopJump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &APlayerCharacter::StopSprint);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::ToggleCrouch);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (const FVector2D MovementVector = Value.Get<FVector2D>(); Controller && MovementVector.SizeSquared() >
		0.0f)
	{
		const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementVector.Y);
		AddMovementInput(Right, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(-LookAxis.Y);
}

void APlayerCharacter::StartJump()
{
	Jump();
}

void APlayerCharacter::StopJump()
{
	StopJumping();
}

void APlayerCharacter::StartSprint()
{
	if (CurrentStamina > 0.0f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::EnableStaminaRecovery()
{
	bCanRecoverStamina = true;
}

void APlayerCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	bCanRecoverStamina = false;

	GetWorldTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
	GetWorldTimerManager().SetTimer(StaminaRecoveryTimerHandle, this,
	                                &APlayerCharacter::EnableStaminaRecovery, StaminaRecoveryDelay, false);
}

void APlayerCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}
