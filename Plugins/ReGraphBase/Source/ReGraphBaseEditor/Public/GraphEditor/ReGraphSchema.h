#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "Graph/ReGraphNode.h"
#include "ReGraphSchema.generated.h"

struct REGRAPHBASEEDITOR_API FReGraphSchemaAction_NewNode : public FEdGraphSchemaAction
{
public:
	TSubclassOf<UReGraphNode> NodeClass;
	TSubclassOf<UEdGraphNode> EditorNodeClass;

	FReGraphSchemaAction_NewNode();
	FReGraphSchemaAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, TSubclassOf<UReGraphNode> InNodeClass, TSubclassOf<UEdGraphNode> InEditorNodeClass = nullptr);

	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
};

UCLASS()
class REGRAPHBASEEDITOR_API UReGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual bool ShouldHidePinDefaultValue(UEdGraphPin* Pin) const override;
};
