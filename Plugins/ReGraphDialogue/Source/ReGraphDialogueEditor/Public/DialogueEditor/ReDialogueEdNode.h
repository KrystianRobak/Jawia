#pragma once

#include "CoreMinimal.h"
#include "GraphEditor/ReGraphEdNode.h"
#include "ReDialogueEdNode.generated.h"

UCLASS()
class REGRAPHDIALOGUEEDITOR_API UReDialogueEdNode : public UReGraphEdNode
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
};
