// Fill out your copyright notice in the Description page of Project Settings.


#include "MultProject/MenuSystem/MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(Host_btn != nullptr)) return false;
	if (!ensure(Join_btn != nullptr)) return false;
	if (!ensure(Connect_btn != nullptr)) return false;
	if (!ensure(MenuBack_btn != nullptr)) return false;
	if (!ensure(Quit_btn != nullptr)) return false;
	if (!ensure(ServerList != nullptr)) return false;

	Host_btn->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	Join_btn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	Connect_btn->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	Quit_btn->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);
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
		/*
		if (!ensure(IPAdressField != nullptr)) return;

		const FString& Adress = IPAdressField->GetText().ToString();
		MenuInterface->Join(Adress);*/
	}
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (!ensure(ServerRowClass != nullptr)) return;

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	for (auto& server : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;
		Row->ServerName->SetText(FText::FromString(server));
		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher != nullptr)
	{
		if (!ensure(JoinMenu != nullptr)) return;

		MenuSwitcher->SetActiveWidget(JoinMenu);
		if (MenuInterface != nullptr)
			MenuInterface->RefreshServerList();
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