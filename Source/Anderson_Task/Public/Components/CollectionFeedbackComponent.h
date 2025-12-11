#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectionFeedbackComponent.generated.h"

class UArtifactData;
class UAudioComponent;
class UNiagaraComponent;
class UParticleSystemComponent;

/**
 * @brief Component to handle feedback effects when an artifact is collected.
 * @details Plays audio and visual effects based on the provided artifact data.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANDERSON_TASK_API UCollectionFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 
	 * @brief Plays feedback effects based on the artifact data.
	 * 
	 * @param Data - The artifact data containing feedback properties.
	 */
	void PlayFeedback(const UArtifactData* Data);
	
	/** 
	 * @brief Stops any ongoing feedback effects.
	 */
	void StopFeedback();

private:
	/** 
	 * @brief Schedules stopping of feedback effects after a duration.
	 * 
	 * @param Duration - Time in seconds after which to stop the effects.
	 */
	void ScheduleStop(float Duration);

protected:
	UPROPERTY()
	UAudioComponent* AudioComp = nullptr;

	UPROPERTY()
	UNiagaraComponent* NiagaraComp = nullptr;

	UPROPERTY()
	UParticleSystemComponent* ParticleComp = nullptr;

	FTimerHandle StopTimerHandle;
};
