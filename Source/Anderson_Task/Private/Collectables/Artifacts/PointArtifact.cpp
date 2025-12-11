#include "Collectables/Artifacts/PointArtifact.h"

#include "Collectables/DataAssets/ArtifactData.h"
#include "Components/CollectionFeedbackComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/PointSystem.h"


APointArtifact::APointArtifact()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true; 

    // Collision sphere
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->InitSphereRadius(50.f);
    CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APointArtifact::OnSphereBeginOverlap);

    // Mesh
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(CollisionSphere);
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Feedback component
    FeedbackComponent = CreateDefaultSubobject<UCollectionFeedbackComponent>(TEXT("FeedbackComponent"));
}

void APointArtifact::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    SetReplicateMovement(false);

    if (ArtifactData)
    {
        if (UStaticMesh* Mesh = ArtifactData->Mesh)
        {
            MeshComponent->SetStaticMesh(Mesh);
        }

        if (UMaterialInterface* Material = ArtifactData->Material)
        {
            MeshComponent->SetMaterial(0, Material);
        }
    }
}

void APointArtifact::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor)
        return;

    if (CanBeCollected_Implementation(OtherActor))
        OnCollected_Implementation(OtherActor);
}

IPointSystem* APointArtifact::FindPointSystemInActor(AActor* Actor)
{
    if (!Actor) return nullptr;

    if (IPointSystem* PS = Cast<IPointSystem>(Actor))
        return PS;

    if (UActorComponent* PSComponent = Actor->FindComponentByInterface(UPointSystem::StaticClass()))
        return Cast<IPointSystem>(PSComponent);

    if (const APawn* Pawn = Cast<APawn>(Actor))
        return Cast<IPointSystem>(Pawn->GetPlayerState());

    return nullptr;
}

bool APointArtifact::OnCollected_Implementation(AActor* Collector)
{
    if (!bIsActive || !ArtifactData || !Collector)
        return false;
    
    IPointSystem* PointSystem = FindPointSystemInActor(Collector);
    if (!PointSystem) 
        return false;

    const int32 PointValue = Execute_GetPointValue(this);
    PointSystem->Execute_AddPoints(Cast<UObject>(PointSystem), PointValue);

    bIsActive = false;

    if (FeedbackComponent)
    {
        FeedbackComponent->PlayFeedback(ArtifactData);
    }

    if (!ArtifactData->bIsToPersistAfterCollection)
        Destroy();
    else if(MeshComponent)
    {
        if (UMaterialInstanceDynamic* DynMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0))
        {
            DynMaterial->SetScalarParameterValue(TEXT("Opacity"), ArtifactData->OpacityAfterCollection);
        }
    }

    return true;
}

bool APointArtifact::CanBeCollected_Implementation(const AActor* Collector) const
{
    return bIsActive && 
           Collector && 
           Collector->IsA<APawn>() &&
           HasAuthority();
}

int32 APointArtifact::GetPointValue_Implementation() const
{
    return ArtifactData ? ArtifactData->PointValue : 0;
}

FText APointArtifact::GetArtifactName_Implementation() const
{
    return ArtifactData ? ArtifactData->ArtifactName : FText::GetEmpty();
}

bool APointArtifact::IsActive_Implementation() const
{
    return bIsActive;
}
