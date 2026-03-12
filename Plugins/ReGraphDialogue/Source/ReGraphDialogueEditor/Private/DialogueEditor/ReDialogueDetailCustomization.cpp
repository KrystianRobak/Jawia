#include "DialogueEditor/ReDialogueDetailCustomization.h"

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Dialogue/ReDialogueNode.h"
#include "Widgets/Text/STextBlock.h"

TSharedRef<IDetailCustomization> FReDialogueNodeDetailCustomization::MakeInstance()
{
	return MakeShared<FReDialogueNodeDetailCustomization>();
}

void FReDialogueNodeDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, NodeId));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, NodeName));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, bCanBeEntryPoint));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, EditorPosition));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, OutgoingEdges));
	DetailBuilder.HideProperty(GET_MEMBER_NAME_CHECKED(UReDialogueNode, NodeType));

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	UReDialogueNode* DialogueNode = Objects.Num() > 0 ? Cast<UReDialogueNode>(Objects[0].Get()) : nullptr;
	if (!DialogueNode)
	{
		return;
	}

	IDetailCategoryBuilder& OverviewCategory = DetailBuilder.EditCategory(TEXT("Overview"), FText::FromString(TEXT("Overview")), ECategoryPriority::Important);
	OverviewCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReGraphNode, DisplayTitle));

	if (DialogueNode->IsA<UReDialogueLineNode>())
	{
		IDetailCategoryBuilder& LineCategory = DetailBuilder.EditCategory(TEXT("Line"), FText::FromString(TEXT("Dialogue Line")), ECategoryPriority::Important);
		LineCategory.AddCustomRow(FText::FromString(TEXT("LineHint")))
		.WholeRowContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Author the spoken line here. Use Actions to mutate state or trigger effects as the line is reached.")))
			.AutoWrapText(true)
		];
		LineCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReDialogueLineNode, Line));
	}
	else if (DialogueNode->IsA<UReDialogueChoiceNode>())
	{
		IDetailCategoryBuilder& ChoiceCategory = DetailBuilder.EditCategory(TEXT("Choices"), FText::FromString(TEXT("Player Choices")), ECategoryPriority::Important);
		ChoiceCategory.AddCustomRow(FText::FromString(TEXT("ChoiceHint")))
		.WholeRowContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Each array entry creates one output pin. Rename Pin Name to control the visible branch label in the graph.")))
			.AutoWrapText(true)
		];
		ChoiceCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReDialogueChoiceNode, Choices));
	}
	else if (DialogueNode->IsA<UReDialogueConditionalNode>())
	{
		IDetailCategoryBuilder& ConditionalCategory = DetailBuilder.EditCategory(TEXT("Condition"), FText::FromString(TEXT("Condition")), ECategoryPriority::Important);
		ConditionalCategory.AddCustomRow(FText::FromString(TEXT("ConditionHint")))
		.WholeRowContent()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Each branch creates one output pin. The first branch whose condition passes wins. Mark one branch as default to use it as fallback.")))
			.AutoWrapText(true)
		];
		ConditionalCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReDialogueConditionalNode, Branches));
	}
	else if (DialogueNode->IsA<UReDialogueEventNode>())
	{
		IDetailCategoryBuilder& EventCategory = DetailBuilder.EditCategory(TEXT("Event"), FText::FromString(TEXT("Event")), ECategoryPriority::Important);
		EventCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReDialogueEventNode, EventName));
	}

	if (!DialogueNode->IsA<UReDialogueStartNode>() && !DialogueNode->IsA<UReDialogueEndNode>())
	{
		IDetailCategoryBuilder& ActionsCategory = DetailBuilder.EditCategory(TEXT("Actions"), FText::FromString(TEXT("Actions")), ECategoryPriority::Default);
		ActionsCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UReDialogueNode, Actions));
	}
}
