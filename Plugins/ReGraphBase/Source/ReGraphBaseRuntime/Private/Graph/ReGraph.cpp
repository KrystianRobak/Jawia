#include "Graph/ReGraph.h"

#include "Graph/ReGraphContext.h"
#include "Graph/ReGraphEdge.h"
#include "Graph/ReGraphNode.h"

void UReGraph::AddNode(UReGraphNode* Node)
{
	if (Node)
	{
		Nodes.AddUnique(Node);
	}
}

void UReGraph::RemoveNode(UReGraphNode* Node)
{
	if (!Node)
	{
		return;
	}

	Nodes.Remove(Node);

	for (int32 EdgeIndex = Edges.Num() - 1; EdgeIndex >= 0; --EdgeIndex)
	{
		UReGraphEdge* Edge = Edges[EdgeIndex];
		if (!Edge || Edge->FromNode == Node || Edge->ToNode == Node)
		{
			Edges.RemoveAt(EdgeIndex);
		}
	}

	for (UReGraphNode* OtherNode : Nodes)
	{
		if (!OtherNode)
		{
			continue;
		}

		for (int32 EdgeIndex = OtherNode->OutgoingEdges.Num() - 1; EdgeIndex >= 0; --EdgeIndex)
		{
			UReGraphEdge* Edge = OtherNode->OutgoingEdges[EdgeIndex];
			if (!Edge || Edge->FromNode == Node || Edge->ToNode == Node)
			{
				OtherNode->OutgoingEdges.RemoveAt(EdgeIndex);
			}
		}
	}
}

UReGraphEdge* UReGraph::ConnectNodes(UReGraphNode* FromNode, UReGraphNode* ToNode, TSubclassOf<UReGraphEdge> EdgeClass)
{
	if (!FromNode || !ToNode)
	{
		return nullptr;
	}

	UClass* ResolvedEdgeClass = EdgeClass ? EdgeClass.Get() : UReGraphEdge::StaticClass();
	UReGraphEdge* NewEdge = NewObject<UReGraphEdge>(this, ResolvedEdgeClass);
	NewEdge->Initialize(FromNode, ToNode);
	Edges.Add(NewEdge);
	FromNode->AddOutgoingEdge(NewEdge);
	return NewEdge;
}

TArray<UReGraphNode*> UReGraph::GetEntryNodes() const
{
	TArray<UReGraphNode*> Result;
	for (UReGraphNode* Node : Nodes)
	{
		if (Node && Node->bCanBeEntryPoint)
		{
			Result.Add(Node);
		}
	}

	return Result;
}

bool UReGraph::EvaluateNode(UReGraphContext* Context, UReGraphNode* Node) const
{
	if (!Context || !Node)
	{
		return false;
	}

	Context->ActiveNode = Node;
	Context->PushNodeVisit(Node);
	return Node->EvaluateNode(Context);
}

TArray<UReGraphEdge*> UReGraph::GetAvailableTransitions(UReGraphContext* Context, UReGraphNode* FromNode) const
{
	TArray<UReGraphEdge*> Result;
	if (!FromNode)
	{
		return Result;
	}

	for (UReGraphEdge* Edge : FromNode->OutgoingEdges)
	{
		if (Edge && Edge->EvaluateEdge(Context))
		{
			Result.Add(Edge);
		}
	}

	return Result;
}

UReGraphNode* UReGraph::Advance(UReGraphContext* Context, int32 TransitionIndex) const
{
	if (!Context)
	{
		return nullptr;
	}

	UReGraphNode* CurrentNode = Context->ActiveNode;
	if (!CurrentNode)
	{
		const TArray<UReGraphNode*> EntryNodes = GetEntryNodes();
		if (EntryNodes.Num() == 0)
		{
			return nullptr;
		}

		CurrentNode = EntryNodes[0];
		if (!EvaluateNode(Context, CurrentNode))
		{
			return nullptr;
		}
	}

	const TArray<UReGraphEdge*> AvailableTransitions = GetAvailableTransitions(Context, CurrentNode);
	if (!AvailableTransitions.IsValidIndex(TransitionIndex))
	{
		return CurrentNode;
	}

	UReGraphNode* NextNode = AvailableTransitions[TransitionIndex]->ToNode;
	if (!NextNode)
	{
		return nullptr;
	}

	EvaluateNode(Context, NextNode);
	return NextNode;
}
