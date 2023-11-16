// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "CommonDialogDataAsset.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct COMMONDIALOG_API FCommonDialogSubSection
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	FText SubSelectionContent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	int SubSectionID = -1;
};

USTRUCT(BlueprintType, Blueprintable)
struct COMMONDIALOG_API FCommonDialogSection
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	FText DialogOwner;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	TSoftObjectPtr<UTexture2D> DialogOwnerTexture;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	FText DialogContent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	float InteractDelayTime = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	TArray<FCommonDialogSubSection> DialogSubSectionIDs;

	//if DialogSubSectionIDs Is Empty, this will decide next section
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	int NextSection = -1;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class COMMONDIALOG_API UCommonDialogDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
	TArray<FCommonDialogSection> DialogSections;
};
