// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TimeMarkerActor.generated.h"

UCLASS()
class ATimeMarkerActor : public AActor
{
	GENERATED_BODY()
public:
	void SetSubtitleText(const FString& text);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetSubtitleText(const FString& text);
};

