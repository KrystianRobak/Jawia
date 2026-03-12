#pragma once

#include "IAssetTools.h"
#include "Modules/ModuleManager.h"

class IAssetTypeActions;

class FReGraphDialogueEditorModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	EAssetTypeCategories::Type GetAssetCategory() const;

private:
	void RegisterCustomizations();
	void UnregisterCustomizations();

	EAssetTypeCategories::Type AssetCategory = EAssetTypeCategories::Misc;
	TArray<TSharedPtr<IAssetTypeActions>> RegisteredAssetTypeActions;
};
