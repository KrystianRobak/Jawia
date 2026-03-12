#include "DialogueEditor/ReDialogueSchema.h"

#include "Dialogue/ReDialogueNode.h"
#include "DialogueEditor/ReDialogueEdNode.h"

void UReDialogueSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "StartNode", "Add Start Node"),
		UReDialogueStartNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "LineNode", "Add Dialogue Line Node"),
		UReDialogueLineNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "ChoiceNode", "Add Player Choice Node"),
		UReDialogueChoiceNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "ConditionalNode", "Add Conditional Node"),
		UReDialogueConditionalNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "EventNode", "Add Event Node"),
		UReDialogueEventNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
	ContextMenuBuilder.AddAction(MakeShared<FReGraphSchemaAction_NewNode>(
		NSLOCTEXT("ReDialogue", "DialogueCategory", "Dialogue"),
		NSLOCTEXT("ReDialogue", "EndNode", "Add End Node"),
		UReDialogueEndNode::StaticClass(),
		UReDialogueEdNode::StaticClass()));
}
