#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphTypes.h"
#include "UObject/Object.h"
#include "ReGraph.generated.h"

class UEdGraph;
class UReGraphContext;
class UReGraphEdge;
class UReGraphNode;
class UReGraphVariable;

UCLASS(BlueprintType)
class REGRAPHBASERUNTIME_API UReGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Graph")
	TArray<TObjectPtr<UReGraphNode>> Nodes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Graph")
	TArray<TObjectPtr<UReGraphEdge>> Edges;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Variables", meta = (TitleProperty = "Name"))
	TArray<TObjectPtr<UReGraphVariable>> DefaultVariables;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Binding")
	TSubclassOf<UObject> BoundObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Binding", meta = (TitleProperty = "VariableName"))
	TArray<FReGraphVariableBinding> VariableBindings;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> EditorGraph;
#endif

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void AddNode(UReGraphNode* Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void RemoveNode(UReGraphNode* Node);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	UReGraphEdge* ConnectNodes(UReGraphNode* FromNode, UReGraphNode* ToNode, TSubclassOf<UReGraphEdge> EdgeClass);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	TArray<UReGraphNode*> GetEntryNodes() const;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	bool EvaluateNode(UReGraphContext* Context, UReGraphNode* Node) const;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	TArray<UReGraphEdge*> GetAvailableTransitions(UReGraphContext* Context, UReGraphNode* FromNode) const;

	UFUNCTION(BlueprintCallable, Category = "Graph")
	UReGraphNode* Advance(UReGraphContext* Context, int32 TransitionIndex = 0) const;
};
