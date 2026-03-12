#include "Graph/ReGraphCondition.h"

#include "Graph/ReGraphContext.h"

bool UReGraphCondition::Evaluate_Implementation(UReGraphContext* Context) const
{
	return Context != nullptr;
}

bool UReGraphVariableCondition::Evaluate_Implementation(UReGraphContext* Context) const
{
	if (!Context)
	{
		return false;
	}

	return CompareValues(Context->GetVariableValue(VariableName), Operator, ExpectedValue);
}

bool UReGraphVariableCondition::CompareValues(const FReGraphValue& Left, EReGraphComparisonOp InOperator, const FReGraphValue& Right)
{
	if (Left.Type != Right.Type)
	{
		return false;
	}

	switch (Left.Type)
	{
	case EReGraphValueType::Bool:
		return InOperator == EReGraphComparisonOp::Equal ? Left.BoolValue == Right.BoolValue
			: InOperator == EReGraphComparisonOp::NotEqual ? Left.BoolValue != Right.BoolValue : false;
	case EReGraphValueType::Int:
		switch (InOperator)
		{
		case EReGraphComparisonOp::Equal: return Left.IntValue == Right.IntValue;
		case EReGraphComparisonOp::NotEqual: return Left.IntValue != Right.IntValue;
		case EReGraphComparisonOp::Greater: return Left.IntValue > Right.IntValue;
		case EReGraphComparisonOp::GreaterOrEqual: return Left.IntValue >= Right.IntValue;
		case EReGraphComparisonOp::Less: return Left.IntValue < Right.IntValue;
		case EReGraphComparisonOp::LessOrEqual: return Left.IntValue <= Right.IntValue;
		default: return false;
		}
	case EReGraphValueType::Float:
		switch (InOperator)
		{
		case EReGraphComparisonOp::Equal: return FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue);
		case EReGraphComparisonOp::NotEqual: return !FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue);
		case EReGraphComparisonOp::Greater: return Left.FloatValue > Right.FloatValue;
		case EReGraphComparisonOp::GreaterOrEqual: return Left.FloatValue >= Right.FloatValue;
		case EReGraphComparisonOp::Less: return Left.FloatValue < Right.FloatValue;
		case EReGraphComparisonOp::LessOrEqual: return Left.FloatValue <= Right.FloatValue;
		default: return false;
		}
	case EReGraphValueType::String:
		return InOperator == EReGraphComparisonOp::Equal ? Left.StringValue == Right.StringValue
			: InOperator == EReGraphComparisonOp::NotEqual ? Left.StringValue != Right.StringValue : false;
	default:
		return false;
	}
}
