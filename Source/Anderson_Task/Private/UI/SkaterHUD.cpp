#include "UI/SkaterHUD.h"
#include "Components/TextBlock.h"
#include "PlayerStates/SkaterPlayerState.h"
#include "Characters/SkaterCharacterBase.h"

void USkaterHUD::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (!PointsText)
        UE_LOG(LogTemp, Error, TEXT("SkaterHUD::NativeConstruct - PointsText is NULL! Check Widget Blueprint binding"));
    
    if (!SpeedText)
        UE_LOG(LogTemp, Error, TEXT("SkaterHUD::NativeConstruct - SpeedText is NULL! Check Widget Blueprint binding"));


    const APlayerController* PC = GetOwningPlayer();
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("SkaterHUD::NativeOnInitialized - PlayerController is NULL"));
        return;
    }

    ASkaterCharacterBase* Character = Cast<ASkaterCharacterBase>(PC->GetPawn());
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("SkaterHUD::NativeOnInitialized - Character is NULL"));
        return;
    }

    ASkaterPlayerState* PS = PC->GetPlayerState<ASkaterPlayerState>();
    if (!PS)
    {
        UE_LOG(LogTemp, Error, TEXT("SkaterHUD::NativeOnInitialized - PlayerState is NULL"));
        return;
    }

    CachedPlayerCharacter = Character;
    CachedPlayerState = PS;

    BindToPlayerState();
}

void USkaterHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!CachedPlayerCharacter.IsValid())
        return;

    const float SpeedPercent = CachedPlayerCharacter->GetSpeedPercent();
    UpdateSpeed(SpeedPercent);
}

void USkaterHUD::BindToPlayerState()
{
    if (!CachedPlayerState.IsValid())
    {
        return;
    }

    CachedPlayerState->OnPointsChanged.AddDynamic(this, &USkaterHUD::OnPointsChangedHandler);
    UpdatePoints(CachedPlayerState->GetPoints_Implementation());
}

void USkaterHUD::OnPointsChangedHandler(int32 OldPoints, int32 NewPoints, int32 Delta)
{
    UpdatePoints(NewPoints);
}

void USkaterHUD::UpdatePoints(int32 NewPoints) const
{
    if (!PointsText)
    {
        return;
    }

    FFormatNamedArguments Args;
    Args.Add(TEXT("0"), NewPoints);
    const FText FormattedText = FText::Format(PointsFormat, Args);
    PointsText->SetText(FormattedText);

}

void USkaterHUD::UpdateSpeed(float SpeedPercent) const
{
    if (!SpeedText)
    {
        return;
    }

    const int32 SpeedInt = FMath::RoundToInt(SpeedPercent * 100.f);
    FFormatNamedArguments Args;
    Args.Add(TEXT("0"), SpeedInt);
    const FText FormattedText = FText::Format(SpeedFormat, Args);
    SpeedText->SetText(FormattedText);
}
