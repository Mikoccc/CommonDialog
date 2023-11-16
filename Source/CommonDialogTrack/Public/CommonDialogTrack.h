// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "MovieSceneTrack.h"
#include "Evaluation/MovieSceneEvalTemplate.h"
#include "Compilation/IMovieSceneTrackTemplateProducer.h"

#include "CommonDialogTrack.generated.h"

UCLASS()
class COMMONDIALOGTRACK_API UCommonDialogTrack : public UMovieSceneTrack, public IMovieSceneTrackTemplateProducer
{
	GENERATED_BODY()
public:
	// 创建 Section
	virtual UMovieSceneSection* CreateNewSection() override;
	// 是否支持多个Section
	virtual bool SupportsMultipleRows() const override { return false; }
	//  Track 名字
	virtual FName GetTrackName() const override;
	// get all sections 
	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override { return Sections; }
	// 支持的section 类型
	virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
#if WITH_EDITORONLY_DATA
	virtual FText GetDisplayName() const override;
#endif
	virtual void PostCompile(FMovieSceneEvaluationTrack& Track, const FMovieSceneTrackCompilerArgs& Args) const override;
	// 创建 FSubtitleEvalTemplate
	virtual FMovieSceneEvalTemplatePtr CreateTemplateForSection(const UMovieSceneSection& InSection) const override;

	// remove all section
	virtual void RemoveAllAnimationData() override { Sections.Empty(); }
	// has section
	virtual bool HasSection(const UMovieSceneSection& Section) const override { return Sections.Contains(&Section); }
	// add section
	virtual void AddSection(UMovieSceneSection& Section) override { Sections.Add(&Section); }
	// remove section
	virtual void RemoveSection(UMovieSceneSection& Section) override { Sections.Remove(&Section); }
	// remove section
	virtual void RemoveSectionAt(int32 SectionIndex) override { Sections.RemoveAt(SectionIndex); }
	// has any empty
	virtual bool IsEmpty() const override { return Sections.Num() == 0; }
protected:
	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;


};

