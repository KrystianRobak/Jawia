#include "Graph/ReGraphTypes.h"

bool FReGraphValue::IsTruthy() const
{
	switch (Type)
	{
	case EReGraphValueType::Bool:
		return BoolValue;
	case EReGraphValueType::Int:
		return IntValue != 0;
	case EReGraphValueType::Float:
		return !FMath::IsNearlyZero(FloatValue);
	case EReGraphValueType::String:
		return !StringValue.IsEmpty();
	default:
		return false;
	}
}

FString FReGraphValue::ExportText() const
{
	switch (Type)
	{
	case EReGraphValueType::Bool:
		return BoolValue ? TEXT("true") : TEXT("false");
	case EReGraphValueType::Int:
		return FString::FromInt(IntValue);
	case EReGraphValueType::Float:
		return FString::SanitizeFloat(FloatValue);
	case EReGraphValueType::String:
		return StringValue;
	default:
		return FString();
	}
}
