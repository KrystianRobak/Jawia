#pragma once

#include "CoreMinimal.h"
#include "GraphEditor/ReGraphSchema.h"
#include "ReDialogueSchema.generated.h"

UCLASS()
class REGRAPHDIALOGUEEDITOR_API UReDialogueSchema : public UReGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
};
