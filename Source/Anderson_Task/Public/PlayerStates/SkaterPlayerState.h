#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PointSystem.h"
#include "SkaterPlayerState.generated.h"

/**
 * @brief The PlayerState class for the Skater game.
 * @details Manages player-specific data such as points. 
 * Implements the IPointSystem interface.
 * Follows SOLID principles by separating point management into its own class.
 */
UCLASS()
class ANDERSON_TASK_API ASkaterPlayerState : public APlayerState, public IPointSystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for ASkaterPlayerState.
	 * @details Initializes default values for the player state.
	 */
	ASkaterPlayerState();

	/**
	 * @brief Adds points to the player's total.
	 * 
	 * @param Points - Number of points to add (can be negative).
	 * @return The new total points.
	 */
    virtual int32 AddPoints_Implementation(int32 Points) override;

	/**
	 * @brief Gets the current point total.
	 * 
	 * @return Current points.
	 */
    virtual int32 GetPoints_Implementation() const override;

	/**
	 * @brief Resets points to zero.
	 */
    virtual void ResetPoints_Implementation() override;

	/**
	 * @brief Checks if the player can afford a certain number of points.
	 *
	 * @param Points - Number of points to check affordability for.
	 * @return true if the player can afford the points, false otherwise.
	 */
	virtual bool CanAffordPoints_Implementation(int32 Points) const override;

protected:
	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Delegate for point changes
	UPROPERTY(BlueprintAssignable, Category = "Points")
	FOnPointsChanged OnPointsChanged;

private:
	// Current points
	UPROPERTY(ReplicatedUsing = OnRep_CurrentPoints)
	int32 CurrentPoints = 0;

	// Replication notification for CurrentPoints
	UFUNCTION()
	void OnRep_CurrentPoints(int32 OldPoints);
};
