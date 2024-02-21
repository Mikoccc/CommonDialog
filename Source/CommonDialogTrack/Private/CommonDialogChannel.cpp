// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonDialogChannel.h"
#include "Curves/StringCurve.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "MovieSceneFrameMigration.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonDialogChannel)

bool FCommonDialogChannel::Evaluate(FFrameTime InTime, FCommonDialogChannelKeyVal& OutValue) const
{

	if (KeyTimes.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(KeyTimes, InTime.FrameNumber) - 1);
		OutValue = KeyValues[Index];
			return true;
	}

	return false;
}

bool FCommonDialogChannel::EvaluateTimeByIndex(int DialogIndex,FFrameNumber& Time) const
{
	for (int i = 0; i < KeyValues.Num(); i++)
	{
		if (KeyValues[i].KeyType == FCommonDialogKeyType::Begin && DialogIndex == KeyValues[i].DialogIndex)
		{
			Time = KeyTimes[i];
			return true;
		}
	}
	return false;
}

void FCommonDialogChannel::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FCommonDialogChannel::GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FCommonDialogChannel::SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FCommonDialogChannel::DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FCommonDialogChannel::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FCommonDialogChannel::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FCommonDialogChannel::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

TRange<FFrameNumber> FCommonDialogChannel::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FCommonDialogChannel::GetNumKeys() const
{
	return KeyTimes.Num();
}

void FCommonDialogChannel::Reset()
{
	KeyTimes.Reset();
	KeyValues.Reset();
	KeyHandles.Reset();
}

void FCommonDialogChannel::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

