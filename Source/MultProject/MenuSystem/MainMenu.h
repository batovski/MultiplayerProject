// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTPROJECT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* _MenuInterface);

	void Setup();
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld);

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
	class UEditableText* IPAdressField;

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

	IMenuInterface* MenuInterface;
};
