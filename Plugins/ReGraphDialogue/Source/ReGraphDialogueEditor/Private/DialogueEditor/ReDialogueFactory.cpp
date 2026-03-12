#include "DialogueEditor/ReDialogueFactory.h"

#include "Dialogue/ReDialogueGraph.h"
#include "Dialogue/ReDialogueNode.h"
#include "DialogueEditor/ReDialogueEdGraph.h"
#include "DialogueEditor/ReDialogueEdNode.h"
#include "DialogueEditor/ReDialogueSchema.h"
#include "Modules/ModuleManager.h"
#include "ReGraphDialogueEditorModule.h"

UReDialogueFactory::UReDialogueFactory()
{
	SupportedClass = UReDialogueGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UReDialogueFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UReDialogueGraph* Graph = NewObject<UReDialogueGraph>(InParent, Class, Name, Flags | RF_Transactional);
	UReDialogueEdGraph* EditorGraph = NewObject<UReDialogueEdGraph>(Graph, NAME_None, RF_Transactional);
	EditorGraph->Schema = UReDialogueSchema::StaticClass();
	EditorGraph->Initialize(Graph);

	UReDialogueStartNode* StartNode = NewObject<UReDialogueStartNode>(Graph);
	Graph->AddNode(StartNode);

	UReDialogueEdNode* EditorStartNode = NewObject<UReDialogueEdNode>(EditorGraph, NAME_None, RF_Transactional);
	EditorStartNode->Initialize(StartNode);
	EditorStartNode->CreateNewGuid();
	EditorStartNode->PostPlacedNewNode();
	EditorStartNode->SetFlags(RF_Transactional);
	EditorStartNode->NodePosX = 0;
	EditorStartNode->NodePosY = 0;
	EditorGraph->AddNode(EditorStartNode, true, false);
	EditorStartNode->AllocateDefaultPins();
	return Graph;
}

bool UReDialogueFactory::ShouldShowInNewMenu() const
{
	return true;
}

uint32 UReDialogueFactory::GetMenuCategories() const
{
	return FModuleManager::LoadModuleChecked<FReGraphDialogueEditorModule>("ReGraphDialogueEditor").GetAssetCategory();
}
