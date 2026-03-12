#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphTypes.h"
#include "UObject/Object.h"
#include "ReGraphVariable.generated.h"

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHBASERUNTIME_API UReGraphVariable : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	FReGraphValue Value;

	UFUNCTION(BlueprintCallable, Category = "Variable")
	void SetValue(const FReGraphValue& InValue);
};
