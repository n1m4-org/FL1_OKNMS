#include "ComboView.h"
#include <UILayoutComponent.h>
#include <Actor.h>
#include <UILayout.h>
#include <string>

using namespace Tako;

void ComboView::Tick(float deltaTime)
{
    if (combo_ != comboDisplayed_)
    {
        UpdateText();
    }
}

void ComboView::Apply(Combo combo)
{
    combo_ = combo;
}

void ComboView::UpdateText()
{
    UILayoutComponent* ulc = GetOwner()->GetComponent<UILayoutComponent>();
    ulc->GetLayout().SetText("Combo", std::to_string(static_cast<int>(combo_)));
    comboDisplayed_ = combo_;
}
