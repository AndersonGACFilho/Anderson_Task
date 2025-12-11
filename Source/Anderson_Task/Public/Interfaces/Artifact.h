#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Artifact.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UArtifact : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface for artifact that provide points.
 * @details Defines the contract for artifacts in the points system.
 * Follows SOLID principles by providing a clear, focused contract.
 */
class ANDERSON_TASK_API IArtifact
{
	GENERATED_BODY()

public:
	/**
	 * @brief Gets the point value of this artifact.
	 * @return The number of points awarded when collected.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Artifact")
	int32 GetPointValue() const;

	/**
	 * @brief Gets the display name of the artifact.
	 * @return The name shown to the player.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Artifact")
	FText GetArtifactName() const;

	/**
	 * @brief Checks if the artifact is currently active/valid.
	 * @details Allows for temporary or conditional artifacts.
	 * @return true if the artifact can award points, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Artifact")
	bool IsActive() const;
};
