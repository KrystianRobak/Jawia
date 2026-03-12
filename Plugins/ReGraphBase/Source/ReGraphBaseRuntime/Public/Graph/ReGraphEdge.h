#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReGraphEdge.generated.h"

class UReGraphCondition;
class UReGraphContext;
class UReGraphNode;

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHBASERUNTIME_API UReGraphEdge : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	FGuid EdgeId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TObjectPtr<UReGraphNode> FromNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TObjectPtr<UReGraphNode> ToNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Graph")
	TObjectPtr<UReGraphCondition> Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	FText TransitionLabel;

	UReGraphEdge();

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void Initialize(UReGraphNode* InFromNode, UReGraphNode* InToNode);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool EvaluateEdge(UReGraphContext* Context) const;
};
