#include "PlayerStates/SkaterPlayerState.h"
#include "Net/UnrealNetwork.h"

ASkaterPlayerState::ASkaterPlayerState()
{
	CurrentPoints = 0;
}

void ASkaterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASkaterPlayerState, CurrentPoints);
}

int32 ASkaterPlayerState::AddPoints_Implementation(int32 Points)
{
	// Only the server should modify points
	if (!HasAuthority())
		return CurrentPoints;

	const int32 OldPoints = CurrentPoints;
	CurrentPoints = FMath::Max(0, CurrentPoints + Points);

	OnPointsChanged.Broadcast(OldPoints, CurrentPoints, CurrentPoints - OldPoints);
	return CurrentPoints;
}

int32 ASkaterPlayerState::GetPoints_Implementation() const
{
	return CurrentPoints;
}

void ASkaterPlayerState::ResetPoints_Implementation()
{
	AddPoints(-CurrentPoints);
}

bool ASkaterPlayerState::CanAffordPoints_Implementation(int32 Points) const
{
	return (CurrentPoints - Points) >= 0;
}

void ASkaterPlayerState::OnRep_CurrentPoints(int32 OldPoints)
{
    OnPointsChanged.Broadcast(OldPoints, CurrentPoints, CurrentPoints - OldPoints);
}
