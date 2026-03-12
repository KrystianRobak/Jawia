#include "Dialogue/ReDialogueAction.h"

#include "Dialogue/ReDialogueRunner.h"
#include "Graph/ReGraphContext.h"

void UReDialogueAction::Execute(UReGraphContext* Context, UReDialogueRunner* Runner) const
{
	if (!Context)
	{
		return;
	}

	switch (ActionType)
	{
	case EReDialogueActionType::SetVariable:
		Context->SetVariableValue(VariableName, Value);
		break;
	case EReDialogueActionType::AddInt:
	{
		FReGraphValue Current = Context->GetVariableValue(VariableName);
		Current.Type = EReGraphValueType::Int;
		Current.IntValue += Value.IntValue;
		Context->SetVariableValue(VariableName, Current);
		break;
	}
	case EReDialogueActionType::AddFloat:
	{
		FReGraphValue Current = Context->GetVariableValue(VariableName);
		Current.Type = EReGraphValueType::Float;
		Current.FloatValue += Value.FloatValue;
		Context->SetVariableValue(VariableName, Current);
		break;
	}
	case EReDialogueActionType::TriggerEvent:
	case EReDialogueActionType::RequestBlueprintCallback:
		Context->EventLog.Add(EventName.ToString());
		if (Runner)
		{
			Runner->BroadcastActionRequest(EventName, ActionType);
		}
		break;
	default:
		break;
	}
}
