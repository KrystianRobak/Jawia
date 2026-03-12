#include "DialogueEditor/ReDialogueAssetTypeActions.h"

#include "Dialogue/ReDialogueGraph.h"
#include "DialogueEditor/ReDialogueEdGraph.h"
#include "DialogueEditor/ReDialogueSchema.h"
#include "Modules/ModuleManager.h"
#include "ReGraphBaseEditorModule.h"

FReDialogueAssetTypeActions::FReDialogueAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{
}

FText FReDialogueAssetTypeActions::GetName() const
{
	return NSLOCTEXT("ReDialogueAssetActions", "AssetName", "Re Dialogue Graph");
}

FColor FReDialogueAssetTypeActions::GetTypeColor() const
{
	return FColor(70, 130, 200);
}

UClass* FReDialogueAssetTypeActions::GetSupportedClass() const
{
	return UReDialogueGraph::StaticClass();
}

uint32 FReDialogueAssetTypeActions::GetCategories()
{
	return AssetCategory;
}

void FReDialogueAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FReGraphBaseEditorModule& BaseEditorModule = FModuleManager::LoadModuleChecked<FReGraphBaseEditorModule>("ReGraphBaseEditor");

	for (UObject* Object : InObjects)
	{
		if (UReDialogueGraph* Graph = Cast<UReDialogueGraph>(Object))
		{
			BaseEditorModule.OpenGraphAssetEditor(Graph, EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone, EditWithinLevelEditor, UReDialogueEdGraph::StaticClass(), UReDialogueSchema::StaticClass());
		}
	}
}
