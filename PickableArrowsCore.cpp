// Supported with union (c) 2018-2021 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    
    // BonusIndependentTraining constants - aiscriptvars indices for real (trained) talent values
    // Using only when BonusIndependentTraining is installed
    const int REAL_TALENT_BOW_INDEX = 95;
    const int REAL_TALENT_CROSSBOW_INDEX = 96;
    
    bool PickableArrowsCore::enabled = true;
    float PickableArrowsCore::groundRecoveryChance = 1.0f;  // 100% domyślnie
    float PickableArrowsCore::npcRecoveryChance = 1.0f;     // 100% domyślnie
    bool PickableArrowsCore::allowNpcInventoryPickup = true;
    bool PickableArrowsCore::allowGroundPickup = true;
    bool PickableArrowsCore::chanceDependsOnSkills = false;
    bool PickableArrowsCore::requireDamageForNpcRecovery = true;

    void PickableArrowsCore::Init() {
        ReadOptions();
        srand((unsigned int)time(NULL));
    }

    int PickableArrowsCore::GetBestRangedSkill() {
        if (!player) return 100;
        
        oCNpc* hero = player;
        if (!hero) return 100;
        
        #if ENGINE == Engine_G1 || ENGINE == Engine_G1A
        int bowPercent = hero->GetTalentValue(oCNpcTalent::NPC_TAL_BOW);
        int crossbowPercent = hero->GetTalentValue(oCNpcTalent::NPC_TAL_CROSSBOW);
        return max(bowPercent, crossbowPercent);
        #else
        int trainedBow = hero->aiscriptvars[REAL_TALENT_BOW_INDEX];
        int trainedXbow = hero->aiscriptvars[REAL_TALENT_CROSSBOW_INDEX];
        
        if (trainedBow > 0 || trainedXbow > 0) {
            return max(trainedBow, trainedXbow);
        }
        
        int effectiveBow = hero->hitChance[NPC_HITCHANCE_BOW];
        int effectiveXbow = hero->hitChance[NPC_HITCHANCE_CROSSBOW];
        return max(effectiveBow, effectiveXbow);
        #endif
    }

    void PickableArrowsCore::ReadOptions() {
        if (!zoptions) return;
        
        enabled = zoptions->ReadBool("PICKABLE_ARROWS", "Enabled", true);
        chanceDependsOnSkills = zoptions->ReadBool("PICKABLE_ARROWS", "ChanceDependsOnSkills", false);
        
        int groundIndex = zoptions->ReadInt("PICKABLE_ARROWS", "GroundRecoveryChance", 100);
        int npcIndex = zoptions->ReadInt("PICKABLE_ARROWS", "NpcRecoveryChance", 100);
        
        groundRecoveryChance = (float)groundIndex / 100.0f;
        npcRecoveryChance = (float)npcIndex / 100.0f;
        
        allowNpcInventoryPickup = zoptions->ReadBool("PICKABLE_ARROWS", "AllowNpcInventoryPickup", true);
        allowGroundPickup = zoptions->ReadBool("PICKABLE_ARROWS", "AllowGroundPickup", true);
        requireDamageForNpcRecovery = zoptions->ReadBool("PICKABLE_ARROWS", "RequireDamageForNpcRecovery", true);
        
        if (groundRecoveryChance < 0.0f) groundRecoveryChance = 0.0f;
        if (groundRecoveryChance > 1.0f) groundRecoveryChance = 1.0f;
        if (npcRecoveryChance < 0.0f) npcRecoveryChance = 0.0f;
        if (npcRecoveryChance > 1.0f) npcRecoveryChance = 1.0f;
    }

    bool PickableArrowsCore::IsArrowOrBolt(oCItem* item) {
        if (!item) return false;
                
        if (item->mainflag == ITM_CAT_MUN) {
            if ((item->flags & ITM_FLAG_BOW) || (item->flags & ITM_FLAG_CROSSBOW)) {
                return true;
            }
        }
        
        return false;
    }

    void PickableArrowsCore::UpdateMenuItem(const char* itemName, const char* value) {
        zSTRING itemNameStr = itemName;
        zCMenuItem* menuItem = zCMenuItem::GetByName(itemNameStr);
        
        if (menuItem) {
            zSTRING valueStr = value;
            menuItem->SetText(valueStr, 0, TRUE);
        }
    }

    void PickableArrowsCore::SetMenuItemEnabled(const char* itemName, bool enabled) {
        zSTRING itemNameStr = itemName;
        zCMenuItem* menuItem = zCMenuItem::GetByName(itemNameStr);
        
        if (menuItem) {
            if (enabled) {
                menuItem->flags |= IT_SELECTABLE;
            } else {
                menuItem->flags &= ~IT_SELECTABLE;
            }
        }
    }

    void PickableArrowsCore::UpdateMenu() {
        zCMenu* activeMenu = zCMenu::GetActive();
        if (!activeMenu) return;
        
        zSTRING menuName = activeMenu->name;
        if (menuName != "PickableArrows:Menu_Opt_PickableArrows") return;
        
        if (chanceDependsOnSkills && player) {
            int skill = GetBestRangedSkill();
            
            if (skill > 100) skill = 100;
            if (skill < 0) skill = 0;
            
            int menuIndex = skill;
            
            zCMenuItem* groundSlider = zCMenuItem::GetByName("PickableArrows:MenuItem_Opt_PickableArrows_GroundRecoveryChance_Slider");
            zCMenuItem* npcSlider = zCMenuItem::GetByName("PickableArrows:MenuItem_Opt_PickableArrows_NpcRecoveryChance_Slider");
            
            if (groundSlider && groundSlider->m_parType == MENU_ITEM_CHOICEBOX) {
                zCMenuItemChoice* choice = (zCMenuItemChoice*)groundSlider;
                choice->option = menuIndex;
            }
            
            if (npcSlider && npcSlider->m_parType == MENU_ITEM_CHOICEBOX) {
                zCMenuItemChoice* choice = (zCMenuItemChoice*)npcSlider;
                choice->option = menuIndex;
            }
            
            if (zoptions) {
                zSTRING indexStr = zSTRING(menuIndex);
                zoptions->WriteString("PICKABLE_ARROWS", "GroundRecoveryChance", indexStr, FALSE);
                zoptions->WriteString("PICKABLE_ARROWS", "NpcRecoveryChance", indexStr, FALSE);
            }
        }
    }
    
    void PickableArrowsCore::ResetChanceSliders() {
        groundRecoveryChance = 1.0f;
        npcRecoveryChance = 1.0f;
        
        zCMenuItem* groundSlider = zCMenuItem::GetByName("PickableArrows:MenuItem_Opt_PickableArrows_GroundRecoveryChance_Slider");
        zCMenuItem* npcSlider = zCMenuItem::GetByName("PickableArrows:MenuItem_Opt_PickableArrows_NpcRecoveryChance_Slider");
        
        if (groundSlider && groundSlider->m_parType == MENU_ITEM_CHOICEBOX) {
            zCMenuItemChoice* choice = (zCMenuItemChoice*)groundSlider;
            choice->option = 100;
        }
        
        if (npcSlider && npcSlider->m_parType == MENU_ITEM_CHOICEBOX) {
            zCMenuItemChoice* choice = (zCMenuItemChoice*)npcSlider;
            choice->option = 100;
        }
        
        if (zoptions) {
            zoptions->WriteString("PICKABLE_ARROWS", "GroundRecoveryChance", "100", TRUE);
            zoptions->WriteString("PICKABLE_ARROWS", "NpcRecoveryChance", "100", TRUE);
        }
    }
}
