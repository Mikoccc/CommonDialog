#include "TimeMarkerSection.h"

#include "UObject\SequencerObjectVersion.h"
#include "Channels\MovieSceneChannelProxy.h"

// 在sequencer 编辑器中显示用
#if WITH_EDITOR
struct FTimeMarkerEditorData
{
	FTimeMarkerEditorData()
	{
		MetaData[0].SetIdentifiers("Subtitle", NSLOCTEXT("SubtitleSection", "SubtitleText", "Subtitle"));
		MetaData[0].SortOrder = 0;
		MetaData[0].Color = FCommonChannelData::RedChannelColor;
		MetaData[0].bCanCollapseToTrack = false;
	}

	FMovieSceneChannelMetaData      MetaData[1];
	TMovieSceneExternalValue<FString> ExternalValues[1];
};
#endif

UTimeMarkerSection::UTimeMarkerSection()
{
	EvalOptions.EnableAndSetCompletionMode
	(GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToRestoreState ?
		EMovieSceneCompletionMode::KeepState :
		GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToProjectDefault ?
		EMovieSceneCompletionMode::RestoreState :
		EMovieSceneCompletionMode::ProjectDefault);
	BlendType = EMovieSceneBlendType::Relative;
	bSupportsInfiniteRange = true;


	FMovieSceneChannelProxyData Channels;

	// 显示 一个stringchannel
#if WITH_EDITOR
	static FTimeMarkerEditorData EditorData;
	Channels.Add(Subtitle, EditorData.MetaData[0], EditorData.ExternalValues[0]);

#else // 动行时
	Channels.Add(Subtitle);
#endif

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

}

FString UTimeMarkerSection::GetSubtitle(FFrameTime Time) const
{
	if (Subtitle.GetNumKeys() > 0)
	{
		return *Subtitle.Evaluate(Time);
	}
	return FString();
}

TArray<FFrameNumber> UTimeMarkerSection::GetTimeKeys()
{
	TArray<FFrameNumber> Res;
	TArrayView<const FFrameNumber> Times = Subtitle.GetTimes();
	for (auto FramNumber : Times)
	{
		Res.Add(FramNumber);
	}
	return Res;
}
