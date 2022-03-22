// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MultProject/MenuSystem/MenuInterface.h"
#include "GamePuzzleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTPROJECT_API UGamePuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	

public:
	
	UGamePuzzleGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(const FString& Adress) override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenuWidget();

	void RefreshServerList() override;

	virtual void LoadMainMenu() override;

private:
	class UMainMenu* Menu;
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);

	void CreateSession();
};
