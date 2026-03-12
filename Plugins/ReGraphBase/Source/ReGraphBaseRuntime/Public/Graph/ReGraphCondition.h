#pragma once

#include "CoreMinimal.h"
#include "Graph/ReGraphTypes.h"
#include "UObject/Object.h"
#include "ReGraphCondition.generated.h"

class UReGraphContext;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class REGRAPHBASERUNTIME_API UReGraphCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Graph")
	bool Evaluate(UReGraphContext* Context) const;
	virtual bool Evaluate_Implementation(UReGraphContext* Context) const;
};

UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class REGRAPHBASERUNTIME_API UReGraphVariableCondition : public UReGraphCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	EReGraphComparisonOp Operator = EReGraphComparisonOp::Equal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FReGraphValue ExpectedValue;

	virtual bool Evaluate_Implementation(UReGraphContext* Context) const override;

	static bool CompareValues(const FReGraphValue& Left, EReGraphComparisonOp InOperator, const FReGraphValue& Right);
};
