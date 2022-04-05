// Fill out your copyright notice in the Description page of Project Settings.


#include "MultProject/MenuSystem/ServerRow.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "MainMenu.h"

void UServerRow::Setup(class UMainMenu* _Parent, uint32 _Index)
{
	Parent = _Parent;
	Index = _Index;
	Server_btn->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	if (Parent == nullptr) return;

	Parent->SelectIndex(Index);
}

void UServerRow::SetServerData(FServerData Data)
{
	ServerName_txt->SetText(FText::FromString(Data.Name));
	Host_txt->SetText(FText::FromString(Data.HostUsername));
	Players_txt->SetText(FText::FromString(Data.CurrentPlayers + "/" + Data.MaxPlayers));
}

