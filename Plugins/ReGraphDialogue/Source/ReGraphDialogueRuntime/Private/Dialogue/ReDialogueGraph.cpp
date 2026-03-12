#include "Dialogue/ReDialogueGraph.h"

#include "Dialogue/ReDialogueAction.h"
#include "Dialogue/ReDialogueCondition.h"
#include "Dialogue/ReDialogueNode.h"
#include "Graph/ReGraphVariable.h"

TArray<UReDialogueLineNode*> UReDialogueGraph::GetAllLineNodes() const
{
	TArray<UReDialogueLineNode*> Result;
	for (UReGraphNode* Node : Nodes)
	{
		if (UReDialogueLineNode* LineNode = Cast<UReDialogueLineNode>(Node))
		{
			Result.Add(LineNode);
		}
	}

	return Result;
}

UReDialogueGraph* UReDialogueGraph::CreateExampleDialogueAsset(UObject* Outer)
{
	UObject* GraphOuter = Outer ? Outer : GetTransientPackage();
	UReDialogueGraph* Graph = NewObject<UReDialogueGraph>(GraphOuter, UReDialogueGraph::StaticClass());

	UReDialogueStartNode* Start = NewObject<UReDialogueStartNode>(Graph);
	UReDialogueLineNode* Greeting = NewObject<UReDialogueLineNode>(Graph);
	UReDialogueChoiceNode* Choice = NewObject<UReDialogueChoiceNode>(Graph);
	UReDialogueConditionalNode* ReputationGate = NewObject<UReDialogueConditionalNode>(Graph);
	UReDialogueLineNode* Accept = NewObject<UReDialogueLineNode>(Graph);
	UReDialogueLineNode* Decline = NewObject<UReDialogueLineNode>(Graph);
	UReDialogueEventNode* QuestStarted = NewObject<UReDialogueEventNode>(Graph);
	UReDialogueEndNode* End = NewObject<UReDialogueEndNode>(Graph);

	UReGraphVariable* Reputation = NewObject<UReGraphVariable>(Graph);
	Reputation->Name = TEXT("Reputation");
	Reputation->Value.Type = EReGraphValueType::Int;
	Reputation->Value.IntValue = 1;
	Graph->DefaultVariables.Add(Reputation);

	Greeting->Line.Speaker = TEXT("Guide");
	Greeting->Line.Text = FText::FromString(TEXT("The lower district is locked down. Will you help?"));
	Choice->Choices.Reset();
	FReDialogueChoiceEntry& AcceptChoice = Choice->Choices.AddDefaulted_GetRef();
	AcceptChoice.PinName = TEXT("Accept");
	AcceptChoice.ChoiceText = FText::FromString(TEXT("I can help."));
	FReDialogueChoiceEntry& DeclineChoice = Choice->Choices.AddDefaulted_GetRef();
	DeclineChoice.PinName = TEXT("Decline");
	DeclineChoice.ChoiceText = FText::FromString(TEXT("Not now."));
	Accept->Line.Speaker = TEXT("Guide");
	Accept->Line.Text = FText::FromString(TEXT("Good. I'll mark the patrol routes on your map."));
	Decline->Line.Speaker = TEXT("Guide");
	Decline->Line.Text = FText::FromString(TEXT("Then stay clear of the square until morning."));
	QuestStarted->EventName = TEXT("Dialogue.QuestStarted");

	ReputationGate->Branches.Reset();
	FReDialogueConditionBranch& AllowedBranch = ReputationGate->Branches.AddDefaulted_GetRef();
	AllowedBranch.PinName = TEXT("Trusted");
	AllowedBranch.Condition = NewObject<UReDialogueCondition>(ReputationGate);
	AllowedBranch.Condition->VariableName = TEXT("Reputation");
	AllowedBranch.Condition->Operator = EReGraphComparisonOp::GreaterOrEqual;
	AllowedBranch.Condition->ExpectedValue.Type = EReGraphValueType::Int;
	AllowedBranch.Condition->ExpectedValue.IntValue = 1;
	FReDialogueConditionBranch& RejectedBranch = ReputationGate->Branches.AddDefaulted_GetRef();
	RejectedBranch.PinName = TEXT("Fallback");
	RejectedBranch.bIsDefaultBranch = true;

	UReDialogueAction* GainTrust = NewObject<UReDialogueAction>(Accept);
	GainTrust->ActionType = EReDialogueActionType::AddInt;
	GainTrust->VariableName = TEXT("Reputation");
	GainTrust->Value.Type = EReGraphValueType::Int;
	GainTrust->Value.IntValue = 1;
	Accept->Actions.Add(GainTrust);

	Graph->AddNode(Start);
	Graph->AddNode(Greeting);
	Graph->AddNode(Choice);
	Graph->AddNode(ReputationGate);
	Graph->AddNode(Accept);
	Graph->AddNode(Decline);
	Graph->AddNode(QuestStarted);
	Graph->AddNode(End);

	Graph->ConnectNodes(Start, Greeting, nullptr);
	Graph->ConnectNodes(Greeting, Choice, nullptr);
	UReGraphEdge* AcceptEdge = Graph->ConnectNodes(Choice, ReputationGate, nullptr);
	Choice->ConfigureEdgeForPin(0, AcceptEdge);
	UReGraphEdge* DeclineEdge = Graph->ConnectNodes(Choice, Decline, nullptr);
	Choice->ConfigureEdgeForPin(1, DeclineEdge);
	UReGraphEdge* TrustedEdge = Graph->ConnectNodes(ReputationGate, Accept, nullptr);
	ReputationGate->ConfigureEdgeForPin(0, TrustedEdge);
	UReGraphEdge* FallbackEdge = Graph->ConnectNodes(ReputationGate, Decline, nullptr);
	ReputationGate->ConfigureEdgeForPin(1, FallbackEdge);
	Graph->ConnectNodes(Accept, QuestStarted, nullptr);
	Graph->ConnectNodes(QuestStarted, End, nullptr);
	Graph->ConnectNodes(Decline, End, nullptr);
	return Graph;
}
