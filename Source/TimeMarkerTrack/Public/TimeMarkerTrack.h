// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "MovieSceneTrack.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"

#include "TimeMarkerTrack.generated.h"

UCLASS()
class TIMEMARKERTRACK_API UTimeMarkerTrack : public UMovieSceneTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()
public:
	// ���� Section
	virtual UMovieSceneSection* CreateNewSection() override;
	// �Ƿ�֧�ֶ��Section
	virtual bool SupportsMultipleRows() const override { return false; }
	//  Track ����
	virtual FName GetTrackName() const override;
	// get all sections 
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }
	// ֧�ֵ�section ����
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
#if WITH_EDITORONLY_DATA
	virtual FText GetDisplayName() const override;
#endif

	// ���� FSubtitleEvalTemplate
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

	// �±���Щ����ȫ���ø�
	// remove all section
	virtual void RemoveAllAnimationData() override { Sections.Empty(); }
	// has section
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	// add section
	virtual void AddSection(UMovieSceneSection& Section) override { Sections.Add(&Section); }
	// remove section
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	// has any empty
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }

protected:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};

