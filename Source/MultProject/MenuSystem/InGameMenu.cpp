// Fill out your copyright notice in the Description page of Project Settings.


#include "MultProject/MenuSystem/InGameMenu.h"
#include "Components/Button.h"


bool UInGameMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(MainMenu_btn != nullptr)) return false;
	if (!ensure(Cancel_btn != nullptr)) return false;

	MainMenu_btn->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);
	Cancel_btn->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);

	return true;
}

void UInGameMenu::CancelPressed()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->LoadMainMenu();
	}
}