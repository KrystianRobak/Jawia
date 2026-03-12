#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/GCObject.h"

class IDetailsView;
class SGraphEditor;
class UEdGraphSchema;
class UReGraph;
class FUICommandList;
struct FPropertyChangedEvent;
struct FPropertyChangedChainEvent;

class REGRAPHBASEEDITOR_API FReGraphAssetEditor final : public FAssetEditorToolkit, public FGCObject
{
public:
	~FReGraphAssetEditor();

	void Initialize(UReGraph* InGraph, const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass);

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

private:
	void BindCommands();
	void DeleteSelectedNodes();
	bool CanDeleteSelectedNodes() const;
	void CopySelectedNodes();
	bool CanCopySelectedNodes() const;
	void CutSelectedNodes();
	bool CanCutSelectedNodes() const;
	void PasteNodes();
	bool CanPasteNodes() const;
	void DuplicateNodes();
	bool CanDuplicateNodes() const;
	void RefreshGraphFromDetails(const FPropertyChangedEvent& PropertyChangedEvent);
	void RefreshGraphFromObject(UObject* ChangedObject, const FPropertyChangedEvent& PropertyChangedEvent);
	void HandleObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent);
	void EnsureEditorGraph(UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass);
	void OnSelectedNodesChanged(const TSet<UObject*>& Selection);

	TSharedRef<SDockTab> SpawnGraphTab(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnAssetDetailsTab(const FSpawnTabArgs& Args) const;
	TSharedRef<SDockTab> SpawnSelectionDetailsTab(const FSpawnTabArgs& Args) const;

private:
	static const FName GraphTabId;
	static const FName AssetDetailsTabId;
	static const FName SelectionDetailsTabId;

	TObjectPtr<UReGraph> GraphAsset = nullptr;
	TSharedPtr<SGraphEditor> GraphEditorWidget;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedPtr<IDetailsView> AssetDetailsView;
	TSharedPtr<IDetailsView> SelectionDetailsView;
	FDelegateHandle PropertyChangedHandle;
};
