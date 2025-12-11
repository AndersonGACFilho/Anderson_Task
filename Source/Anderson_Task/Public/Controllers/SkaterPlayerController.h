#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkaterPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSkaterController, Log, All);

/**
 * @brief Player controller for the Skater game.
 * @details Manages HUD creation and display.
 */
UCLASS()
class ANDERSON_TASK_API ASkaterPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Called when the game starts.
	 * @details Creates and displays the HUD widget.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Widget class to spawn for the HUD.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/**
	 * @brief Reference to the spawned HUD widget.
	 */
	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidget;
};
