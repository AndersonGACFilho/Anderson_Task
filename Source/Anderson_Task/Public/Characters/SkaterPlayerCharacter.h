#pragma once

#include "CoreMinimal.h"
#include "Characters/SkaterCharacterBase.h"
#include "SkaterPlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * @brief Player Character class for the Skater game.
 * @details This class handles player input, camera setup, and skater-specific movement logic.
 * It uses Enhanced Input for handling player controls and provides properties for animation state.
 */
UCLASS(Config = Game)
class ASkaterPlayerCharacter : public ASkaterCharacterBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for ASkaterPlayerCharacter.
	 * @details Sets default values for this character's properties, including camera setup and input actions
	 * using Enhanced Input.
	 */
	ASkaterPlayerCharacter();

protected:
	/** 
	 * @brief Notifies when the controller has changed.
	 * @details Sets up the input mapping context for the new controller.
	 */
	virtual void NotifyControllerChanged() override;
	
	/** 
	 * @brief Sets up player input component.
	 * @details Binds input actions to their respective handler functions.
	 * @param PlayerInputComponent - The input component to set up.
	 */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	/** 
	 * @brief Handles movement input.
	 * @details Processes the input action value to determine movement direction and magnitude.
	 * 
	 * @param Value - The input action value containing movement data.
	 */
	void HandleMoveInput(const FInputActionValue& Value);

	/** 
	 * @brief Handles movement input release.
	 * @details Resets the movement input vector when the input action is released.
	 */
	void HandleMoveInputReleased();
	
	/** 
	 * @brief Handles look input.
	 * @details Processes the input action value to adjust the character's view direction.
	 * 
	 * @param Value - The input action value containing look data.
	 */
	void HandleLookInput(const FInputActionValue& Value);

protected:

	// Input Actions & Mapping Contexts ------------------------------
	// Default mapping context for the character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// Input actions -------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Look",
		meta = (ClampMin = "0.0"))
	float LookSensitivity = 1.f;
};