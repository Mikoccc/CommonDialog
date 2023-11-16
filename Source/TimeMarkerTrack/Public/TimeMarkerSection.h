// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MovieSceneSection.h"
#include "Channels/MovieSceneStringChannel.h"

#include "TimeMarkerSection.generated.h"

UCLASS()
class TIMEMARKERTRACK_API UTimeMarkerSection : public UMovieSceneSection
{
	GENERATED_BODY()
public:
	UTimeMarkerSection();

	FString GetTitle() { return TEXT(""); }

	FString GetSubtitle(FFrameTime Time) const;

	UFUNCTION(BlueprintCallable)
	TArray<FFrameNumber> GetTimeKeys();

	// ����һ��string ���͵�channel
	UPROPERTY()
	FMovieSceneStringChannel Subtitle;

};

