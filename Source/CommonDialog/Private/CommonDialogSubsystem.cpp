// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonDialogSubsystem.h"

#include "Engine/AssetManager.h"
#include "CommonUIExtensions.h"
#include "LogCommonDialog.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"


UCommonDialogSubsystem::UCommonDialogSubsystem()
{

}

void UCommonDialogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCommonDialogSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCommonDialogSubsystem::StartDialog(const FCommonDialogConfig& DialogConfig, bool bCanInteract)
{
	UAssetManager& AssetManager = UAssetManager::Get();
 	UCommonDialogDataAsset* LoadAsset = LoadObject<UCommonDialogDataAsset>(nullptr, *DialogConfig.Asset.ToString());
	if (LoadAsset == nullptr || LoadAsset->DialogSections.Num() <= 0)
	{
		UE_LOG(LogCommonDialog, Warning, TEXT("UCommonDialogSubsystem::StartDialog Fail, due to DialogSections is Empty"));
		return;
	}

	//temp get first local player
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ULocalPlayer* TempLocalPlayer = PlayerController->GetLocalPlayer();

	CurDialogMode = DialogConfig.Mode;
	CurDialogSections = LoadAsset->DialogSections;
	CurSection = CurDialogSections[0];
	SetInteract(bCanInteract);
	UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(TempLocalPlayer, DialogConfig.LayerName, DialogConfig.WidgetClass);
	OnDialogStart.Broadcast();
}

void UCommonDialogSubsystem::EndDialog()
{
	OnDialogEnd.Broadcast();
	OnDialogUpdate.RemoveAll(this);
	CurDialogMode = ECommonDialogMode::None;
}

void UCommonDialogSubsystem::PushDialogToSection(int SectionID, bool bCanInteract)
{
	if (CurDialogSections.Num() <= SectionID)
	{
		UE_LOG(LogCommonDialog, Warning, TEXT("UCommonDialogSubsystem::PushDialogToSection Fail, Due to NextSectionID is out of Range"));
		return;
	}
	
	if (SectionID < 0)
	{
		EndDialog();
		return;
	}

	
	SetInteract(bCanInteract);

	CurSection = CurDialogSections[SectionID];
	OnDialogUpdate.Broadcast(SectionID);
}

void UCommonDialogSubsystem::SetInteract(bool bCanInteract, bool bInit)
{
	bInteract = bCanInteract;
	OnDialogInteractChange.Broadcast(bCanInteract, bInit);
}

void UCommonDialogSubsystem::SetDialogMode(ECommonDialogMode TargetMode)
{
	CurDialogMode = TargetMode;
}

void UCommonDialogSubsystem::NotifySequencePush()
{
	OnSequencePush.ExecuteIfBound();
	OnSequencePush.Unbind();
}

