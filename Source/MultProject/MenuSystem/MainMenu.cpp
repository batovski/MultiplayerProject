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
		if(SelectedIndex.IsSet())
		{
			MenuInterface->Join(SelectedIndex.GetValue());
		}
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

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	if (!ensure(ServerRowClass != nullptr)) return;

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();
	uint32 i = 0;
	for (auto& server : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;
		Row->SetServerData(server);
		Row->Setup(this, i);
		++i;
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

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	if (ServerList == nullptr) return;

	for(int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}