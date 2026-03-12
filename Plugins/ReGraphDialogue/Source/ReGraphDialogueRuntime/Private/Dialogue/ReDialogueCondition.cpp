#include "Dialogue/ReDialogueCondition.h"

bool UReDialogueCondition::Evaluate_Implementation(UReGraphContext* Context) const
{
	const bool bResult = Super::Evaluate_Implementation(Context);
	return bNegateResult ? !bResult : bResult;
}
