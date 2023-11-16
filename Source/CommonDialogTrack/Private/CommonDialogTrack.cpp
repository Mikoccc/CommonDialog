#include "CommonDialogTrack.h"
#include "CommonDialogEvalTemplate.h"
#include "CommonDialogTrackSection.h"

#include "Evaluation/MovieSceneEvaluationTrack.h"

void UCommonDialogTrack::PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const
{
	Track.SetEvaluationMethod(EEvaluationMethod::Swept);
}

FMovieSceneEvalTemplatePtr UCommonDialogTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	const UCommonDialogTrackSection* Section = CastChecked<const UCommonDialogTrackSection>(&InSection);
	return FCommonDialogEvalTemplate(Section);
}

UMovieSceneSection* UCommonDialogTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UCommonDialogTrack::StaticClass(), NAME_None, RF_Transactional);
}

#if WITH_EDITORONLY_DATA
FText UCommonDialogTrack::GetDisplayName() const
{
	return NSLOCTEXT("CommonDialogTrack", "TrackName", "CommonDialog");
}
#endif

FName UCommonDialogTrack::GetTrackName() const
{
	static FName TrackName("CommonDialog");
	return TrackName;
}

bool UCommonDialogTrack::SupportsType(TSubclassOf<UMovieSceneSection> Type) const
{
	return Type == UCommonDialogTrackSection::StaticClass();
}