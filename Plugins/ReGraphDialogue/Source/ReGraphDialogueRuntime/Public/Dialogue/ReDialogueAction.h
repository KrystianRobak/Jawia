#pragma once

#include "CoreMinimal.h"
#include "Dialogue/ReDialogueTypes.h"
#include "Graph/ReGraphTypes.h"
#include "UObject/Object.h"
#include "ReDialogueAction.generated.h"

class UReGraphContext;
class UReDialogueRunner;

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EReDialogueActionType ActionType = EReDialogueActionType::TriggerEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FReGraphValue Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName EventName;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void Execute(UReGraphContext* Context, UReDialogueRunner* Runner) const;
};
