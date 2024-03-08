#pragma once

#include "Component.hpp"
#include <string>

class HUDComponent : public Component {
public:
    explicit HUDComponent(const uint32_t id);
    ~HUDComponent() override;

    std::string     background      {"./media/hud/background.png"};
    std::string     healthDetails   {"./media/hud/healthDetails.png"};
    float           maxHeartTimer   {0.5};
    float           heartTimer      {0};
    bool            heart           {false};

    std::string     health          {"./media/hud/health.png"};
    std::string     poison          {"./media/hud/poison.png"};
    std::string     healthHighlight {"./media/hud/healthHighlight.png"};
    std::string     healingHighlight{"./media/hud/healingHighlight.png"};
    std::string     primaryWeapon   {"./media/hud/primaryWeapon.png"};
    std::string     primaryCancel   {"./media/hud/primaryCancel.png"};
    std::string     secondaryWeapon {"./media/hud/secondaryWeapon.png"};
    std::string     secondaryCancel {"./media/hud/secondaryCancel.png"};
    std::string     levelingUp      {"./media/hud/levelingUp.png"};
    std::string     stats           {"./media/hud/stats.png"};
    std::string     dashFrame       {"./media/hud/dashFrame.png"};
    std::string     dashCancel      {"./media/hud/dashCancel.png"};
    std::string     dashComplete    {"./media/hud/dashComplete.png"};
    std::string     dashRecharging  {"./media/hud/dashRecharging.png"};
    std::string     stun            {"./media/hud/stun.png"};
    std::string     keys            {"./media/hud/keys.png"};

    std::string     map             {"./media/map/map.png"};
    std::string     mapBorders      {"./media/map/borders.png"};

    float           transparencySign {1.0f};
};