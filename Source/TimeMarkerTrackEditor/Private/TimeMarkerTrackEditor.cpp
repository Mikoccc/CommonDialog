// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeMarkerTrackEditor.h"

#include "SequencerSectionPainter.h"
#include "Internationalization/Internationalization.h"
#include "EditorStyleSet.h"
#include "Fonts/FontMeasure.h"

#include "TimeMarkerSection.h"
#include "TimeMarkerTrack.h"

#include "TimeToPixel.h"
#include "ScopedTransaction.h"

#include "MovieSceneSequenceEditor.h"

#define LOCTEXT_NAMESPACE "TimeMarkerTrackEditor"

class FTimeMarkerSection final : public ISequencerSection
{
public:
	FTimeMarkerSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<UTimeMarkerSection>(&InSection))
		, Sequencer(InSequencer)
	{
	}
	virtual ~FTimeMarkerSection() override
	{
	}
	virtual UMovieSceneSection* GetSectionObject() override { return Section; }
	virtual FText GetSectionTitle() const override
	{
		if (Section != nullptr)
		{
			return FText::FromString(Section->GetTitle());
		}
		return FText::GetEmpty();
	}
	virtual void BuildSectionContextMenu(FMenuBuilder& MenuBuilder, const FGuid& ObjectBinding) override
	{
	}
	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		const int32 LayerId = InPainter.PaintSectionBackground();
		if (!Section->HasStartFrame() || !Section->HasEndFrame())
		{
			return LayerId;
		}

		const ESlateDrawEffect DrawEffects = InPainter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
		const FLinearColor DrawColor = FEditorStyle::GetSlateColor("SelectionColor").GetColor(FWidgetStyle());

		TSharedPtr<ISequencer> SequencerPtr = Sequencer.Pin();
		TArray<FFrameNumber> KeyTimes;
		TArray<FKeyHandle> KeyHandles;
		TArray<FFrameNumber> NewKeyTimes;
		// 得到所有的关键帧
		Section->Subtitle.GetKeys(TRange<FFrameNumber>::All(), &KeyTimes, &KeyHandles);
		//Section->Subtitle.GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
		// 循环渲染
		for (int i = 0; i < KeyTimes.Num(); i++)
		{
			//FFrameTime CurrentTime = SequencerPtr->GetLocalTime().Time;
			// 取得这一帧的时间
			FFrameTime CurrentTime = KeyTimes[i];
			// 得到字符串
			FString FrameString = Section->GetSubtitle(CurrentTime);

			// 下边这些是计算位置
			const FTimeToPixel& TimeToPixelConverter = InPainter.GetTimeConverter();
			const FSlateFontInfo SmallLayoutFont = FCoreStyle::GetDefaultFontStyle("Bold", 10);
			const TSharedRef< FSlateFontMeasure > FontMeasureService = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
			const float Time = TimeToPixelConverter.FrameToPixel(CurrentTime);

			FVector2D TextSize = FontMeasureService->Measure(FrameString, SmallLayoutFont);

			// Flip the text position if getting near the end of the view range
			static const float TextOffsetPx = 10.f;
			bool  bDrawLeft = (InPainter.SectionGeometry.Size.X - Time) < (TextSize.X + 22.f) - TextOffsetPx;
			float TextPosition = bDrawLeft ? Time - TextSize.X - TextOffsetPx : Time + TextOffsetPx;
			//handle mirrored labels
			const float MajorTickHeight = 9.0f;
			FVector2D TextOffset(TextPosition, InPainter.SectionGeometry.Size.Y - (MajorTickHeight + TextSize.Y));
			// 把文字渲染上去
			FSlateDrawElement::MakeText(
				InPainter.DrawElements,
				LayerId + 6,
				InPainter.SectionGeometry.ToPaintGeometry(TextOffset, TextSize),
				FrameString,
				SmallLayoutFont,
				DrawEffects,
				DrawColor
			);
		}

		return LayerId;
	}

private:
	UTimeMarkerSection* Section;
	TWeakPtr<ISequencer> Sequencer;
};


FTimeMarkerTrackEditor::FTimeMarkerTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FTimeMarkerTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FTimeMarkerTrackEditor(InSequencer));
}

void FTimeMarkerTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
// 	UMovieSceneSequence* RootMovieSceneSequence = GetSequencer()->GetRootMovieSceneSequence();
// 	FMovieSceneSequenceEditor* SequenceEditor = FMovieSceneSequenceEditor::Find(RootMovieSceneSequence);
// 
// 	if (SequenceEditor && SequenceEditor->SupportsEvents(RootMovieSceneSequence))
// 	{
// 		MenuBuilder.AddSubMenu(
// 			LOCTEXT("AddTrack", "TimeMarker Track"),
// 			LOCTEXT("AddTooltip", "Adds a new TimeMarker track on the timeline that can Mark Time and collected."),
// 			FNewMenuDelegate::CreateRaw(this, &FTimeMarkerTrackEditor::AddNewObjectBindingTrack, TArray<FGuid>()),
// 			false,
// 			FSlateIcon("TimeMarker", "EventIcon"),
// 		);
// 	}
// 

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddTrack", "TimeMarker Track"),
		LOCTEXT("AddTooltip", "Adds a new TimeMarker track that can Mark Time and collected."),
		FSlateIcon("TimeMarker", "EventIcon"),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FTimeMarkerTrackEditor::HandleAddTimeMarkerTrackMenuEntryExecute)
		)
	);
}

// 添加菜单
void FTimeMarkerTrackEditor::BuildObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	// 因为我们要用到ASubtitleActor 所以只有在 ASubtitleActor 下才会显示这个菜单
// 	if (ObjectClass->IsChildOf(ASubtitleActor::StaticClass()))
// 	{
// 		MenuBuilder.AddMenuEntry(
// 			LOCTEXT("AddSubtitleTrackTrack", "Subtitle"),
// 			LOCTEXT("AddSubtitleTrackTooltip", "Adds Subtitle Track."),
// 			FSlateIcon("Subtitle", "EventIcon"),
// 			FUIAction(
// 				FExecuteAction::CreateSP(this, &FSubtitleTrackEditor::AddNewObjectBindingTrack, ObjectBindings)
// 			)
// 		);
// 
// 	}
	// 因为我们要用到ASubtitleActor 所以只有在 ASubtitleActor 下才会显示这个菜单
	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddSubtitleTrackTrack", "Subtitle"),
		LOCTEXT("AddSubtitleTrackTooltip", "Adds Subtitle Track."),
		FSlateIcon("Subtitle", "EventIcon"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FTimeMarkerTrackEditor::AddNewObjectBindingTrack, ObjectBindings)
		)
	);
}

bool FTimeMarkerTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UTimeMarkerTrack::StaticClass();
}

TSharedRef<ISequencerSection> FTimeMarkerTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FTimeMarkerSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FTimeMarkerTrackEditor::GetIconBrush() const
{
	return nullptr;
}

// 创建一个Section
void FTimeMarkerTrackEditor::CreateNewSection(UTimeMarkerTrack* Track) const
{
	TSharedPtr<ISequencer> SequencerPin = GetSequencer();
	UClass* Class = UTimeMarkerSection::StaticClass();

	if (Class && SequencerPin)
	{
		FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomSection_Transaction", "Add New Section From Class %s"), FText::FromName(Class->GetFName())));
		//UMovieScene3DTransformSection* NewSection = NewObject<UMovieScene3DTransformSection>(Track, UMovieScene3DTransformSection::StaticClass(), NAME_None, RF_Transactional);
		UTimeMarkerSection* NewAttachSection = NewObject<UTimeMarkerSection>(Track, Class, NAME_None, RF_Transactional);

		const FQualifiedFrameTime CurrentTime = SequencerPin->GetLocalTime();

		const FFrameNumber Duration = (5.f * CurrentTime.Rate).FrameNumber;
		NewAttachSection->SetRange(TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, CurrentTime.Time.FrameNumber + Duration));
		NewAttachSection->InitialPlacement(Track->GetAllSections(), CurrentTime.Time.FrameNumber, Duration.Value, Track->SupportsMultipleRows());

		Track->AddSection(*NewAttachSection);
		//Track->AddSection(*NewSection);

		SequencerPin->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	}
}

void FTimeMarkerTrackEditor::HandleAddTimeMarkerTrackMenuEntryExecute()
{
	UMovieScene* FocusedMovieScene = GetFocusedMovieScene();

	if (FocusedMovieScene == nullptr)
	{
		return;
	}

	if (FocusedMovieScene->IsReadOnly())
	{
		return;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("Sequencer", "AddTimeMarkerTrack_Transaction", "Add TimeMarker Track"));
	FocusedMovieScene->Modify();

	auto NewTrack = FocusedMovieScene->AddTrack<UTimeMarkerTrack>();
	ensure(NewTrack);

	//NewTrack->SetDisplayName(LOCTEXT("AudioTrackName", "Audio"));

	if (GetSequencer().IsValid())
	{
		GetSequencer()->OnAddTrack(NewTrack, FGuid());
		CreateNewSection(NewTrack);
	}
}

// 创建 Track
void FTimeMarkerTrackEditor::AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const
{
	UMovieScene* MovieScene = GetFocusedMovieScene();
	if (MovieScene == nullptr || MovieScene->IsReadOnly())
	{
		return;
	}

	UClass* ClassToAdd = UTimeMarkerTrack::StaticClass();//LoadClassFromAssetData(AssetData);


	const FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomObjectTrack_Transaction", "Add Object Track %s"), FText::FromName(ClassToAdd->GetFName())));

	MovieScene->Modify();

	for (const FGuid& ObjectBindingID : InObjectBindings)
	{
		UTimeMarkerTrack* CustomTrack = CastChecked<UTimeMarkerTrack>(MovieScene->AddTrack(ClassToAdd, ObjectBindingID));
		CreateNewSection(CustomTrack);

		if (GetSequencer().IsValid())
		{
			GetSequencer()->OnAddTrack(CustomTrack, FGuid());
		}
	}
}



#undef LOCTEXT_NAMESPACE