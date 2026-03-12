#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraph.h"
#include "ReDialogueGraph.generated.h"

class UReDialogueLineNode;

UCLASS(BlueprintType)
class REGRAPHDIALOGUERUNTIME_API UReDialogueGraph : public UReGraph
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	TArray<UReDialogueLineNode*> GetAllLineNodes() const;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	static UReDialogueGraph* CreateExampleDialogueAsset(UObject* Outer);
};
