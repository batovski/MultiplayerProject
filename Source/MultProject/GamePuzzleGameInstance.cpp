// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePuzzleGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultProject/MenuSystem/MainMenu.h"
#include "MultProject/MenuSystem/MenuWidget.h"
#include "UObject/UnrealNames.h"
#include "Blueprint/UserWidget.h"

const static FName SESSION_NAME  = EName::NAME_GameSession;

UGamePuzzleGameInstance::UGamePuzzleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;

}

void UGamePuzzleGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem!= nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface.IsValid()){
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGamePuzzleGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGamePuzzleGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGamePuzzleGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGamePuzzleGameInstance::OnJoinSessionComplete);
		}

		if(GEngine != nullptr)
		{
			GEngine->OnNetworkFailure().AddUObject(this, &UGamePuzzleGameInstance::OnNetworkFailure);
		}
	}
}
void UGamePuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if(Success)
	{
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;

		Engine->AddOnScreenDebugMessage(0, 4, FColor::Green, TEXT("Hosting"));

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		World->ServerTravel("/Game/Maps/Lobby?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create the session!"));
	}
}

void UGamePuzzleGameInstance::OnFindSessionComplete(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("Finished searching sessions!"));
	if(Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServerNames;
		for(FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found the session: %s"), *SearchResult.GetSessionIdStr());

			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;

			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UGamePuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if(Success)
	{
		CreateSession();
	}
}

void UGamePuzzleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Adress;
	if(!SessionInterface->GetResolvedConnectString(SessionName, Adress))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string!"));
	}

	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr)) return;

		PlayerController->ClientTravel(Adress, ETravelType::TRAVEL_Absolute);
	}
}

void UGamePuzzleGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}


void UGamePuzzleGameInstance::Host()
{
	if (SessionInterface.IsValid()) {
		auto ExistingSesion = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSesion == nullptr)
		{
			CreateSession();
		}
		else 
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
}
void UGamePuzzleGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
			SessionSettings.bIsLANMatch = true;
		else
			SessionSettings.bIsLANMatch = false;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		//SessionSettings.Set(TEXT("Test"), FString("Hello"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UGamePuzzleGameInstance::Join(uint32 Index)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}
void UGamePuzzleGameInstance::StartSession()
{
	if(SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}


void UGamePuzzleGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UGamePuzzleGameInstance::LoadInGameMenuWidget()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UGamePuzzleGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->MaxSearchResults = 100;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UGamePuzzleGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}