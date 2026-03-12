#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphCondition.h"
#include "ReDialogueCondition.generated.h"

UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueCondition : public UReGraphVariableCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	bool bNegateResult = false;

	virtual bool Evaluate_Implementation(UReGraphContext* Context) const override;
};
