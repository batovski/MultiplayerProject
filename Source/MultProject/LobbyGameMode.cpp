// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GamePuzzleGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if(NumberOfPlayers >= 2)
	{
		auto GameInstance = Cast<UGamePuzzleGameInstance>(GetGameInstance());

		if (GameInstance == nullptr) return;
		GameInstance->StartSession();

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		bUseSeamlessTravel = true;
		World->ServerTravel("/Game/Maps/ThirdPersonExampleMap?listen");
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}