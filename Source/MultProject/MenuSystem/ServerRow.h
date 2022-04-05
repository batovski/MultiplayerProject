// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"

#include "MainMenu.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class MULTPROJECT_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);
	void SetServerData(struct FServerData Data);
private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName_txt;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Host_txt;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Players_txt;

	UPROPERTY()
	class UMainMenu* Parent;

	UPROPERTY(meta = (BindWidget))
	class UButton* Server_btn;

	UFUNCTION()
	void OnClicked();

	uint32 Index;
};
