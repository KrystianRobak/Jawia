#include "ReGraphBaseEditorModule.h"

#include "GraphEditor/ReGraphDetailCustomizations.h"
#include "GraphEditor/ReGraphAssetEditor.h"
#include "PropertyEditorModule.h"

void FReGraphBaseEditorModule::StartupModule()
{
	RegisterCustomizations();
}

void FReGraphBaseEditorModule::ShutdownModule()
{
	UnregisterCustomizations();
}

void FReGraphBaseEditorModule::OpenGraphAssetEditor(UReGraph* Graph, const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass)
{
	if (!Graph)
	{
		return;
	}

	TSharedRef<FReGraphAssetEditor> Editor = MakeShared<FReGraphAssetEditor>();
	Editor->Initialize(Graph, Mode, InitToolkitHost, EditorGraphClass, SchemaClass);
}

void FReGraphBaseEditorModule::RegisterCustomizations()
{
	FPropertyEditorModule& PropertyEditor = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReGraph"), FOnGetDetailCustomizationInstance::CreateStatic(&FReGraphDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomClassLayout(TEXT("ReGraphNode"), FOnGetDetailCustomizationInstance::CreateStatic(&FReGraphNodeDetailCustomization::MakeInstance));
	PropertyEditor.RegisterCustomPropertyTypeLayout(TEXT("ReGraphValue"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FReGraphValueCustomization::MakeInstance));
	PropertyEditor.RegisterCustomPropertyTypeLayout(TEXT("ReGraphVariableBinding"), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FReGraphVariableBindingCustomization::MakeInstance));
	PropertyEditor.NotifyCustomizationModuleChanged();
}

void FReGraphBaseEditorModule::UnregisterCustomizations()
{
	if (!FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		return;
	}

	FPropertyEditorModule& PropertyEditor = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReGraph"));
	PropertyEditor.UnregisterCustomClassLayout(TEXT("ReGraphNode"));
	PropertyEditor.UnregisterCustomPropertyTypeLayout(TEXT("ReGraphValue"));
	PropertyEditor.UnregisterCustomPropertyTypeLayout(TEXT("ReGraphVariableBinding"));
	PropertyEditor.NotifyCustomizationModuleChanged();
}

IMPLEMENT_MODULE(FReGraphBaseEditorModule, ReGraphBaseEditor)
