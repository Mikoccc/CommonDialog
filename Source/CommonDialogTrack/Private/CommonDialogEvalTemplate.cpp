#include "CommonDialogEvalTemplate.h"
#include "CommonDialogTrackSection.h"

#include "LevelSequencePlayer.h"
#include "Engine/GameInstance.h"

struct FCommonDialogExecutionToken final : IMovieSceneExecutionToken
{
	TArray<FCommonDialogChannelKeyVal> KeyVals;
	const UCommonDialogTrackSection* Section;

	explicit FCommonDialogExecutionToken(TArray<FCommonDialogChannelKeyVal> InKeyVals,const UCommonDialogTrackSection* InSection)
		: KeyVals(MoveTemp(InKeyVals)),
		Section(InSection)
	{}

	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		ULevelSequencePlayer* PlayerObj = Cast<ULevelSequencePlayer>(Player.AsUObject());
		if (!PlayerObj)
		{
			UE_LOG(LogTemp, Warning, TEXT("ULevelSequencePlayer is Null"));
			return;
		}

		UWorld* World = PlayerObj->GetWorld();
		check(World);

		UGameInstance* GameInstance = World->GetGameInstance();
		check(GameInstance);

		UCommonDialogSubsystem* DialogSubsystem = UGameInstance::GetSubsystem<UCommonDialogSubsystem>(GameInstance);
		if (!DialogSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("UCommonDialogSubsystem is Null"));
			return;
		}

		for (auto KeyVal : KeyVals)
		{
			switch (KeyVal.KeyType)
			{
			case FCommonDialogKeyType::Begin:
				//推动对话
				if (KeyVal.DialogIndex == 0)
				{
					DialogSubsystem->StartDialog(Section->DialogConfig, KeyVal.bInteract);
				}
				else
				{
					DialogSubsystem->PushDialogToSection(KeyVal.DialogIndex, KeyVal.bInteract);
				}
				
				//设置对话模式
				DialogSubsystem->SetDialogMode(KeyVal.DialogMode);

				//绑定跳转事件
				if (KeyVal.DialogIndexToJump != -1 || KeyVal.MarkToJump != FString())
				{
					FFrameTime FrameTime;
					if (KeyVal.DialogIndexToJump != -1)
					{
						FFrameNumber FrameNumber;
						Section->DialogChannel.EvaluateTimeByIndex(KeyVal.DialogIndexToJump, FrameNumber);
						UMovieScene* MovieScene = PlayerObj->GetSequence()->GetMovieScene();
						check(MovieScene);
						FrameTime = ConvertFrameTime(FrameNumber, MovieScene->GetTickResolution(), MovieScene->GetDisplayRate());
					}
					auto PushSequence = [PlayerObj, KeyVal, FrameTime]()->void
						{
							FMovieSceneSequencePlaybackParams PlayBackParams;
							PlayBackParams.bHasJumped = true;
							PlayBackParams.UpdateMethod = EUpdatePositionMethod::Jump;
							if (KeyVal.DialogIndexToJump != -1)
							{
								PlayBackParams.PositionType = EMovieScenePositionType::Frame;
								PlayBackParams.Frame = FrameTime;
							}
							else if (KeyVal.MarkToJump != FString())
							{
								PlayBackParams.PositionType = EMovieScenePositionType::MarkedFrame;
								PlayBackParams.MarkedFrame = KeyVal.MarkToJump;
							}
							PlayerObj->SetPlaybackPosition(PlayBackParams);
							PlayerObj->Play();
						};
					DialogSubsystem->OnSequencePush.BindLambda(PushSequence);
				}
				break;
			case FCommonDialogKeyType::End:

				if (DialogSubsystem->CurDialogMode == ECommonDialogMode::Sequencer)
				{
					//Sequencer模式需要等待
					if (KeyVal.bPause)
					{
						PlayerObj->Pause();
					}

					//如果是最后一句话 需要关闭对话
					if (DialogSubsystem->CurSection.DialogSubSectionIDs.Num() == 0 && DialogSubsystem->CurSection.NextSection < 0)
					{
						DialogSubsystem->EndDialog();
					}
				}

				break;
			case FCommonDialogKeyType::CanInteract:
				DialogSubsystem->SetInteract(true, false);
				break;
			}
		}
	}	
};

FCommonDialogEvalTemplate::FCommonDialogEvalTemplate(const UCommonDialogTrackSection* InSection)
	: Section(InSection)
{}

FCommonDialogEvalTemplate::FCommonDialogEvalTemplate()
{
	Section = nullptr;
}

void FCommonDialogEvalTemplate::EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange,
	const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	if (Section == nullptr)
	{
		return;
	}

	//不接受反向跑的事件
	const bool bBackwards = Context.GetDirection() == EPlayDirection::Backwards;
	if (bBackwards)
	{
		return;
	}

	TArray<FCommonDialogChannelKeyVal> ValidKeyVals;

	TArrayView<const FFrameNumber>  KeyTimes = Section->DialogChannel.GetTimes();
	TArrayView<const FCommonDialogChannelKeyVal> KeyValues = Section->DialogChannel.GetValues();

	const float PositionInSeconds = Context.GetTime() * Context.GetRootToSequenceTransform().InverseLinearOnly() / Context.GetFrameRate();

	for (int32 KeyIndex = 0; KeyIndex < KeyTimes.Num(); ++KeyIndex)
	{
		FFrameNumber Time = KeyTimes[KeyIndex];
		if (SweptRange.Contains(Time))
		{
			ValidKeyVals.Add(KeyValues[KeyIndex]);
		}
	}

	if (ValidKeyVals.Num())
	{
		ExecutionTokens.Add(FCommonDialogExecutionToken(MoveTemp(ValidKeyVals), Section));
	}
}

void FCommonDialogEvalTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
// 	if (!Section)
// 	{
// 		return;
// 	}
// 
// 	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
// 	{
// 		return;
// 	}
// 	
// 	PlayerObj = Player.AsUObject();

}

void FCommonDialogEvalTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
// 	if (!Section)
// 	{
// 		return;
// 	}
// 
// 	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
// 	{
// 		return;
// 	}
// 
// 	Section->PlayerObj = nullptr;
// }
}