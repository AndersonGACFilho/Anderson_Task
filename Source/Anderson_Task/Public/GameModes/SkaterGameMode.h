#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkaterGameMode.generated.h"

/**
 * @brief Game mode class for the Skater game.
 * @details This class sets the default pawn to the SkaterPlayerCharacter Blueprint.
 */
UCLASS(minimalapi)
class ASkaterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkaterGameMode();

protected:
	/**
	 * @brief Called after the game mode's properties have been initialized.
	 * @details Sets the DefaultPawnClass to the specified DefaultCharacterClass.
	 */
	virtual void PostInitProperties() override;

	/**
	 * @brief Default character class.
	 * @details Specifies the character class to use as the default pawn.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APawn> DefaultCharacterClass;

	/**
	 * @brief Default player controller class.
	 * @details Specifies the player controller class to use for players.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Classes")
	TSubclassOf<APlayerController> DefaultPlayerControllerClass;
};



