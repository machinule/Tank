// David Mihai/William Hess 2017

#include "Tank.h"
#include "Shell.h"

// Sets default values
AShell::AShell(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsGrabbed = false;
}