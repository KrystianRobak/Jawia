#include "GraphEditor/ReGraphSchema.h"

#include "Graph/ReGraph.h"
#include "GraphEditor/ReGraphEdGraph.h"
#include "GraphEditor/ReGraphEdNode.h"

FReGraphSchemaAction_NewNode::FReGraphSchemaAction_NewNode()
	: FEdGraphSchemaAction()
{
}

FReGraphSchemaAction_NewNode::FReGraphSchemaAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, TSubclassOf<UReGraphNode> InNodeClass, TSubclassOf<UEdGraphNode> InEditorNodeClass)
	: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, FText::GetEmpty(), 0)
	, NodeClass(InNodeClass)
	, EditorNodeClass(InEditorNodeClass)
{
}

UEdGraphNode* FReGraphSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	UReGraphEdGraph* ReGraph = Cast<UReGraphEdGraph>(ParentGraph);
	if (!ReGraph || !ReGraph->RuntimeGraph)
	{
		return nullptr;
	}

	UClass* ResolvedNodeClass = NodeClass ? NodeClass.Get() : UReGraphNode::StaticClass();
	UReGraphNode* RuntimeNode = NewObject<UReGraphNode>(ReGraph->RuntimeGraph, ResolvedNodeClass);
	RuntimeNode->NodeName = *ResolvedNodeClass->GetName();
	RuntimeNode->DisplayTitle = FText::FromString(ResolvedNodeClass->GetName());
	ReGraph->RuntimeGraph->AddNode(RuntimeNode);

	UClass* ResolvedEditorNodeClass = EditorNodeClass ? EditorNodeClass.Get() : UReGraphEdNode::StaticClass();
	UReGraphEdNode* GraphNode = NewObject<UReGraphEdNode>(ParentGraph, ResolvedEditorNodeClass);
	GraphNode->Initialize(RuntimeNode);
	GraphNode->CreateNewGuid();
	GraphNode->PostPlacedNewNode();
	GraphNode->SetFlags(RF_Transactional);
	GraphNode->NodePosX = Location.X;
	GraphNode->NodePosY = Location.Y;
	ParentGraph->AddNode(GraphNode, true, bSelectNewNode);
	GraphNode->AllocateDefaultPins();

	if (FromPin)
	{
		UEdGraphPin* TargetPin = FromPin->Direction == EGPD_Output ? GraphNode->InputPin : GraphNode->GetPrimaryOutputPin();
		if (TargetPin)
		{
			FromPin->MakeLinkTo(TargetPin);
		}
	}

	ReGraph->SyncRuntimeGraphFromEditor();
	return GraphNode;
}

void UReGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReGraph", "NodeCategory", "Graph"),
		NSLOCTEXT("ReGraph", "GenericNode", "Add Graph Node"),
		UReGraphNode::StaticClass()));
}

const FPinConnectionResponse UReGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (!A || !B || A == B || A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid connection"));
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must face opposite directions"));
	}

	if (A->Direction == EGPD_Output && A->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	}

	if (B->Direction == EGPD_Output && B->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UReGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const bool bConnected = Super::TryCreateConnection(A, B);
	if (bConnected)
	{
		if (UReGraphEdGraph* Graph = Cast<UReGraphEdGraph>(A ? A->GetOwningNode()->GetGraph() : nullptr))
		{
			Graph->SyncRuntimeGraphFromEditor();
		}
	}

	return bConnected;
}

void UReGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
	if (UReGraphEdGraph* Graph = Cast<UReGraphEdGraph>(TargetPin.GetOwningNode()->GetGraph()))
	{
		Graph->SyncRuntimeGraphFromEditor();
	}
}

FLinearColor UReGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(0.75f, 0.75f, 0.2f);
}

bool UReGraphSchema::ShouldHidePinDefaultValue(UEdGraphPin* Pin) const
{
	return true;
}
