// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Evaluation/MovieSceneEvalTemplate.h"
#include "TimeMarkerSection.h"

#include "TimeMarkerEvalTemplate.generated.h"

USTRUCT()
struct FTimeMarkerEvalTemplate: public FMovieSceneEvalTemplate
{
	GENERATED_BODY()
public:
	FTimeMarkerEvalTemplate(const UTimeMarkerSection* InSection);
	FTimeMarkerEvalTemplate();
	//FTimeMarkerEvalTemplate(const UTimeMarkerSection* InSection);
	// 运行时 sequencer 循环处理函数
	virtual void Evaluate(const FMovieSceneEvaluationOperand& Operand, const FMovieSceneContext& Context, const FPersistentEvaluationData& PersistentData, FMovieSceneExecutionTokens& ExecutionTokens) const override;
	virtual UScriptStruct& GetScriptStructImpl() const override { return *StaticStruct(); }
	virtual void Setup(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void TearDown(FPersistentEvaluationData& PersistentData, IMovieScenePlayer& Player) const override;
	virtual void SetupOverrides() override { EnableOverrides(RequiresSetupFlag | RequiresTearDownFlag); }

	UPROPERTY()
	const UTimeMarkerSection* Section = nullptr;
	UPROPERTY(Transient)
	const UObject* PlayerObj = nullptr;
};

