#include "Controllers/SkaterPlayerController.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogSkaterController);

void ASkaterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!HUDWidgetClass)
	{
		UE_LOG(LogSkaterController, Warning, TEXT("%s: HUDWidgetClass is not set"), *GetName());
		return;
	}

	HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	if (!HUDWidget)
	{
		UE_LOG(LogSkaterController, Warning, TEXT("%s: Failed to create HUDWidget from class %s"),
			*GetName(), *HUDWidgetClass->GetName());
		return;
	}
	HUDWidget->AddToViewport();
}
