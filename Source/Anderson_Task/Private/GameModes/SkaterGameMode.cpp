#include "GameModes/SkaterGameMode.h"
#include "UObject/ConstructorHelpers.h"

ASkaterGameMode::ASkaterGameMode()
{

}

void ASkaterGameMode::PostInitProperties()
{
	Super::PostInitProperties();

	if (DefaultCharacterClass)
	{
		DefaultPawnClass = DefaultCharacterClass;
	}

	if (DefaultPlayerControllerClass)
	{
		PlayerControllerClass = DefaultPlayerControllerClass;
	}
}