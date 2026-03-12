#pragma once

#include "Modules/ModuleManager.h"

class FReGraphDialogueRuntimeModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
