#include "GraphEditor/ReGraphEdGraph.h"

#include "EdGraph/EdGraphNode.h"
#include "Graph/ReGraph.h"
#include "Graph/ReGraphEdge.h"
#include "Graph/ReGraphNode.h"
#include "GraphEditor/ReGraphEdNode.h"

void UReGraphEdGraph::Initialize(UReGraph* InRuntimeGraph)
{
	RuntimeGraph = InRuntimeGraph;
	if (RuntimeGraph)
	{
		RuntimeGraph->EditorGraph = this;
	}
}

void UReGraphEdGraph::SyncRuntimeGraphFromEditor()
{
	if (!RuntimeGraph)
	{
		return;
	}

	RuntimeGraph->Edges.Reset();
	for (UReGraphNode* Node : RuntimeGraph->Nodes)
	{
		if (Node)
		{
			Node->OutgoingEdges.Reset();
		}
	}

	for (UEdGraphNode* GraphNode : Nodes)
	{
		const UReGraphEdNode* ReNode = Cast<UReGraphEdNode>(GraphNode);
		if (!ReNode || !ReNode->RuntimeNode)
		{
			continue;
		}

		ReNode->RuntimeNode->EditorPosition = FVector2D(GraphNode->NodePosX, GraphNode->NodePosY);

		for (int32 OutputIndex = 0; OutputIndex < ReNode->OutputPins.Num(); ++OutputIndex)
		{
			UEdGraphPin* OutputPin = ReNode->OutputPins[OutputIndex];
			if (!OutputPin)
			{
				continue;
			}

			for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
			{
				const UReGraphEdNode* LinkedNode = LinkedPin ? Cast<UReGraphEdNode>(LinkedPin->GetOwningNode()) : nullptr;
				if (LinkedNode && LinkedNode->RuntimeNode)
				{
					UReGraphEdge* NewEdge = RuntimeGraph->ConnectNodes(ReNode->RuntimeNode, LinkedNode->RuntimeNode, UReGraphEdge::StaticClass());
					ReNode->RuntimeNode->ConfigureEdgeForPin(OutputIndex, NewEdge);
				}
			}
		}
	}
}

void UReGraphEdGraph::RemoveGraphNode(UReGraphEdNode* NodeToRemove)
{
	if (!NodeToRemove)
	{
		return;
	}

	Modify();

	if (RuntimeGraph && NodeToRemove->RuntimeNode)
	{
		RuntimeGraph->Modify();
		RuntimeGraph->RemoveNode(NodeToRemove->RuntimeNode);
	}

	RemoveNode(NodeToRemove);
	SyncRuntimeGraphFromEditor();
}
