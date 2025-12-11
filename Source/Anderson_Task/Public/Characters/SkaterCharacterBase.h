#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkaterCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogSkaterCharacter, Log, All);

/**
 * Defines the current movement state of the skater.
 * Used to determine braking behavior and animation states.
 */
UENUM(BlueprintType)
enum class ESkaterMovementState : uint8
{
	Coasting      UMETA(DisplayName = "Coasting"),
	Accelerating  UMETA(DisplayName = "Accelerating"),
	Braking       UMETA(DisplayName = "Braking")
};

/**
 * @brief Base class for all skater characters (players and bots).
 * @details Contains shared movement logic, components, and state management.
 * Derived classes implement input sources (player input or AI).
 */
UCLASS(Abstract)
class ASkaterCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for ASkaterCharacterBase. 
	 * @details Sets default values for this character's properties, including camera setup.
	 */
	ASkaterCharacterBase();

	/** 
	 * @brief Called every frame.
	 * @details Applies skater-specific movement logic each frame.
	 * 
	 * @param DeltaTime - Time elapsed since the last tick.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Called after the components have been initialized.
	 * @details Caches the CharacterMovementComponent for efficient access during gameplay.
	 */
	virtual void PostInitializeComponents() override;

	/** 
	 * @brief Gets the camera boom component.
	 * @return The camera boom component.
	 */
	UFUNCTION(BlueprintPure, Category = "Skater|Components")
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** 
	 * @brief Gets the follow camera component.
	 * @return The follow camera component.
	 */
	UFUNCTION(BlueprintPure, Category = "Skater|Components")
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** 
	 * @brief Gets the current movement state.
	 * @return The current movement state.
	 */
	UFUNCTION(BlueprintPure, Category = "Skater|State")
	FORCEINLINE ESkaterMovementState GetMovementState() const { return CurrentMovementState; }

	/** 
	 * @brief Gets the current speed as a percentage of max speed.
	 * @return Speed percentage (0.0 to 1.0).
	 */
	UFUNCTION(BlueprintPure, Category = "Skater|State")
	float GetSpeedPercent() const;

	/** 
	 * @brief Gets the current speed in units per second.
	 * @return Current speed.
	 */
	UFUNCTION(BlueprintPure, Category = "Skater|State")
	float GetCurrentSpeed() const;

protected:
	/** 
	 * Current movement input vector.
	 * X = steering (-1 left, +1 right)
	 * Y = acceleration (+1 forward, -1 brake)
	 * Set by derived classes (player input or AI).
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D CurrentInputVector = FVector2D::ZeroVector;

	/**
	 * @brief Sets the movement input vector directly.
	 * @details Use this for AI control or external input sources.
	 * 
	 * @param NewInput - X for steering, Y for acceleration/braking
	 */
	UFUNCTION(BlueprintCallable, Category = "Skater|Input")
	void SetMovementInput(FVector2D NewInput);

	/**
	 * @brief Clears all movement input.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skater|Input")
	void ClearMovementInput();

	/**
	 * @brief Sets the movement state and applies appropriate deceleration.
	 * 
	 * @param NewState - The new movement state
	 */
	UFUNCTION(BlueprintCallable, Category = "Skater|State")
	void SetMovementState(ESkaterMovementState NewState);

	/** 
	 * @brief Called before movement update each frame.
	 * @details Can be overridden by derived classes for custom pre-movement logic.
	 * 
	 * @param DeltaTime - Time elapsed since the last tick.
	 */
	virtual void PreMovementUpdate(float DeltaTime) {}

	/** 
	 * @brief Called after movement update each frame.
	 * @details Can be overridden by derived classes for custom post-movement logic.
	 * 
	 * @param DeltaTime - Time elapsed since the last tick.
	 */
	virtual void PostMovementUpdate(float DeltaTime) {}

private:
	/** 
	 * @brief Applies skater-specific movement logic.
	 * @details Handles steering interpolation, rotation, acceleration, and braking.
	 * Updates animation state flags and adjusts braking deceleration based on input.
	 * 
	 * @param DeltaTime - Time elapsed since the last tick.
	 */
	void UpdateSkaterMovement(float DeltaTime);

	/**
	 * @brief Processes steering input.
	 * @details Interpolates the current turn value based on input and applies rotation to the 
	 * character.
	 * 
	 * @param DeltaTime - Time elapsed since the last tick.
	 */
	void ProcessSteering(float DeltaTime);

	/**
	 * @brief Processes acceleration and braking input.
	 * @details Adjusts movement input, braking deceleration, and animation state flags based 
	 * on input.
	 */
	void ProcessAcceleration();
	
	/** 
	 * @brief Gets the cached CharacterMovementComponent.
	 * @return The cached CharacterMovementComponent, or nullptr if not valid.
	 */
	FORCEINLINE UCharacterMovementComponent* GetCachedMovementComponent() const
	{
		return CachedMovementComponent.IsValid() ? CachedMovementComponent.Get() : nullptr;
	}

public:
	// Animation state properties
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	ESkaterMovementState CurrentMovementState = ESkaterMovementState::Coasting;

protected:
	// Components ----------------------------------------------------
	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	// Skateboard mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	TObjectPtr<UStaticMeshComponent> SkateboardMesh;

	// Movement properties -------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Speed", 
		meta = (ClampMin = "0.0"))
	float MaxSkateSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Speed", 
		meta = (ClampMin = "0.0"))
	float BaseGroundFriction = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Deceleration",
		meta = (ClampMin = "0.0"))
	float AccelerateDeceleration = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Deceleration",
		meta = (ClampMin = "0.0"))
	float BrakeDeceleration = 2048.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Deceleration",
		meta = (ClampMin = "0.0"))
	float CoastDeceleration = 50.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Steering", 
		meta = (ClampMin = "0.0"))
	float TurnRate = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement|Steering", 
		meta = (ClampMin = "0.0"))
	float SteeringInterpSpeed = 5.0f;

	// Camera properties ---------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", 
		meta = (ClampMin = "0.0"))
	float CameraArmLength = 300.f;

private:
	// Cached CharacterMovementComponent
	TWeakObjectPtr<UCharacterMovementComponent> CachedMovementComponent;

	// Minimum turn value to apply rotation (prevents micro-jitter)
	static constexpr float TurnDeadzone = 0.01f;

	// Minimum forward input to register as acceleration/braking
	static constexpr float InputDeadzone = 0.1f;

	// Current turn value for steering interpolation
	float CurrentTurnValue = 0.f;
};