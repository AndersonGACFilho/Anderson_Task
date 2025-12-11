#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkaterHUD.generated.h"

class UTextBlock;
class ASkaterPlayerState;
class ASkaterCharacterBase;

/**
 * @brief Main HUD widget for displaying game information.
 * @details Displays player points and speed. Binds to PlayerState for point updates.
 */
UCLASS()
class ANDERSON_TASK_API USkaterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Updates the displayed points value.
	 * @param NewPoints - The new point value to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdatePoints(int32 NewPoints) const;

	/**
	 * @brief Updates the displayed speed value.
	 * @param SpeedPercent - The speed as a percentage (0-1).
	 */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateSpeed(float SpeedPercent) const;

protected:
	/**
	 * @brief Called when the widget is constructed.
	 */
	virtual void NativeConstruct() override;

	/**
	 * @brief Called every frame.
	 * 
	 * @param MyGeometry - The geometry of the widget.
	 * @param InDeltaTime - Time elapsed since the last tick.
	 */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	/**
	 * @brief Subscribes to point change events from the player state.
	 */
	void BindToPlayerState();

	/**
	 * @brief Handler for point changes.
	 */
	UFUNCTION()
	void OnPointsChangedHandler(int32 OldPoints, int32 NewPoints, int32 Delta);

protected:
	TWeakObjectPtr<ASkaterPlayerState> CachedPlayerState = nullptr;

	TWeakObjectPtr<ASkaterCharacterBase> CachedPlayerCharacter = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "HUD", meta = (BindWidget))
	UTextBlock* PointsText;

	UPROPERTY(BlueprintReadWrite, Category = "HUD", meta = (BindWidget))
	UTextBlock* SpeedText;
	
	UPROPERTY(EditDefaultsOnly, Category = "HUD|Format")
	FText PointsFormat = FText::FromString("Points: {0}");

	UPROPERTY(EditDefaultsOnly, Category = "HUD|Format")
	FText SpeedFormat = FText::FromString("Speed: {0}%");
};
