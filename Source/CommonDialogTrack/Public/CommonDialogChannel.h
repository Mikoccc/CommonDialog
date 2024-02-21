// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "Channels/MovieSceneEvent.h"
#include "MovieSceneClipboard.h"
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "CoreTypes.h"
#include "Curves/KeyHandle.h"
#include "Math/Range.h"
#include "Misc/FrameNumber.h"
#include "Misc/FrameTime.h"
#include "UObject/ObjectMacros.h"
#include "CommonDialogSubsystem.h"

#include "CommonDialogChannel.generated.h"

struct FFrameRate;
struct FKeyHandle;



//CanInteract 能够交互，有一些交互需要等待
UENUM(BlueprintType)
enum class FCommonDialogKeyType
{
	//Begin 是对话段落开始的key
	Begin = 0,

	//End 是对话结束的Key 需要判断是否等待交互 DialogMode为Sequencer会等待，等待后可以跳转到某个mark，或者是dialogkey Auto会继续播放Sequencer
	End = 1,

	//CanInteract 允许UI交互的key，有一些交互需要等待Sequence
	CanInteract = 2,

};

USTRUCT(BlueprintType)
struct FCommonDialogChannelKeyVal
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCommonDialogKeyType KeyType = FCommonDialogKeyType::Begin;

	//Begin
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::Begin"))
	int DialogIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::Begin"))
	ECommonDialogMode DialogMode = ECommonDialogMode::Sequencer;

	//End 只有当当前播放模式为Sequencer的时候 当前对话才会跳转 如果都不配就不会跳转
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::Begin"))
	int DialogIndexToJump = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::Begin"))
	FString MarkToJump = FString();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::Begin"))
	bool bInteract = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (EditConditionHides, EditCondition = "KeyType == FCommonDialogKeyType::End"))
	bool bPause = true;
};

#if WITH_EDITOR
namespace MovieSceneClipboard
{
	template<> inline FName GetKeyTypeName<FCommonDialogChannelKeyVal>()
	{
		return "FCommonDialogChannelKeyVal";
	}
}
#endif

USTRUCT()
struct COMMONDIALOGTRACK_API FCommonDialogChannel : public FMovieSceneChannel
{
	GENERATED_BODY()

	/**
	 * Access a mutable interface for this channel's data
	 *
	 * @return An object that is able to manipulate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<FCommonDialogChannelKeyVal> GetData()
	{
		return TMovieSceneChannelData<FCommonDialogChannelKeyVal>(&KeyTimes, &KeyValues, &KeyHandles);
	}

	/**
	 * Access a constant interface for this channel's data
	 *
	 * @return An object that is able to interrogate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<const FCommonDialogChannelKeyVal> GetData() const
	{
		return TMovieSceneChannelData<const FCommonDialogChannelKeyVal>(&KeyTimes, &KeyValues);
	}

	/**
	* Const access to this channel's times
	*/
	TArrayView<const FFrameNumber> GetTimes() const
	{
		return KeyTimes;
	}

	/**
	* Const access to this channel's values
	*/
	TArrayView<const FCommonDialogChannelKeyVal> GetValues() const
	{
		return KeyValues;
	}
	/**
	* Evaluate this channel
	*
	* @param InTime     The time to evaluate at
	* @return A pointer to the string, or nullptr
	*/
	bool Evaluate(FFrameTime InTime, FCommonDialogChannelKeyVal& OutValue) const;

	bool EvaluateTimeByIndex(int DialogIndex,FFrameNumber& Time) const;
public:

	// ~ FMovieSceneChannel Interface
	virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes, TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Reset() override;
	virtual void Offset(FFrameNumber DeltaPosition) override;

private:

	/** Array of times for each key */
	UPROPERTY(meta=(KeyTimes))
	TArray<FFrameNumber> KeyTimes;

	// 用于标记 当前Dialog
	UPROPERTY(meta=(KeyValues))
	TArray<FCommonDialogChannelKeyVal> KeyValues;

	/** This needs to be a UPROPERTY so it gets saved into editor transactions but transient so it doesn't get saved into assets. */
	UPROPERTY(Transient)
	FMovieSceneKeyHandleMap KeyHandles;
};

template<>
struct TMovieSceneChannelTraits<FCommonDialogChannel> : TMovieSceneChannelTraitsBase<FCommonDialogChannel>
{
	enum { SupportsDefaults = false };
};

inline bool EvaluateChannel(const FCommonDialogChannel* InChannel, FFrameTime InTime, FCommonDialogChannelKeyVal& OutValue)
{
	return false;
}
