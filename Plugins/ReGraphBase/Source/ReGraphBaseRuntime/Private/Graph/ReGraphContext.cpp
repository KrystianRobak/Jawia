#include "Graph/ReGraphContext.h"

#include "UObject/Field.h"
#include "UObject/UnrealType.h"
#include "Graph/ReGraph.h"
#include "Graph/ReGraphNode.h"
#include "Graph/ReGraphVariable.h"

void UReGraphContext::Initialize(UReGraph* InGraph, UObject* InBoundObject)
{
	Graph = InGraph;
	ActiveNode = nullptr;
	BoundObject = InBoundObject;
	if (Graph && BoundObject && Graph->BoundObjectClass && !BoundObject->IsA(Graph->BoundObjectClass))
	{
		BoundObject = nullptr;
	}
	Variables.Reset();
	TraversalHistory.Reset();
	EventLog.Reset();

	if (!Graph)
	{
		return;
	}

	for (UReGraphVariable* Variable : Graph->DefaultVariables)
	{
		if (Variable && !Variable->Name.IsNone())
		{
			Variables.FindOrAdd(Variable->Name) = Variable->Value;
		}
	}

	ApplyBindingsFromObject();
}

bool UReGraphContext::HasVariable(FName VariableName) const
{
	return Variables.Contains(VariableName);
}

FReGraphValue UReGraphContext::GetVariableValue(FName VariableName) const
{
	if (const FReGraphValue* Value = Variables.Find(VariableName))
	{
		return *Value;
	}

	return FReGraphValue();
}

void UReGraphContext::SetVariableValue(FName VariableName, const FReGraphValue& Value)
{
	Variables.FindOrAdd(VariableName) = Value;
	ApplyBindingToObject(VariableName, Value);
}

void UReGraphContext::ApplyBindingsFromObject()
{
	if (!Graph || !BoundObject)
	{
		return;
	}

	for (const FReGraphVariableBinding& Binding : Graph->VariableBindings)
	{
		if (Binding.VariableName.IsNone() || Binding.PropertyName.IsNone())
		{
			continue;
		}

		FProperty* Property = BoundObject->GetClass()->FindPropertyByName(Binding.PropertyName);
		if (!Property)
		{
			continue;
		}

		FReGraphValue Value;
		if (const FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			Value.Type = EReGraphValueType::Bool;
			Value.BoolValue = BoolProperty->GetPropertyValue_InContainer(BoundObject);
		}
		else if (const FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			Value.Type = EReGraphValueType::Int;
			Value.IntValue = IntProperty->GetPropertyValue_InContainer(BoundObject);
		}
		else if (const FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			Value.Type = EReGraphValueType::Float;
			Value.FloatValue = FloatProperty->GetPropertyValue_InContainer(BoundObject);
		}
		else if (const FStrProperty* StringProperty = CastField<FStrProperty>(Property))
		{
			Value.Type = EReGraphValueType::String;
			Value.StringValue = StringProperty->GetPropertyValue_InContainer(BoundObject);
		}
		else if (const FNameProperty* NameProperty = CastField<FNameProperty>(Property))
		{
			Value.Type = EReGraphValueType::String;
			Value.StringValue = NameProperty->GetPropertyValue_InContainer(BoundObject).ToString();
		}
		else
		{
			continue;
		}

		Variables.FindOrAdd(Binding.VariableName) = Value;
	}
}

void UReGraphContext::ApplyBindingToObject(FName VariableName, const FReGraphValue& Value)
{
	if (!Graph || !BoundObject)
	{
		return;
	}

	for (const FReGraphVariableBinding& Binding : Graph->VariableBindings)
	{
		if (Binding.VariableName != VariableName || Binding.PropertyName.IsNone() || !Binding.bWriteBackToObject)
		{
			continue;
		}

		FProperty* Property = BoundObject->GetClass()->FindPropertyByName(Binding.PropertyName);
		if (!Property)
		{
			continue;
		}

		if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
		{
			BoolProperty->SetPropertyValue_InContainer(BoundObject, Value.BoolValue);
		}
		else if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
		{
			IntProperty->SetPropertyValue_InContainer(BoundObject, Value.IntValue);
		}
		else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
		{
			FloatProperty->SetPropertyValue_InContainer(BoundObject, Value.FloatValue);
		}
		else if (FStrProperty* StringProperty = CastField<FStrProperty>(Property))
		{
			StringProperty->SetPropertyValue_InContainer(BoundObject, Value.StringValue);
		}
		else if (FNameProperty* NameProperty = CastField<FNameProperty>(Property))
		{
			NameProperty->SetPropertyValue_InContainer(BoundObject, FName(*Value.StringValue));
		}
	}
}

void UReGraphContext::PushNodeVisit(const UReGraphNode* Node)
{
	if (!Node)
	{
		return;
	}

	FReGraphTraversalStep& Step = TraversalHistory.AddDefaulted_GetRef();
	Step.NodeId = Node->NodeId;
	Step.NodeName = Node->NodeName;
}
