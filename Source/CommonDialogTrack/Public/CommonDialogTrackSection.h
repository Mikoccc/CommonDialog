// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MovieSceneSection.h"
#include "CommonDialogChannel.h"

#include "CommonDialogTrackSection.generated.h"

UCLASS()
class COMMONDIALOGTRACK_API UCommonDialogTrackSection : public UMovieSceneSection
{
	GENERATED_BODY()
public:
	UCommonDialogTrackSection();

	FString GetTitle() { return TEXT(""); }

	FString GetDialogKeyDisplay(FFrameTime Time) const;

	//对话标记
	UPROPERTY()
	FCommonDialogChannel DialogChannel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCommonDialogConfig DialogConfig;

};

