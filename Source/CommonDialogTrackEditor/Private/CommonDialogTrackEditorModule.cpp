// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonDialogTrackEditorModule.h"
#include "CommonDialogTrackEditor.h"
#include "CommonDialogChannel.h"

#include "ISequencerModule.h"
#include "SequencerChannelInterface.h"

#define LOCTEXT_NAMESPACE "FCommonDialogTrackEditorModule"

void FCommonDialogTrackEditorModule::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>(TEXT("Sequencer"));
	SequencerModule.RegisterChannelInterface<FCommonDialogChannel>();
	SeqHandles.Add(SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FCommonDialogTrackEditor::CreateTrackEditor)));
}

void FCommonDialogTrackEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Sequencer")))
	{
		ISequencerModule& SequencerModule = FModuleManager::GetModuleChecked<ISequencerModule>(TEXT("Sequencer"));

		for (const auto Handle : SeqHandles)
		{
			SequencerModule.UnRegisterTrackEditor(Handle);
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonDialogTrackEditorModule, CommonDialogTrack)