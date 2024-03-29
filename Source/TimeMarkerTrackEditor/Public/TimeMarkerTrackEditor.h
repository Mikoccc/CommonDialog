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
	//增加Track下拉菜单，该功能不需要绑定到Object上面
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;
	
	// 增加object track 下的菜单
	virtual void BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass) override;
	// 创建用于渲染编辑器下的Section 
	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	// 支持的 Track 类型
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	// 图标
	virtual const FSlateBrush* GetIconBrush() const override;
private:
	// 创建 track
	/** Callback for executing the "Add Audio Track" menu entry. */
	void HandleAddTimeMarkerTrackMenuEntryExecute();

	void AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const;
	void CreateNewSection(UTimeMarkerTrack* Track) const;
};