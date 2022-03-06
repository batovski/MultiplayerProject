// Fill out your copyright notice in the Description page of Project Settings.


#include "MultProject/MenuSystem/MainMenu.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(Host_btn != nullptr)) return false;
	if (!ensure(Join_btn != nullptr)) return false;
	if (!ensure(Connect_btn != nullptr)) return false;
	if (!ensure(MenuBack_btn != nullptr)) return false;
	if (!ensure(IPAdressField != nullptr)) return false;

	Host_btn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	Join_btn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	Connect_btn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	MenuBack_btn->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if(MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (!ensure(IPAdressField != nullptr)) return;

		const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join(Adress);
	}
}
void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher != nullptr)
	{
		if (!ensure(JoinMenu != nullptr)) return;

		MenuSwitcher->SetActiveWidget(JoinMenu);
	}
}
void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher != nullptr)
	{
		if (!ensure(MainMenu != nullptr)) return;

		MenuSwitcher->SetActiveWidget(MainMenu);
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
	FInputModeGameOnly InputModeData;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}