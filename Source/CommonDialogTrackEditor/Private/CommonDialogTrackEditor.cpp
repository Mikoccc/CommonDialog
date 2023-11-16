// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonDialogTrackEditor.h"

#include "SequencerSectionPainter.h"
#include "Internationalization/Internationalization.h"
#include "EditorStyleSet.h"
#include "Fonts/FontMeasure.h"

#include "CommonDialogTrackSection.h"
#include "CommonDialogTrack.h"

#include "TimeToPixel.h"
#include "ScopedTransaction.h"
#include "SequencerUtilities.h"

#include "MovieSceneSequenceEditor.h"

#define LOCTEXT_NAMESPACE "CommonDialogTrackEditor"

class FCommonDialogTrackSection final : public ISequencerSection
{
public:
	FCommonDialogTrackSection(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
		: Section(Cast<UCommonDialogTrackSection>(&InSection))
		, Sequencer(InSequencer)
	{
	}
	virtual ~FCommonDialogTrackSection() override
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

		const ESlateDrawEffect DrawEffects = InPainter.bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;
		const FLinearColor DrawColor = FEditorStyle::GetSlateColor("SelectionColor").GetColor(FWidgetStyle());

		TSharedPtr<ISequencer> SequencerPtr = Sequencer.Pin();
		TArray<FFrameNumber> KeyTimes;
		TArray<FKeyHandle> KeyHandles;
		TArray<FFrameNumber> NewKeyTimes;
		// 得到所有的关键帧
		Section->DialogChannel.GetKeys(TRange<FFrameNumber>::All(), &KeyTimes, &KeyHandles);
		// 循环渲染
		for (int i = 0; i < KeyTimes.Num(); i++)
		{
			//FFrameTime CurrentTime = SequencerPtr->GetLocalTime().Time;
			// 取得这一帧的时间
			FFrameTime CurrentTime = KeyTimes[i];
			// 得到字符串
			FString FrameString = Section->GetDialogKeyDisplay(CurrentTime);

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
	UCommonDialogTrackSection* Section;
	TWeakPtr<ISequencer> Sequencer;
};


FCommonDialogTrackEditor::FCommonDialogTrackEditor(TSharedRef<ISequencer> InSequencer)
	: FMovieSceneTrackEditor(InSequencer)
{
}

TSharedRef<ISequencerTrackEditor> FCommonDialogTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FCommonDialogTrackEditor(InSequencer));
}

void FCommonDialogTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
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
		LOCTEXT("AddTrack", "CommomDialog Track"),
		LOCTEXT("AddTooltip", "Adds a new CommomDialog track that can play dialog"),
		FSlateIcon("CommonDialog", "EventIcon"),
		FUIAction(
			FExecuteAction::CreateRaw(this, &FCommonDialogTrackEditor::HandleAddTimeMarkerTrackMenuEntryExecute)
		)
	);
}

bool FCommonDialogTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return Type == UCommonDialogTrack::StaticClass();
}

TSharedRef<ISequencerSection> FCommonDialogTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FCommonDialogTrackSection(SectionObject, GetSequencer()));
}

const FSlateBrush* FCommonDialogTrackEditor::GetIconBrush() const
{
	return nullptr;
}

//增加Section
// TSharedPtr<SWidget> FCommonDialogTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
// {
// 	check(Track);
// 
// 	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
// 	if (!SequencerPtr.IsValid())
// 	{
// 		return SNullWidget::NullWidget;
// 	}
// 	UCommonDialogTrack* DialogTrack = Cast<UCommonDialogTrack>(Track);
// 	if (DialogTrack == nullptr)
// 	{
// 		return SNullWidget::NullWidget;
// 	}
// 
// 	auto SubMenuCallback = [this, DialogTrack]
// 		{
// 			FMenuBuilder MenuBuilder(true, nullptr);
// 
// 			if (DialogTrack)
// 			{
// 				MenuBuilder.AddMenuEntry(
// 					LOCTEXT("AddNewDialogSection", "Dialog"),
// 					LOCTEXT("AddNewDialogSectionTooltip", "Adds a new section that can dialog"),
// 					FSlateIcon(),
// 					FUIAction(
// 						FExecuteAction::CreateSP(this, &FCommonDialogTrackEditor::CreateNewSection, DialogTrack)
// 					)
// 				);
// 			}
// 			else
// 			{
// 				MenuBuilder.AddWidget(SNew(STextBlock).Text(LOCTEXT("InvalidTrack", "Track is no longer valid")), FText(), true);
// 			}
// 
// 			return MenuBuilder.MakeWidget();
// 		};
// 
// 	return SNew(SHorizontalBox)
// 		+ SHorizontalBox::Slot()
// 		.AutoWidth()
// 		.VAlign(VAlign_Center)
// 		[
// 			FSequencerUtilities::MakeAddButton(LOCTEXT("AddSection", "Section"), FOnGetContent::CreateLambda(SubMenuCallback), Params.NodeIsHovered, GetSequencer())
// 		];
// }

// 创建一个Section
void FCommonDialogTrackEditor::CreateNewSection(UCommonDialogTrack* Track) const
{
	TSharedPtr<ISequencer> SequencerPin = GetSequencer();
	UClass* Class = UCommonDialogTrackSection::StaticClass();

	if (Class && SequencerPin)
	{
		FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomSection_Transaction", "Add New Section From Class %s"), FText::FromName(Class->GetFName())));
		//UMovieScene3DTransformSection* NewSection = NewObject<UMovieScene3DTransformSection>(Track, UMovieScene3DTransformSection::StaticClass(), NAME_None, RF_Transactional);
		UCommonDialogTrackSection* NewAttachSection = NewObject<UCommonDialogTrackSection>(Track, Class, NAME_None, RF_Transactional);

		const FQualifiedFrameTime CurrentTime = SequencerPin->GetLocalTime();

		const FFrameNumber Duration = (5.f * CurrentTime.Rate).FrameNumber;
		NewAttachSection->SetRange(TRange<FFrameNumber>(CurrentTime.Time.FrameNumber, CurrentTime.Time.FrameNumber + Duration));
		NewAttachSection->InitialPlacement(Track->GetAllSections(), CurrentTime.Time.FrameNumber, Duration.Value, Track->SupportsMultipleRows());

		Track->AddSection(*NewAttachSection);
		//Track->AddSection(*NewSection);

		SequencerPin->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
	}
}

void FCommonDialogTrackEditor::HandleAddTimeMarkerTrackMenuEntryExecute()
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

	auto NewTrack = FocusedMovieScene->AddTrack<UCommonDialogTrack>();
	ensure(NewTrack);

	//NewTrack->SetDisplayName(LOCTEXT("AudioTrackName", "Audio"));

	if (GetSequencer().IsValid())
	{
		GetSequencer()->OnAddTrack(NewTrack, FGuid());
		CreateNewSection(NewTrack);
	}
}

// 创建 Track
void FCommonDialogTrackEditor::AddNewObjectBindingTrack(TArray<FGuid> InObjectBindings) const
{
	UMovieScene* MovieScene = GetFocusedMovieScene();
	if (MovieScene == nullptr || MovieScene->IsReadOnly())
	{
		return;
	}

	UClass* ClassToAdd = UCommonDialogTrack::StaticClass();//LoadClassFromAssetData(AssetData);


	const FScopedTransaction Transaction(FText::Format(LOCTEXT("AddCustomObjectTrack_Transaction", "Add Object Track %s"), FText::FromName(ClassToAdd->GetFName())));

	MovieScene->Modify();

	for (const FGuid& ObjectBindingID : InObjectBindings)
	{
		UCommonDialogTrack* CustomTrack = CastChecked<UCommonDialogTrack>(MovieScene->AddTrack(ClassToAdd, ObjectBindingID));
		CreateNewSection(CustomTrack);

		if (GetSequencer().IsValid())
		{
			GetSequencer()->OnAddTrack(CustomTrack, FGuid());
		}
	}
}



#undef LOCTEXT_NAMESPACE