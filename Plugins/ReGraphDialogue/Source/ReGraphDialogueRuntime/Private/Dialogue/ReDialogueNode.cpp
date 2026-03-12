#include "Dialogue/ReDialogueNode.h"

#include "Graph/ReGraphEdge.h"
#include "Graph/ReGraphContext.h"

bool UReDialogueNode::EvaluateNode_Implementation(UReGraphContext* Context)
{
	return Super::EvaluateNode_Implementation(Context);
}

void UReDialogueNode::GetOutputPinNames(TArray<FName>& OutPinNames) const
{
	Super::GetOutputPinNames(OutPinNames);
}

UReDialogueStartNode::UReDialogueStartNode()
{
	NodeType = EReDialogueNodeType::Start;
	NodeName = TEXT("Start");
	DisplayTitle = FText::FromString(TEXT("Start"));
	bCanBeEntryPoint = true;
}

UReDialogueLineNode::UReDialogueLineNode()
{
	NodeType = EReDialogueNodeType::Line;
	NodeName = TEXT("Line");
	DisplayTitle = FText::FromString(TEXT("Dialogue Line"));
}

UReDialogueChoiceNode::UReDialogueChoiceNode()
{
	NodeType = EReDialogueNodeType::Choice;
	NodeName = TEXT("Choice");
	DisplayTitle = FText::FromString(TEXT("Player Choice"));
	FReDialogueChoiceEntry& FirstChoice = Choices.AddDefaulted_GetRef();
	FirstChoice.PinName = TEXT("ChoiceA");
	FirstChoice.ChoiceText = FText::FromString(TEXT("Continue"));
}

void UReDialogueChoiceNode::GetOutputPinNames(TArray<FName>& OutPinNames) const
{
	OutPinNames.Reset();
	if (Choices.Num() == 0)
	{
		OutPinNames.Add(TEXT("Choice"));
		return;
	}

	for (const FReDialogueChoiceEntry& Choice : Choices)
	{
		OutPinNames.Add(Choice.PinName.IsNone() ? TEXT("Choice") : Choice.PinName);
	}
}

void UReDialogueChoiceNode::ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const
{
	Super::ConfigureEdgeForPin(PinIndex, Edge);
	if (Edge && Choices.IsValidIndex(PinIndex))
	{
		Edge->TransitionLabel = Choices[PinIndex].ChoiceText;
	}
}

UReDialogueConditionalNode::UReDialogueConditionalNode()
{
	NodeType = EReDialogueNodeType::Conditional;
	NodeName = TEXT("Conditional");
	DisplayTitle = FText::FromString(TEXT("Conditional"));
	FReDialogueConditionBranch& TrueBranch = Branches.AddDefaulted_GetRef();
	TrueBranch.PinName = TEXT("True");
	FReDialogueConditionBranch& FalseBranch = Branches.AddDefaulted_GetRef();
	FalseBranch.PinName = TEXT("False");
	FalseBranch.bIsDefaultBranch = true;
}

bool UReDialogueConditionalNode::EvaluateNode_Implementation(UReGraphContext* Context)
{
	return Super::EvaluateNode_Implementation(Context);
}

void UReDialogueConditionalNode::GetOutputPinNames(TArray<FName>& OutPinNames) const
{
	OutPinNames.Reset();
	if (Branches.Num() == 0)
	{
		OutPinNames.Add(TEXT("Out"));
		return;
	}

	for (const FReDialogueConditionBranch& Branch : Branches)
	{
		OutPinNames.Add(Branch.PinName.IsNone() ? TEXT("Branch") : Branch.PinName);
	}
}

void UReDialogueConditionalNode::ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const
{
	Super::ConfigureEdgeForPin(PinIndex, Edge);
	if (!Edge || !Branches.IsValidIndex(PinIndex))
	{
		return;
	}

	Edge->TransitionLabel = FText::FromName(Branches[PinIndex].PinName);
	Edge->Condition = Branches[PinIndex].bIsDefaultBranch ? nullptr : Branches[PinIndex].Condition;
}

UReDialogueEventNode::UReDialogueEventNode()
{
	NodeType = EReDialogueNodeType::Event;
	NodeName = TEXT("Event");
	DisplayTitle = FText::FromString(TEXT("Event"));
}

bool UReDialogueEventNode::EvaluateNode_Implementation(UReGraphContext* Context)
{
	if (!Super::EvaluateNode_Implementation(Context))
	{
		return false;
	}

	if (Context && !EventName.IsNone())
	{
		Context->EventLog.Add(EventName.ToString());
	}

	return true;
}

UReDialogueEndNode::UReDialogueEndNode()
{
	NodeType = EReDialogueNodeType::End;
	NodeName = TEXT("End");
	DisplayTitle = FText::FromString(TEXT("End"));
}
