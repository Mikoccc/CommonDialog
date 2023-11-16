// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Evaluation/MovieSceneEvalTemplate.h"

#include "CommonDialogEvalTemplate.generated.h"

class UCommonDialogTrackSection;

USTRUCT()
struct FCommonDialogEvalTemplate : public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
public:
	FCommonDialogEvalTemplate(const UCommonDialogTrackSection* InSection);
	FCommonDialogEvalTemplate();

	// 运行时 sequencer 循环处理函数
	virtual void EvaluateSwept(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const TRange<FFrameNumber>& SweptRange, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	void PushSequence();

	UPROPERTY()
	const UCommonDialogTrackSection* Section = nullptr;
};