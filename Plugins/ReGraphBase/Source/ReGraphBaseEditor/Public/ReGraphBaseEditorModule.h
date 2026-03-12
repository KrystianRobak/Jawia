#pragma once

#include "Toolkits/IToolkitHost.h"
#include "Modules/ModuleManager.h"

class UEdGraph;
class UEdGraphSchema;
class UReGraph;

class REGRAPHBASEEDITOR_API FReGraphBaseEditorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void OpenGraphAssetEditor(UReGraph* Graph, const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass);

private:
	void RegisterCustomizations();
	void UnregisterCustomizations();
};
