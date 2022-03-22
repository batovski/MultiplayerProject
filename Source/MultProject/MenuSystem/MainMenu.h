// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MenuWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTPROJECT_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

protected:
	virtual bool Initialize();
	
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* Host_btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Join_btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* MenuBack_btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Connect_btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Quit_btn;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitPressed();

	TSubclassOf<class UUserWidget> ServerRowClass;
};
