#include "Dialogue/ReDialogueRunner.h"

#include "Dialogue/ReDialogueAction.h"
#include "Dialogue/ReDialogueGraph.h"
#include "Dialogue/ReDialogueNode.h"
#include "Graph/ReGraphContext.h"
#include "Graph/ReGraphEdge.h"

bool UReDialogueRunner::StartDialogue(UReDialogueGraph* Graph)
{
	return StartDialogueBound(Graph, nullptr);
}

bool UReDialogueRunner::StartDialogueBound(UReDialogueGraph* Graph, UObject* BindingObject)
{
	if (!Graph)
	{
		return false;
	}

	ActiveGraph = Graph;
	RuntimeContext = NewObject<UReGraphContext>(this);
	RuntimeContext->Initialize(Graph, BindingObject);
	CurrentNode = nullptr;

	const TArray<UReGraphNode*> EntryNodes = Graph->GetEntryNodes();
	if (EntryNodes.Num() == 0)
	{
		return false;
	}

	return MoveToNode(Cast<UReDialogueNode>(EntryNodes[0])) && ResolveAutomaticFlow();
}

bool UReDialogueRunner::ChooseOption(int32 Index)
{
	UReDialogueChoiceNode* ChoiceNode = Cast<UReDialogueChoiceNode>(CurrentNode);
	if (!ChoiceNode || !ActiveGraph)
	{
		return false;
	}

	const TArray<UReGraphEdge*> AvailableEdges = ActiveGraph->GetAvailableTransitions(RuntimeContext, ChoiceNode);
	if (!AvailableEdges.IsValidIndex(Index))
	{
		return false;
	}

	return MoveToNode(Cast<UReDialogueNode>(AvailableEdges[Index]->ToNode)) && ResolveAutomaticFlow();
}

bool UReDialogueRunner::AdvanceDialogue()
{
	if (!ActiveGraph || !CurrentNode)
	{
		return false;
	}

	const TArray<UReGraphEdge*> AvailableEdges = ActiveGraph->GetAvailableTransitions(RuntimeContext, CurrentNode);
	if (AvailableEdges.Num() == 0)
	{
		return false;
	}

	return MoveToNode(Cast<UReDialogueNode>(AvailableEdges[0]->ToNode)) && ResolveAutomaticFlow();
}

FReDialogueLine UReDialogueRunner::GetCurrentLine() const
{
	if (const UReDialogueLineNode* LineNode = Cast<UReDialogueLineNode>(CurrentNode))
	{
		return LineNode->Line;
	}

	return FReDialogueLine();
}

TArray<FReDialogueChoice> UReDialogueRunner::GetChoices() const
{
	TArray<FReDialogueChoice> Result;
	const UReDialogueChoiceNode* ChoiceNode = Cast<UReDialogueChoiceNode>(CurrentNode);
	if (!ChoiceNode || !ActiveGraph || !RuntimeContext)
	{
		return Result;
	}

	const TArray<UReGraphEdge*> AvailableEdges = ActiveGraph->GetAvailableTransitions(RuntimeContext, const_cast<UReDialogueChoiceNode*>(ChoiceNode));
	for (int32 Index = 0; Index < AvailableEdges.Num(); ++Index)
	{
		FReDialogueChoice& Choice = Result.AddDefaulted_GetRef();
		Choice.ChoiceText = ChoiceNode->Choices.IsValidIndex(Index) ? ChoiceNode->Choices[Index].ChoiceText : FText::FromString(TEXT("Continue"));
		Choice.TransitionIndex = Index;
		Choice.TargetNodeId = AvailableEdges[Index] && AvailableEdges[Index]->ToNode ? AvailableEdges[Index]->ToNode->NodeId : FGuid();
	}

	return Result;
}

bool UReDialogueRunner::IsDialogueFinished() const
{
	return CurrentNode && Cast<UReDialogueEndNode>(CurrentNode) != nullptr;
}

void UReDialogueRunner::BroadcastActionRequest(FName EventName, EReDialogueActionType ActionType)
{
	OnActionRequested.Broadcast(EventName, ActionType);
}

bool UReDialogueRunner::MoveToNode(UReDialogueNode* Node)
{
	if (!Node || !ActiveGraph || !RuntimeContext)
	{
		return false;
	}

	CurrentNode = Node;
	RuntimeContext->ActiveNode = Node;
	if (!ActiveGraph->EvaluateNode(RuntimeContext, Node))
	{
		return false;
	}

	for (const UReDialogueAction* Action : Node->Actions)
	{
		if (Action)
		{
			Action->Execute(RuntimeContext, this);
		}
	}

	if (const UReDialogueEventNode* EventNode = Cast<UReDialogueEventNode>(Node))
	{
		if (!EventNode->EventName.IsNone())
		{
			BroadcastActionRequest(EventNode->EventName, EReDialogueActionType::TriggerEvent);
		}
	}

	if (const UReDialogueLineNode* LineNode = Cast<UReDialogueLineNode>(Node))
	{
		OnLineAdvanced.Broadcast(LineNode->Line);
	}

	return true;
}

bool UReDialogueRunner::ResolveAutomaticFlow()
{
	while (CurrentNode)
	{
		if (Cast<UReDialogueLineNode>(CurrentNode) || Cast<UReDialogueChoiceNode>(CurrentNode) || Cast<UReDialogueEndNode>(CurrentNode))
		{
			return true;
		}

		const TArray<UReGraphEdge*> AvailableEdges = ActiveGraph->GetAvailableTransitions(RuntimeContext, CurrentNode);
		if (AvailableEdges.Num() == 0)
		{
			return false;
		}

		if (!MoveToNode(Cast<UReDialogueNode>(AvailableEdges[0]->ToNode)))
		{
			return false;
		}
	}

	return false;
}
