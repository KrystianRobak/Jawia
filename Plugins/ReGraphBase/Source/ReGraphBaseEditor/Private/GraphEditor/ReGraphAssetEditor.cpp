#include "GraphEditor/ReGraphAssetEditor.h"

#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphUtilities.h"
#include "Framework/Commands/GenericCommands.h"
#include "Framework/Docking/TabManager.h"
#include "Graph/ReGraph.h"
#include "Graph/ReGraphNode.h"
#include "GraphEditor/ReGraphEdGraph.h"
#include "GraphEditor/ReGraphEdNode.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "ScopedTransaction.h"
#include "HAL/PlatformApplicationMisc.h"
#include "UObject/UObjectGlobals.h"
#include "Widgets/Docking/SDockTab.h"

const FName FReGraphAssetEditor::GraphTabId(TEXT("ReGraphAssetEditor_Graph"));
const FName FReGraphAssetEditor::AssetDetailsTabId(TEXT("ReGraphAssetEditor_AssetDetails"));
const FName FReGraphAssetEditor::SelectionDetailsTabId(TEXT("ReGraphAssetEditor_SelectionDetails"));

FReGraphAssetEditor::~FReGraphAssetEditor()
{
	if (PropertyChangedHandle.IsValid())
	{
		FCoreUObjectDelegates::OnObjectPropertyChanged.Remove(PropertyChangedHandle);
	}
}

void FReGraphAssetEditor::Initialize(UReGraph* InGraph, const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass)
{
	GraphAsset = InGraph;
	EnsureEditorGraph(EditorGraphClass, SchemaClass);
	PropertyChangedHandle = FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &FReGraphAssetEditor::HandleObjectPropertyChanged);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs AssetDetailsArgs;
	AssetDetailsArgs.bHideSelectionTip = true;
	AssetDetailsArgs.NameAreaSettings = FDetailsViewArgs::ObjectsUseNameArea;
	AssetDetailsArgs.bUpdatesFromSelection = false;
	AssetDetailsView = PropertyEditorModule.CreateDetailView(AssetDetailsArgs);
	AssetDetailsView->SetObject(GraphAsset);
	AssetDetailsView->OnFinishedChangingProperties().AddRaw(this, &FReGraphAssetEditor::RefreshGraphFromDetails);

	FDetailsViewArgs SelectionDetailsArgs;
	SelectionDetailsArgs.bHideSelectionTip = true;
	SelectionDetailsArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	SelectionDetailsArgs.bUpdatesFromSelection = false;
	SelectionDetailsView = PropertyEditorModule.CreateDetailView(SelectionDetailsArgs);
	SelectionDetailsView->OnFinishedChangingProperties().AddRaw(this, &FReGraphAssetEditor::RefreshGraphFromDetails);

	BindCommands();

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateRaw(this, &FReGraphAssetEditor::OnSelectedNodesChanged);

	GraphEditorWidget = SNew(SGraphEditor)
		.GraphToEdit(GraphAsset->EditorGraph)
		.GraphEvents(GraphEvents)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true);

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("ReGraphAssetEditorLayout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Horizontal)
		->Split
		(
			FTabManager::NewStack()
			->AddTab(GraphTabId, ETabState::OpenedTab)
			->SetHideTabWell(true)
			->SetSizeCoefficient(0.72f)
		)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(AssetDetailsTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(SelectionDetailsTabId, ETabState::OpenedTab)
				->SetHideTabWell(true)
			)
			->SetSizeCoefficient(0.28f)
		)
	);

	InitAssetEditor(Mode, InitToolkitHost, GetToolkitFName(), Layout, true, true, GraphAsset);
}

void FReGraphAssetEditor::BindCommands()
{
	GraphEditorCommands = MakeShared<FUICommandList>();
	const FGenericCommands& GenericCommands = FGenericCommands::Get();

	GraphEditorCommands->MapAction(
		GenericCommands.Delete,
		FExecuteAction::CreateRaw(this, &FReGraphAssetEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CanDeleteSelectedNodes));
	GraphEditorCommands->MapAction(
		GenericCommands.Copy,
		FExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CanCopySelectedNodes));
	GraphEditorCommands->MapAction(
		GenericCommands.Cut,
		FExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CanCutSelectedNodes));
	GraphEditorCommands->MapAction(
		GenericCommands.Paste,
		FExecuteAction::CreateRaw(this, &FReGraphAssetEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CanPasteNodes));
	GraphEditorCommands->MapAction(
		GenericCommands.Duplicate,
		FExecuteAction::CreateRaw(this, &FReGraphAssetEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FReGraphAssetEditor::CanDuplicateNodes));
}

FName FReGraphAssetEditor::GetToolkitFName() const
{
	return FName(TEXT("ReGraphAssetEditor"));
}

FText FReGraphAssetEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("ReGraphAssetEditor", "ToolkitName", "ReGraph Editor");
}

FString FReGraphAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("ReGraph");
}

FLinearColor FReGraphAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.12f, 0.2f, 0.4f);
}

void FReGraphAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(GraphTabId, FOnSpawnTab::CreateRaw(this, &FReGraphAssetEditor::SpawnGraphTab))
		.SetDisplayName(NSLOCTEXT("ReGraphAssetEditor", "GraphTab", "Graph"));
	InTabManager->RegisterTabSpawner(AssetDetailsTabId, FOnSpawnTab::CreateRaw(this, &FReGraphAssetEditor::SpawnAssetDetailsTab))
		.SetDisplayName(NSLOCTEXT("ReGraphAssetEditor", "AssetDetailsTab", "Graph Details"));
	InTabManager->RegisterTabSpawner(SelectionDetailsTabId, FOnSpawnTab::CreateRaw(this, &FReGraphAssetEditor::SpawnSelectionDetailsTab))
		.SetDisplayName(NSLOCTEXT("ReGraphAssetEditor", "SelectionDetailsTab", "Selection"));
}

void FReGraphAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner(GraphTabId);
	InTabManager->UnregisterTabSpawner(AssetDetailsTabId);
	InTabManager->UnregisterTabSpawner(SelectionDetailsTabId);
}

void FReGraphAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(GraphAsset);
}

FString FReGraphAssetEditor::GetReferencerName() const
{
	return TEXT("FReGraphAssetEditor");
}

void FReGraphAssetEditor::DeleteSelectedNodes()
{
	if (!GraphEditorWidget.IsValid())
	{
		return;
	}

	UReGraphEdGraph* EditorGraph = Cast<UReGraphEdGraph>(GraphAsset ? GraphAsset->EditorGraph : nullptr);
	if (!EditorGraph)
	{
		return;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ReGraphAssetEditor", "DeleteNodes", "Delete Graph Nodes"));
	EditorGraph->Modify();
	GraphAsset->Modify();

	const FGraphPanelSelectionSet SelectedNodes = GraphEditorWidget->GetSelectedNodes();
	GraphEditorWidget->ClearSelectionSet();

	for (UObject* SelectedObject : SelectedNodes)
	{
		if (UReGraphEdNode* Node = Cast<UReGraphEdNode>(SelectedObject))
		{
			Node->Modify();
			EditorGraph->RemoveGraphNode(Node);
		}
	}

	if (AssetDetailsView.IsValid())
	{
		AssetDetailsView->SetObject(GraphAsset);
	}

	if (SelectionDetailsView.IsValid())
	{
		SelectionDetailsView->SetObject(nullptr);
	}
}

bool FReGraphAssetEditor::CanDeleteSelectedNodes() const
{
	return GraphEditorWidget.IsValid() && GraphEditorWidget->GetSelectedNodes().Num() > 0;
}

void FReGraphAssetEditor::CopySelectedNodes()
{
	if (!GraphEditorWidget.IsValid())
	{
		return;
	}

	FGraphPanelSelectionSet SelectedNodes = GraphEditorWidget->GetSelectedNodes();
	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FReGraphAssetEditor::CanCopySelectedNodes() const
{
	return GraphEditorWidget.IsValid() && GraphEditorWidget->GetSelectedNodes().Num() > 0;
}

void FReGraphAssetEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedNodes();
}

bool FReGraphAssetEditor::CanCutSelectedNodes() const
{
	return CanCopySelectedNodes() && CanDeleteSelectedNodes();
}

void FReGraphAssetEditor::PasteNodes()
{
	if (!GraphEditorWidget.IsValid() || !GraphAsset || !GraphAsset->EditorGraph)
	{
		return;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
	if (ClipboardContent.IsEmpty())
	{
		return;
	}

	UReGraphEdGraph* EditorGraph = Cast<UReGraphEdGraph>(GraphAsset->EditorGraph);
	if (!EditorGraph)
	{
		return;
	}

	const FScopedTransaction Transaction(NSLOCTEXT("ReGraphAssetEditor", "PasteNodes", "Paste Graph Nodes"));
	EditorGraph->Modify();
	GraphAsset->Modify();

	GraphEditorWidget->ClearSelectionSet();

	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EditorGraph, ClipboardContent, PastedNodes);

	for (UEdGraphNode* PastedNode : PastedNodes)
	{
		if (UReGraphEdNode* ReNode = Cast<UReGraphEdNode>(PastedNode))
		{
			if (ReNode->RuntimeNode)
			{
				ReNode->RuntimeNode->NodeId = FGuid::NewGuid();
				GraphAsset->AddNode(ReNode->RuntimeNode);
			}

			PastedNode->CreateNewGuid();
			PastedNode->NodePosX += 48;
			PastedNode->NodePosY += 48;
			PastedNode->SnapToGrid(16);
			GraphEditorWidget->SetNodeSelection(PastedNode, true);
		}
	}

	EditorGraph->SyncRuntimeGraphFromEditor();
	GraphEditorWidget->NotifyGraphChanged();
}

bool FReGraphAssetEditor::CanPasteNodes() const
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
	return !ClipboardContent.IsEmpty() && GraphEditorWidget.IsValid();
}

void FReGraphAssetEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FReGraphAssetEditor::CanDuplicateNodes() const
{
	return CanCopySelectedNodes();
}

void FReGraphAssetEditor::RefreshGraphFromDetails(const FPropertyChangedEvent& PropertyChangedEvent)
{
	RefreshGraphFromObject(nullptr, PropertyChangedEvent);
}

void FReGraphAssetEditor::RefreshGraphFromObject(UObject* ChangedObject, const FPropertyChangedEvent& PropertyChangedEvent)
{
	UReGraphEdGraph* EditorGraph = Cast<UReGraphEdGraph>(GraphAsset ? GraphAsset->EditorGraph : nullptr);
	if (!EditorGraph)
	{
		return;
	}

	bool bAnyNodeRefreshed = false;
	for (UEdGraphNode* GraphNode : EditorGraph->Nodes)
	{
		UReGraphEdNode* ReNode = Cast<UReGraphEdNode>(GraphNode);
		if (!ReNode || !ReNode->RuntimeNode)
		{
			continue;
		}

		const bool bRefreshThisNode = ChangedObject == nullptr
			|| ChangedObject == ReNode->RuntimeNode.Get()
			|| ChangedObject->IsIn(ReNode->RuntimeNode.Get());

		if (bRefreshThisNode)
		{
			ReNode->Modify();
			ReNode->RefreshPins();
			bAnyNodeRefreshed = true;
		}
	}

	if (bAnyNodeRefreshed || ChangedObject == GraphAsset)
	{
		EditorGraph->SyncRuntimeGraphFromEditor();
		if (GraphEditorWidget.IsValid())
		{
			GraphEditorWidget->NotifyGraphChanged();
		}
	}
}

void FReGraphAssetEditor::HandleObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (!GraphAsset || !ObjectBeingModified)
	{
		return;
	}

	if (ObjectBeingModified == GraphAsset || ObjectBeingModified->IsIn(GraphAsset))
	{
		RefreshGraphFromObject(ObjectBeingModified, PropertyChangedEvent);
	}
}

void FReGraphAssetEditor::EnsureEditorGraph(UClass* EditorGraphClass, TSubclassOf<UEdGraphSchema> SchemaClass)
{
	if (!GraphAsset)
	{
		return;
	}

	if (!GraphAsset->EditorGraph)
	{
		UClass* ResolvedGraphClass = EditorGraphClass ? EditorGraphClass : UReGraphEdGraph::StaticClass();
		UReGraphEdGraph* EditorGraph = NewObject<UReGraphEdGraph>(GraphAsset, ResolvedGraphClass, NAME_None, RF_Transactional);
		EditorGraph->Schema = SchemaClass ? SchemaClass.Get() : nullptr;
		EditorGraph->Initialize(GraphAsset);
	}
	else if (SchemaClass)
	{
		GraphAsset->EditorGraph->Schema = SchemaClass.Get();
	}
}

void FReGraphAssetEditor::OnSelectedNodesChanged(const TSet<UObject*>& Selection)
{
	TArray<UObject*> SelectionObjects;
	for (UObject* Item : Selection)
	{
		if (const UReGraphEdNode* GraphNode = Cast<UReGraphEdNode>(Item))
		{
			if (GraphNode->RuntimeNode)
			{
				SelectionObjects.Add(GraphNode->RuntimeNode.Get());
				continue;
			}
		}

		SelectionObjects.Add(Item);
	}

	if (SelectionObjects.Num() == 0)
	{
		SelectionDetailsView->SetObject(nullptr);
		return;
	}

	SelectionDetailsView->SetObjects(SelectionObjects);
}

TSharedRef<SDockTab> FReGraphAssetEditor::SpawnGraphTab(const FSpawnTabArgs& Args) const
{
	return SNew(SDockTab)
		.Label(NSLOCTEXT("ReGraphAssetEditor", "GraphTabLabel", "Graph"))
		[
			GraphEditorWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FReGraphAssetEditor::SpawnAssetDetailsTab(const FSpawnTabArgs& Args) const
{
	return SNew(SDockTab)
		.Label(NSLOCTEXT("ReGraphAssetEditor", "AssetDetailsLabel", "Graph Details"))
		[
			AssetDetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> FReGraphAssetEditor::SpawnSelectionDetailsTab(const FSpawnTabArgs& Args) const
{
	return SNew(SDockTab)
		.Label(NSLOCTEXT("ReGraphAssetEditor", "SelectionLabel", "Selection"))
		[
			SelectionDetailsView.ToSharedRef()
		];
}
