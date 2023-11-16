// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeMarkerTrackEditorModule.h"
#include "TimeMarkerTrackEditor.h"
#include "ISequencerModule.h"

#define LOCTEXT_NAMESPACE "FTimeMarkerTrackEditorModule"

void FTimeMarkerTrackEditorModule::StartupModule()
{
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>(TEXT("Sequencer"));
	SeqHandles.Add(SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FTimeMarkerTrackEditor::CreateTrackEditor)));
}

void FTimeMarkerTrackEditorModule::ShutdownModule()
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
	
IMPLEMENT_MODULE(FTimeMarkerTrackEditorModule, TimeMarkerTrackEditor)