#include "ComboManager.h"
#include <Actor.h>
#include <system/combo/ComboView.h>



void ComboManager::ApplyToView()
{
    auto pActor = GetOwner()->Resolve(viewActor_);
    if (!pActor) return;

    auto pScript = pActor->GetScript<ComboView>();
    if (!pScript) return;

    pScript->Apply(combo_);
}
