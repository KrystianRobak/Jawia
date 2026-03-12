#include "Graph/ReGraphEdge.h"

#include "Graph/ReGraphCondition.h"
#include "Graph/ReGraphContext.h"
#include "Graph/ReGraphNode.h"

UReGraphEdge::UReGraphEdge()
{
	EdgeId = FGuid::NewGuid();
}

void UReGraphEdge::Initialize(UReGraphNode* InFromNode, UReGraphNode* InToNode)
{
	FromNode = InFromNode;
	ToNode = InToNode;
}

bool UReGraphEdge::EvaluateEdge(UReGraphContext* Context) const
{
	return !Condition || Condition->Evaluate(Context);
}
