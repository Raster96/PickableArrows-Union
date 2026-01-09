// Supported with union (c) 2018-2021 Union team

namespace GOTHIC_ENGINE {

    class PickableArrowsCore {
    public:
        // Configuration
        static bool enabled;
        static float groundRecoveryChance;
        static float npcRecoveryChance;
        static bool allowNpcInventoryPickup;
        static bool allowGroundPickup;
        static bool chanceDependsOnSkills;
        static bool requireDamageForNpcRecovery;
        
        static void Init();
        static void ReadOptions();
        static void UpdateMenu();
        
        static bool IsArrowOrBolt(oCItem* item);
        static int GetBestRangedSkill();
        static void UpdateMenuItem(const char* itemName, const char* value);
        static void SetMenuItemEnabled(const char* itemName, bool enabled);
        static void ResetChanceSliders();
    };
}
