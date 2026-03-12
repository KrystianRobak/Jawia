#include "GraphEditor/ReGraphFactory.h"

#include "Graph/ReGraph.h"
#include "GraphEditor/ReGraphEdGraph.h"
#include "GraphEditor/ReGraphSchema.h"

UReGraphFactory::UReGraphFactory()
{
	SupportedClass = UReGraph::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UReGraphFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UReGraph* Graph = NewObject<UReGraph>(InParent, Class, Name, Flags | RF_Transactional);
	UReGraphEdGraph* EditorGraph = NewObject<UReGraphEdGraph>(Graph, NAME_None, RF_Transactional);
	EditorGraph->Schema = UReGraphSchema::StaticClass();
	EditorGraph->Initialize(Graph);
	return Graph;
}
