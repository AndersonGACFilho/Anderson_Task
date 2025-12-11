#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Artifact.h"
#include "Interfaces/Collectable.h"
#include "PointArtifact.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UArtifactData;
class UCollectionFeedbackComponent;
class IPointSystem;

/**
 * @brief Actor representing a collectable point artifact.
 * @details Data-driven design - configure artifact properties via Data Asset.
 */
UCLASS()
class ANDERSON_TASK_API APointArtifact : public AActor, public ICollectable, public IArtifact
{
	GENERATED_BODY()

public:
	APointArtifact();

protected:
	virtual void PostInitializeComponents() override;

	// ICollectable interface
	/**
	 * @brief Handles the collection of the artifact.
	 * @details Plays feedback effects and marks the artifact as inactive.
	 * 
	 * @param Collector The actor that collected the artifact.
	 * @return true if collection was successful, false otherwise.
	 */
	virtual bool OnCollected_Implementation(AActor* Collector) override;

	/**
	 * @brief Checks if the artifact can be collected by the given actor.
	 * @details Validates if the artifact is active before allowing collection.
	 * 
	 * @param Collector The actor attempting to collect.
	 * @return true if the artifact can be collected, false otherwise.
	 */
	virtual bool CanBeCollected_Implementation(const AActor* Collector) const override;

	// IArtifact interface
	/**
	 * @brief Gets the point value of this artifact.
	 * @return The number of points awarded when collected.
	 */
	virtual int32 GetPointValue_Implementation() const override;

	/**
	 * @brief Gets the display name of the artifact.
	 * @return The name shown to the player.
	 */
	virtual FText GetArtifactName_Implementation() const override;

	/**
	 * @brief Checks if the artifact is currently active/valid.
	 * @details Allows for temporary or conditional artifacts.
	 * @return true if the artifact can award points, false otherwise.
	 */
	virtual bool IsActive_Implementation() const override;

private:
	/**
	 * @brief Handler for when another actor begins overlapping the artifact's collision sphere.
	 * @details Initiates the collection process if the overlapping actor can collect the artifact.
	 * 
	 * @param OverlappedComponent The component that was overlapped.
	 * @param OtherActor The actor that initiated the overlap.
	 * @param OtherComp The specific component of the other actor that was involved in the overlap.
	 * @param OtherBodyIndex The body index of the other component.
	 * @param bFromSweep Whether the overlap was the result of a sweep.
	 * @param SweepResult Details about the hit result if bFromSweep is true.
	 */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief Finds the IPointSystem interface in the given actor.
	 * @details Searches the actor for an implementation of the IPointSystem interface.
	 * 
	 * @param Actor - The actor to search for a point system.
	 * @return The IPointSystem interface if found, nullptr otherwise.
	 */
	static IPointSystem* FindPointSystemInActor(AActor* Actor);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCollectionFeedbackComponent> FeedbackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifact")
	TObjectPtr<UArtifactData> ArtifactData;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsActive = true;
};
