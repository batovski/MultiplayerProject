// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Adress);

	UFUNCTION(BlueprintCallable)
	void LoadMenu();


private:
	TSubclassOf<class UUserWidget> MenuClass;

};
