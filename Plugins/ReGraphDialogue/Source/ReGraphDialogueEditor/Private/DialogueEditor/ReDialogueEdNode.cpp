#include "DialogueEditor/ReDialogueEdNode.h"

#include "Dialogue/ReDialogueNode.h"

FText UReDialogueEdNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	const UReDialogueNode* DialogueNode = Cast<UReDialogueNode>(RuntimeNode);
	if (!DialogueNode)
	{
		return Super::GetNodeTitle(TitleType);
	}

	if (const UReDialogueLineNode* LineNode = Cast<UReDialogueLineNode>(DialogueNode))
	{
		if (!LineNode->Line.Speaker.IsNone() && !LineNode->Line.Text.IsEmpty())
		{
			return FText::Format(FText::FromString(TEXT("{0}: {1}")), FText::FromName(LineNode->Line.Speaker), LineNode->Line.Text);
		}
	}

	if (const UReDialogueChoiceNode* ChoiceNode = Cast<UReDialogueChoiceNode>(DialogueNode))
	{
		return FText::Format(FText::FromString(TEXT("Choices ({0})")), FText::AsNumber(ChoiceNode->Choices.Num()));
	}

	if (const UReDialogueConditionalNode* ConditionalNode = Cast<UReDialogueConditionalNode>(DialogueNode))
	{
		return FText::Format(FText::FromString(TEXT("Conditional ({0})")), FText::AsNumber(ConditionalNode->Branches.Num()));
	}

	if (const UReDialogueEventNode* EventNode = Cast<UReDialogueEventNode>(DialogueNode))
	{
		if (!EventNode->EventName.IsNone())
		{
			return FText::Format(FText::FromString(TEXT("Event: {0}")), FText::FromName(EventNode->EventName));
		}
	}

	return Super::GetNodeTitle(TitleType);
}

FLinearColor UReDialogueEdNode::GetNodeTitleColor() const
{
	const UReDialogueNode* DialogueNode = Cast<UReDialogueNode>(RuntimeNode);
	if (!DialogueNode)
	{
		return Super::GetNodeTitleColor();
	}

	switch (DialogueNode->NodeType)
	{
	case EReDialogueNodeType::Start:
		return FLinearColor(0.08f, 0.4f, 0.18f);
	case EReDialogueNodeType::Line:
		return FLinearColor(0.12f, 0.22f, 0.48f);
	case EReDialogueNodeType::Choice:
		return FLinearColor(0.45f, 0.22f, 0.08f);
	case EReDialogueNodeType::Conditional:
		return FLinearColor(0.48f, 0.4f, 0.1f);
	case EReDialogueNodeType::Event:
		return FLinearColor(0.48f, 0.08f, 0.24f);
	case EReDialogueNodeType::End:
		return FLinearColor(0.2f, 0.2f, 0.2f);
	default:
		return Super::GetNodeTitleColor();
	}
}
