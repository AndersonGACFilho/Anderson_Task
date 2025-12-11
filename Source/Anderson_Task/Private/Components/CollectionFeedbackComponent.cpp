#include "Components/CollectionFeedbackComponent.h"
#include "Collectables/DataAssets/ArtifactData.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"

void UCollectionFeedbackComponent::PlayFeedback(const UArtifactData* Data)
{
    if (!Data || !GetWorld())
        return;

    StopFeedback();

    if (USoundBase* Sound = Data->CollectionSound)
    {
        AudioComp = UGameplayStatics::SpawnSoundAtLocation(this, Sound, 
            GetOwner()->GetActorLocation());
    }

    if (UFXSystemAsset* VFXAsset = Data->CollectionEffect)
    {
        if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(VFXAsset))
        {
            NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem,
                GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
        }
        else if (UParticleSystem* ParticleSystem = Cast<UParticleSystem>(VFXAsset))
        {
            ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, 
                GetOwner()->GetActorTransform());
        }
    }

    if (Data->EffectDuration > 0.f)
    {
        ScheduleStop(Data->EffectDuration);
    }
}

void UCollectionFeedbackComponent::StopFeedback()
{
    if (AudioComp)
    {
        AudioComp->Stop();
        AudioComp->DestroyComponent();
        AudioComp = nullptr;
    }

    if (NiagaraComp)
    {
        NiagaraComp->Deactivate();
        NiagaraComp->DestroyComponent();
        NiagaraComp = nullptr;
    }

    if (ParticleComp)
    {
        ParticleComp->DeactivateSystem();
        ParticleComp->DestroyComponent();
        ParticleComp = nullptr;
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StopTimerHandle);
    }
}

void UCollectionFeedbackComponent::ScheduleStop(float Duration)
{
    if (!GetWorld())
        return;

    GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, this,
        &UCollectionFeedbackComponent::StopFeedback, Duration, false);
}
