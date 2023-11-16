#include "TimeMarkerTrack.h"
#include "TimeMarkerEvalTemplate.h"
#include "TimeMarkerSection.h"

#include "Evaluation/MovieSceneEvaluationTrack.h"

FMovieSceneEvalTemplatePtr UTimeMarkerTrack::CreateTemplateForSection(const UMovieSceneSection& InSection) const
{
	const UTimeMarkerSection* Section = CastChecked<const UTimeMarkerSection>(&InSection);
	return FTimeMarkerEvalTemplate(Section);
}

UMovieSceneSection* UTimeMarkerTrack::CreateNewSection()
{
	return NewObject<UMovieSceneSection>(this, UTimeMarkerTrack::StaticClass(), NAME_None, RF_Transactional);
}

#if WITH_EDITORONLY_DATA
FText UTimeMarkerTrack::GetDisplayName() const
{
	return NSLOCTEXT("TimeMarkerTrack", "TrackName", "TimeMarker");
}
#endif

FName UTimeMarkerTrack::GetTrackName() const
{
	static FName TrackName("TimeMarker");
	return TrackName;
}

bool UTimeMarkerTrack::SupportsType(TSubclassOf<UMovieSceneSection> Type) const
{
	return Type == UTimeMarkerSection::StaticClass();
}