#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PointSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPointsChanged, int32, OldPoints, int32, NewPoints, int32, Delta);

UINTERFACE(MinimalAPI, Blueprintable)
class UPointSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for managing player points/score.
 * @details Provides a contract for adding, retrieving, and resetting points.
 * Should be implemented by PlayerState or GameMode classes.
 * Follows SOLID principles by separating point management concerns.
 */
class ANDERSON_TASK_API IPointSystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief Adds points to the player's score.
	 * @param Points - Number of points to add (can be negative for deduction).
	 * @return The new total points after addition.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Points")
	int32 AddPoints(int32 Points);

	/**
	 * @brief Gets the current point total.
	 * @return Current points.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Points")
	int32 GetPoints() const;

	/**
	 * @brief Resets points to zero.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Points")
	void ResetPoints();

	/**
	 * @brief Checks if points can be deducted.
	 * @param Points - Number of points to check.
	 * @return true if the player has enough points, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Points")
	bool CanAffordPoints(int32 Points) const;
};
