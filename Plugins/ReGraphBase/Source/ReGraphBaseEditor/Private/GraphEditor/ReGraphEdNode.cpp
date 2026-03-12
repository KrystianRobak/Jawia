#include "GraphEditor/ReGraphEdNode.h"

#include "EdGraph/EdGraphPin.h"
#include "Graph/ReGraphNode.h"
#include "GraphEditor/ReGraphEdGraph.h"

void UReGraphEdNode::Initialize(UReGraphNode* InRuntimeNode)
{
	RuntimeNode = InRuntimeNode;
}

void UReGraphEdNode::AllocateDefaultPins()
{
	InputPin = CreatePin(EGPD_Input, TEXT("ReGraphFlow"), TEXT("In"));
	OutputPins.Reset();

	TArray<FName> OutputPinNames;
	if (RuntimeNode)
	{
		RuntimeNode->GetOutputPinNames(OutputPinNames);
	}

	if (OutputPinNames.Num() == 0)
	{
		OutputPinNames.Add(TEXT("Out"));
	}

	for (const FName& OutputPinName : OutputPinNames)
	{
		UEdGraphPin* NewPin = CreatePin(EGPD_Output, TEXT("ReGraphFlow"), OutputPinName);
		NewPin->PinFriendlyName = FText::FromName(OutputPinName);
		OutputPins.Add(NewPin);
	}
}

UEdGraphPin* UReGraphEdNode::GetPrimaryOutputPin() const
{
	return OutputPins.Num() > 0 ? OutputPins[0] : nullptr;
}

void UReGraphEdNode::RefreshPins()
{
	TArray<UEdGraphPin*> PreviousInputLinks;
	TMap<FName, TArray<UEdGraphPin*>> PreviousOutputLinks;

	if (InputPin)
	{
		PreviousInputLinks = InputPin->LinkedTo;
	}

	for (UEdGraphPin* ExistingOutputPin : OutputPins)
	{
		if (ExistingOutputPin)
		{
			PreviousOutputLinks.Add(ExistingOutputPin->PinName, ExistingOutputPin->LinkedTo);
		}
	}

	for (UEdGraphPin* ExistingPin : Pins)
	{
		if (ExistingPin)
		{
			ExistingPin->BreakAllPinLinks();
		}
	}

	Pins.Reset();
	InputPin = nullptr;
	OutputPins.Reset();
	AllocateDefaultPins();

	if (InputPin)
	{
		for (UEdGraphPin* LinkedPin : PreviousInputLinks)
		{
			if (LinkedPin)
			{
				InputPin->MakeLinkTo(LinkedPin);
			}
		}
	}

	for (UEdGraphPin* NewOutputPin : OutputPins)
	{
		if (!NewOutputPin)
		{
			continue;
		}

		if (const TArray<UEdGraphPin*>* LinkedPins = PreviousOutputLinks.Find(NewOutputPin->PinName))
		{
			for (UEdGraphPin* LinkedPin : *LinkedPins)
			{
				if (LinkedPin)
				{
					NewOutputPin->MakeLinkTo(LinkedPin);
				}
			}
		}
	}
}

FText UReGraphEdNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (RuntimeNode && !RuntimeNode->DisplayTitle.IsEmpty())
	{
		return RuntimeNode->DisplayTitle;
	}

	return RuntimeNode ? FText::FromName(RuntimeNode->NodeName) : FText::FromString(TEXT("ReGraph Node"));
}

FLinearColor UReGraphEdNode::GetNodeTitleColor() const
{
	return RuntimeNode && RuntimeNode->bCanBeEntryPoint ? FLinearColor(0.1f, 0.45f, 0.2f) : FLinearColor(0.12f, 0.12f, 0.12f);
}

void UReGraphEdNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);

	if (UReGraphEdGraph* Graph = Cast<UReGraphEdGraph>(GetGraph()))
	{
		Graph->SyncRuntimeGraphFromEditor();
	}
}

void UReGraphEdNode::PostEditUndo()
{
	Super::PostEditUndo();

	if (UReGraphEdGraph* Graph = Cast<UReGraphEdGraph>(GetGraph()))
	{
		Graph->SyncRuntimeGraphFromEditor();
	}
}

bool UReGraphEdNode::CanDuplicateNode() const
{
	return true;
}

bool UReGraphEdNode::CanUserDeleteNode() const
{
	return true;
}
