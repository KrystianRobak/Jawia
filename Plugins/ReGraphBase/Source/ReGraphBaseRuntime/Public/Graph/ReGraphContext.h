#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphTypes.h"
#include "UObject/Object.h"
#include "ReGraphContext.generated.h"

class UReGraph;
class UReGraphNode;
class UReGraphVariable;

UCLASS(BlueprintType)
class REGRAPHBASERUNTIME_API UReGraphContext : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TObjectPtr<UReGraph> Graph;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TObjectPtr<UReGraphNode> ActiveNode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TObjectPtr<UObject> BoundObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TMap<FName, FReGraphValue> Variables;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TArray<FReGraphTraversalStep> TraversalHistory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	TArray<FString> EventLog;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void Initialize(UReGraph* InGraph, UObject* InBoundObject = nullptr);

	UFUNCTION(BlueprintPure, Category = "Graph")
	bool HasVariable(FName VariableName) const;

	UFUNCTION(BlueprintPure, Category = "Graph")
	FReGraphValue GetVariableValue(FName VariableName) const;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void SetVariableValue(FName VariableName, const FReGraphValue& Value);

	void ApplyBindingsFromObject();
	void ApplyBindingToObject(FName VariableName, const FReGraphValue& Value);

	void PushNodeVisit(const UReGraphNode* Node);
};
