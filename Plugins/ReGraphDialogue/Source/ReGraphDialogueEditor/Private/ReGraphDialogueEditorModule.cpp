#include "ReGraphDialogueEditorModule.h"

#include "AssetToolsModule.h"
#include "DialogueEditor/ReDialogueAssetTypeActions.h"
#include "DialogueEditor/ReDialogueDetailCustomization.h"
#include "IAssetTools.h"
#include "PropertyEditorModule.h"

void FReGraphDialogueEditorModule::StartupModule()
{
	RegisterCustomizations();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("ReGraph")), NSLOCTEXT("ReGraphDialogueEditor", "ReGraphCategory", "ReGraph"));

	TSharedPtr<IAssetTypeActions> DialogueActions = MakeShared<FReDialogueAssetTypeActions>(AssetCategory);
	AssetTools.RegisterAssetTypeActions(DialogueActions.ToSharedRef());
	RegisteredAssetTypeActions.Add(DialogueActions);
}

void FReGraphDialogueEditorModule::ShutdownModule()
{
	UnregisterCustomizations();

	if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		return;
	}

	IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (const TSharedPtr<IAssetTypeActions>& Action : RegisteredAssetTypeActions)
	{
		if (Action.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
		}
	}

	RegisteredAssetTypeActions.Reset();
}

EAssetTypeCategories::Type FReGraphDialogueEditorModule::GetAssetCategory() const
{
	return AssetCategory;
}

void FReGraphDialogueEditorModule::RegisterCustomizations()
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueStartNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueLineNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueChoiceNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueConditionalNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueEventNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReDialogueEndNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReDialogueNodeDetailCustomization::MakeInstance));
	PropertyEditor.NotifyCustomizationModuleChanged();
}

void FReGraphDialogueEditorModule::UnregisterCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		return;
	}

	FPropertyEditorModule& PropertyEditor = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueStartNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueLineNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueChoiceNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueConditionalNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueEventNode"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReDialogueEndNode"));
	PropertyEditor.NotifyCustomizationModuleChanged();
}

IMPLEMENT_MODULE(FReGraphDialogueEditorModule, ReGraphDialogueEditor)
