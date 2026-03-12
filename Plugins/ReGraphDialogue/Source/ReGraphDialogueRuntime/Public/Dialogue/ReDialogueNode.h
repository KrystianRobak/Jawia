#pragma once

#include "CoreMinimal.h"
#include "Dialogue/ReDialogueAction.h"
#include "Dialogue/ReDialogueCondition.h"
#include "Dialogue/ReDialogueTypes.h"
#include "Graph/ReGraphNode.h"
#include "ReDialogueNode.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueNode : public UReGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	EReDialogueNodeType NodeType = EReDialogueNodeType::Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Dialogue")
	TArray<TObjectPtr<UReDialogueAction>> Actions;

	virtual bool EvaluateNode_Implementation(UReGraphContext* Context) override;
	virtual void GetOutputPinNames(TArray<FName>& OutPinNames) const override;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueStartNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UReDialogueStartNode();
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueLineNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FReDialogueLine Line;

	UReDialogueLineNode();
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueChoiceNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (TitleProperty = "PinName"))
	TArray<FReDialogueChoiceEntry> Choices;

	UReDialogueChoiceNode();

	virtual void GetOutputPinNames(TArray<FName>& OutPinNames) const override;
	virtual void ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const override;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueConditionalNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (TitleProperty = "PinName"))
	TArray<FReDialogueConditionBranch> Branches;

	UReDialogueConditionalNode();

	virtual bool EvaluateNode_Implementation(UReGraphContext* Context) override;
	virtual void GetOutputPinNames(TArray<FName>& OutPinNames) const override;
	virtual void ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const override;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueEventNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName EventName;

	UReDialogueEventNode();

	virtual bool EvaluateNode_Implementation(UReGraphContext* Context) override;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHDIALOGUERUNTIME_API UReDialogueEndNode : public UReDialogueNode
{
	GENERATED_BODY()

public:
	UReDialogueEndNode();
};
