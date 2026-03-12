#pragma once

#include "CoreMinimal.h"
#include "ReGraphTypes.generated.h"

UENUM(BlueprintType)
enum class EReGraphValueType : uint8
{
	Bool,
	Int,
	Float,
	String
};

UENUM(BlueprintType)
enum class EReGraphComparisonOp : uint8
{
	Equal,
	NotEqual,
	Greater,
	GreaterOrEqual,
	Less,
	LessOrEqual
};

USTRUCT(BlueprintType)
struct REGRAPHBASERUNTIME_API FReGraphValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	EReGraphValueType Type = EReGraphValueType::Bool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	bool BoolValue = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	float FloatValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	FString StringValue;

	bool IsTruthy() const;
	FString ExportText() const;
};

USTRUCT(BlueprintType)
struct REGRAPHBASERUNTIME_API FReGraphTraversalStep
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	FGuid NodeId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Graph")
	FName NodeName;
};

USTRUCT(BlueprintType)
struct REGRAPHBASERUNTIME_API FReGraphVariableBinding
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	FName PropertyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Binding")
	bool bWriteBackToObject = true;
};
