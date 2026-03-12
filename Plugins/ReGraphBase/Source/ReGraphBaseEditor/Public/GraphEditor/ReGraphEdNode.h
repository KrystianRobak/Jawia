#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "ReGraphEdNode.generated.h"

class UReGraphNode;
class UEdGraphPin;

UCLASS()
class REGRAPHBASEEDITOR_API UReGraphEdNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UReGraphNode> RuntimeNode;

	UEdGraphPin* InputPin = nullptr;

	TArray<UEdGraphPin*> OutputPins;

	void Initialize(UReGraphNode* InRuntimeNode);
	UEdGraphPin* GetPrimaryOutputPin() const;
	void RefreshPins();

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void PostEditUndo() override;
	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;
};
