// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonDialogDataAsset.h"
#include "GameplayTagContainer.h"

#include "CommonDialogSubsystem.generated.h"

class UCommonActivatableWidget;

DECLARE_DELEGATE(FOnSequencePush);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDialogInteractChange, bool, bCanInteract, bool, bInit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogUpdate, int, SectionID);


UENUM(BlueprintType)
enum class ECommonDialogMode : uint8
{
	// None, no dialogMode
	None, 

	// Normal, pure UI dialog
	Normal,

	// Sequencer, SequenceDialog but push by UI Interact
	Sequencer,

	// Auto£¬ autoDialog Push By Sequence
	Auto
};

USTRUCT(BlueprintType)
struct FCommonDialogConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UCommonDialogDataAsset> Asset = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECommonDialogMode Mode = ECommonDialogMode::None;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag LayerName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> WidgetClass;
};

/**
 * 
 */
UCLASS()
class COMMONDIALOG_API UCommonDialogSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonDialogSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void StartDialog(const FCommonDialogConfig& DialogConfig, bool bCanInteract = true);

	UFUNCTION(BlueprintCallable)
	void EndDialog();

	UFUNCTION(BlueprintCallable)
	void PushDialogToSection(int SeectionID, bool bCanInteract = true);

	UFUNCTION(BlueprintCallable)
	void SetInteract(bool bCanInteract, bool bInit = true);

	UFUNCTION(BlueprintCallable)
	void SetDialogMode(ECommonDialogMode TargetMode);

	UFUNCTION(BlueprintCallable)
	void NotifySequencePush();

	UPROPERTY(BlueprintAssignable)
	FOnDialogStart OnDialogStart;

	UPROPERTY(BlueprintAssignable)
	FOnDialogEnd OnDialogEnd;

	UPROPERTY(BlueprintAssignable)
	FOnDialogUpdate OnDialogUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnDialogInteractChange OnDialogInteractChange;

	FOnSequencePush OnSequencePush;

	UPROPERTY(BlueprintReadOnly)
	FCommonDialogSection CurSection;

	UPROPERTY(BlueprintReadOnly)
	ECommonDialogMode CurDialogMode = ECommonDialogMode::None;

	UPROPERTY(BlueprintReadOnly)
	bool bInteract;

protected:
	TArray<FCommonDialogSection> CurDialogSections;
};
