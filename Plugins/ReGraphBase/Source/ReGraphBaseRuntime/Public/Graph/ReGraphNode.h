#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ReGraphNode.generated.h"

class UReGraphContext;
class UReGraphEdge;

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHBASERUNTIME_API UReGraphNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	FGuid NodeId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	FName NodeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	FText DisplayTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	bool bCanBeEntryPoint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	FVector2D EditorPosition = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced, Category = "Graph")
	TArray<TObjectPtr<UReGraphEdge>> OutgoingEdges;

	UReGraphNode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Graph")
	bool EvaluateNode(UReGraphContext* Context);
	virtual bool EvaluateNode_Implementation(UReGraphContext* Context);

	UFUNCTION(BlueprintCallable, Category = "Graph")
	void AddOutgoingEdge(UReGraphEdge* Edge);

	virtual void GetOutputPinNames(TArray<FName>& OutPinNames) const;
	virtual void ConfigureEdgeForPin(int32 PinIndex, UReGraphEdge* Edge) const;
};
