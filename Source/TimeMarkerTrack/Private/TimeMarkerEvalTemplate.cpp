#include "TimeMarkerEvalTemplate.h"

#include "TimeMarkerActor.h"

struct FTimeMarkerExecutionToken final : IMovieSceneExecutionToken
{
	const UTimeMarkerSection* Section;

	explicit FTimeMarkerExecutionToken(const UTimeMarkerSection* InSection)
		: Section(InSection)
	{}

	virtual void Execute(const FMovieSceneContext& Context, const FMovieSceneEvaluationOperand& Operand, FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) override
	{
		// 如果没有播放
		if ((Context.GetStatus() != EMovieScenePlayerStatus::Playing && Context.GetStatus() != EMovieScenePlayerStatus::Scrubbing) || Context.HasJumped())
		{
			return;
		}
		//如果没有绑定的actor
		if (!Operand.ObjectBindingID.IsValid())
		{
			return;
		}

		AActor* BindActor = nullptr;
		for (TWeakObjectPtr<> WeakObject : Player.FindBoundObjects(Operand))
		{
			BindActor = Cast<AActor>(WeakObject.Get());
			if (BindActor)
			{
				break;
			}
		}
		if (!BindActor)
		{
			return;
		}
		auto SubtitleActor = Cast<ATimeMarkerActor>(BindActor);
		// 得到绑定的actor 并转为ASubtitleActor
		if (!SubtitleActor)
		{
			return;
		}

		const FFrameTime Time = Context.GetTime();
		FString Text = Section->GetSubtitle(Time);

		// 取得当前时间的字幕并传到actor中处理
		SubtitleActor->SetSubtitleText(Text);
	}
};



FTimeMarkerEvalTemplate::FTimeMarkerEvalTemplate(const UTimeMarkerSection* InSection)
	: Section(InSection)
{}

FTimeMarkerEvalTemplate::FTimeMarkerEvalTemplate()
{
	Section = nullptr;
}

void FTimeMarkerEvalTemplate::Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context,
	const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const
{
	// 启动 FSubtitleExecutionToken
	ExecutionTokens.Add(FTimeMarkerExecutionToken(Section));
}

void FTimeMarkerEvalTemplate::Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	if (!Section)
	{
		return;
	}

	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
	{
		return;
	}

	const_cast<FTimeMarkerEvalTemplate*>(this)->PlayerObj = Player.AsUObject();
}

void FTimeMarkerEvalTemplate::TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const
{
	if (!Section)
	{
		return;
	}

	if (Player.AsUObject() == nullptr || Player.AsUObject()->GetWorld() == nullptr)
	{
		return;
	}

	const_cast<FTimeMarkerEvalTemplate*>(this)->PlayerObj = nullptr;

}
