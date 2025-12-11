#include "Characters/SkaterPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

ASkaterPlayerCharacter::ASkaterPlayerCharacter()
{
}

void ASkaterPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	const APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC)
	{
		UE_LOG(LogSkaterCharacter, Warning, TEXT("%s: No PlayerController found"), *GetName());
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem)
	{
		UE_LOG(LogSkaterCharacter, Warning, TEXT("%s: EnhancedInputLocalPlayerSubsystem not found"), *GetName());
		return;
	}

	if (!DefaultMappingContext)
	{
		UE_LOG(LogSkaterCharacter, Warning, TEXT("%s: DefaultMappingContext is not set"), *GetName());
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void ASkaterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		UE_LOG(LogSkaterCharacter, Warning, TEXT("%s: EnhancedInputComponent not found"), *GetName());
		return;
	}

	// Jump
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, 
		&ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, 
		&ACharacter::StopJumping);

	// Movement
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, 
		&ASkaterPlayerCharacter::HandleMoveInput);
	Input->BindAction(MoveAction, ETriggerEvent::Completed, this, 
		&ASkaterPlayerCharacter::HandleMoveInputReleased);

	// Look
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, 
		&ASkaterPlayerCharacter::HandleLookInput);
}

void ASkaterPlayerCharacter::HandleMoveInput(const FInputActionValue& Value)
{
	SetMovementInput(Value.Get<FVector2D>());
}

void ASkaterPlayerCharacter::HandleMoveInputReleased()
{
	ClearMovementInput();
}

void ASkaterPlayerCharacter::HandleLookInput(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X * LookSensitivity);
	AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
}