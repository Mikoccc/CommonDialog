// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UObject/Package.h"
#include "MovieSceneTrackEditor.h"

class UTimeMarkerTrack;

class FTimeMarkerTrackEditor final : public FMovieSceneTrackEditor
{

public:
	explicit FTimeMarkerTrackEditor(TSharedRef<ISequencer> InSequencer);
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> OwningSequencer);

public:
	//����Track�����˵����ù��ܲ���Ҫ�󶨵�Object����
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	
	// ����object track �µĲ˵�
	virtual void BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass) override;
	// ����������Ⱦ�༭���µ�Section 
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	// ֧�ֵ� Track ����
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	// ͼ��
	virtual const FSlateBrush* GetIconBrush() const override;
private:
	// ���� track
	/** Callback for executing the "Add Audio Track" menu entry. */
	void HandleAddTimeMarkerTrackMenuEntryExecute();

	void AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const;
	void CreateNewSection(UTimeMarkerTrack* Track) const;
};