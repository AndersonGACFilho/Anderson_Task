#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArtifactData.generated.h"

class UStaticMesh;
class UMaterialInterface;
class USoundBase;
class UFXSystemAsset;

/**
 * @brief Data asset defining artifact properties.
 * @details Data-driven approach - create different assets for each artifact type.
 */
UCLASS(BlueprintType)
class ANDERSON_TASK_API UArtifactData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * @brief Gets the primary asset ID for this artifact data.
	 * @return The primary asset ID.
	 */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	// Visual and audio properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UMaterialInterface> Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual")
	bool bIsToPersistAfterCollection = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visual", 
		meta = (EditCondition = "bIsToPersistAfterCollection", ClampMin = "0.0", ClampMax = "1.0"))
	float OpacityAfterCollection = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<USoundBase> CollectionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UFXSystemAsset> CollectionEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX", 
		meta = (ClampMin = "0.0"))
	float EffectDuration;

	// Artifact properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Artifact")
	FText ArtifactName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Artifact", meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Artifact")
	int32 PointValue = 100;
};
