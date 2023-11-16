// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/Package.h"
#include "MovieSceneTrackEditor.h"

class UCommonDialogTrack;

class FCommonDialogTrackEditor final : public FMovieSceneTrackEditor
{

public:
	explicit FCommonDialogTrackEditor(TSharedRef<ISequencer> InSequencer);
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	//����Track�����˵����ù��ܲ���Ҫ�󶨵�Object����
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	
	// ����������Ⱦ�༭���µ�Section 
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	// ֧�ֵ� Track ����
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	// ͼ��
	virtual const FSlateBrush* GetIconBrush() const override;

	/*virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params) override;*/
private:
	// ���� track
	/** Callback for executing the "Add Audio Track" menu entry. */
	void HandleAddTimeMarkerTrackMenuEntryExecute();

	void AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const;
	void CreateNewSection(UCommonDialogTrack* Track) const;
};