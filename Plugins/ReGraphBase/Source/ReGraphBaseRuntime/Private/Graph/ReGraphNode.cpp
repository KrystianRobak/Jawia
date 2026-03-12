#include "Graph/ReGraphNode.h"

#include "Graph/ReGraphContext.h"
#include "Graph/ReGraphEdge.h"

UReGraphNode::UReGraphNode()
{
	NodeId = FGuid::NewGuid();
}

bool UReGraphNode::EvaluateNode_Implementation(UReGraphContext* Context)
{
	return Context != nullptr;
}

void UReGraphNode::AddOutgoingEdge(UReGraphEdge* Edge)
{
	if (Edge)
	{
		OutgoingEdges.AddUnique(Edge);
	}
}

void UReGraphNode::GetOutputPinNames(TArray<FName>& OutPinNames) const
{
	OutPinNames.Reset();
	OutPinNames.Add(TEXT("Out"));
}

void UReGraphNode::ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const
{
	TArray<FName> PinNames;
	GetOutputPinNames(PinNames);

	if (Edge && PinNames.IsValidIndex(PinIndex))
	{
		Edge->TransitionLabel = FText::FromName(PinNames[PinIndex]);
	}
}
