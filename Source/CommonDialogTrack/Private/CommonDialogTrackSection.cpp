#include "CommonDialogTrackSection.h"

#include "LevelSequencePlayer.h"
#include "UObject\SequencerObjectVersion.h"
#include "Channels\MovieSceneChannelProxy.h"

UCommonDialogTrackSection::UCommonDialogTrackSection()
{
	EvalOptions.EnableAndSetCompletionMode
	(GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToRestoreState ?
		EMovieSceneCompletionMode::KeepState :
		GetLinkerCustomVersion(FSequencerObjectVersion::GUID) < FSequencerObjectVersion::WhenFinishedDefaultsToProjectDefault ?
		EMovieSceneCompletionMode::RestoreState :
		EMovieSceneCompletionMode::ProjectDefault);
	BlendType = EMovieSceneBlendType::Relative;
	bSupportsInfiniteRange = true;


#if WITH_EDITOR

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(DialogChannel, FMovieSceneChannelMetaData());

#else

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(DialogChannel);

#endif

}

FString UCommonDialogTrackSection::GetDialogKeyDisplay(FFrameTime Time) const
{
	if (DialogChannel.GetNumKeys() > 0)
	{
		FCommonDialogChannelKeyVal Val;
		if (DialogChannel.Evaluate(Time, Val))
		{
			switch (Val.KeyType)
			{
				case FCommonDialogKeyType::Begin:
					return FString::FromInt(Val.DialogIndex);
				case FCommonDialogKeyType::End:
					return "End";
				case FCommonDialogKeyType::CanInteract:
					return "CanInteract";
			}
		}
	}
	return FString();
}

