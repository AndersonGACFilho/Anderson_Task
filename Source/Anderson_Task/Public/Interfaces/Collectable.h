#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Collectable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCollectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for collectable items in the game.
 * @details Classes that implement this interface can be collected by players or other entities.
 * Follows SOLID principles by providing a clean contract for collection behavior.
 */
class ANDERSON_TASK_API ICollectable
{
	GENERATED_BODY()

public:
	/**
	 * @brief Function to handle the collection of the item.
	 * @details Implement this function to define what happens when the item is collected.
	 *
	 * @param Collector - The actor that collected the item (more flexible than Pawn).
	 * @return true if collection was successful, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collectable")
	bool OnCollected(AActor* Collector);
    
	/**
	 * @brief Checks if the item can be collected by the given actor.
	 * @details Optional function to add validation logic before collection.
	 *
	 * @param Collector - The actor attempting to collect.
	 * @return true if the item can be collected, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collectable")
	bool CanBeCollected(const AActor* Collector) const;
};
