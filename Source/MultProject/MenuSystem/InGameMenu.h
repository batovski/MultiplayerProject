// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultProject/MenuSystem/MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTPROJECT_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenu_btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Cancel_btn;

	UFUNCTION()
		void CancelPressed();
	UFUNCTION()
		void QuitPressed();
};
