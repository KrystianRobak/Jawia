#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "ReGraphEdGraph.generated.h"

class UReGraph;

UCLASS()
class REGRAPHBASEEDITOR_API UReGraphEdGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UReGraph> RuntimeGraph;

	void Initialize(UReGraph* InRuntimeGraph);
	void SyncRuntimeGraphFromEditor();
	void RemoveGraphNode(class UReGraphEdNode* NodeToRemove);
};
