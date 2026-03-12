#pragma once

#include "CoreMinimal.h"
#include "Dialogue/ReDialogueTypes.h"
#include "UObject/Object.h"
#include "ReDialogueRunner.generated.h"

class UReDialogueGraph;
class UReDialogueNode;
class UReGraphContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReDialogueActionRequestedSignature, FName, EventName, EReDialogueActionType, ActionType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReDialogueLineAdvancedSignature, const FReDialogueLine&, Line);

UCLASS(BlueprintType)
class REGRAPHDIALOGUERUNTIME_API UReDialogueRunner : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FReDialogueActionRequestedSignature OnActionRequested;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FReDialogueLineAdvancedSignature OnLineAdvanced;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UReDialogueGraph> ActiveGraph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UReGraphContext> RuntimeContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UReDialogueNode> CurrentNode;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogue(UReDialogueGraph* Graph);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogueBound(UReDialogueGraph* Graph, UObject* BindingObject);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool ChooseOption(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool AdvanceDialogue();

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	FReDialogueLine GetCurrentLine() const;

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	TArray<FReDialogueChoice> GetChoices() const;

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	bool IsDialogueFinished() const;

	void BroadcastActionRequest(FName EventName, EReDialogueActionType ActionType);

protected:
	bool MoveToNode(UReDialogueNode* Node);
	bool ResolveAutomaticFlow();
};
