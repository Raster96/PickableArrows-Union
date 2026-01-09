// PickableArrows Plugin for Union SDK
// Allows collecting and reusing shot projectiles (arrows and bolts)
// Union SOURCE file

#include <map>

namespace GOTHIC_ENGINE {

    struct ArrowConvertData {
        int itemInstance;
        bool createItem; 
    };
    
    struct NpcHitData {
        int itemInstance;
        oCNpc* hitNpc;
        int hpBefore;
    };
    
    static std::map<oCAIArrow*, ArrowConvertData> arrowsToConvert;
    
    static std::map<oCAIArrow*, NpcHitData> npcHitsToProcess;
        
    void AddArrowToNpcInventory(int itemInstance, oCNpc* npc) {
        if (!npc || itemInstance < 0) return;
        npc->CreateItems(itemInstance, 1);
    }
    
    bool RollGroundRecoveryChance() {
        if (!PickableArrowsCore::enabled) return false;
        
        float chance = PickableArrowsCore::groundRecoveryChance;
        
        if (PickableArrowsCore::chanceDependsOnSkills) {
            int skill = PickableArrowsCore::GetBestRangedSkill();
            chance = (float)skill / 100.0f;
        }
        
        float roll = (float)rand() / (float)RAND_MAX;
        return (roll <= chance);
    }
    
    bool RollNpcRecoveryChance() {
        if (!PickableArrowsCore::enabled) return false;
        
        float chance = PickableArrowsCore::npcRecoveryChance;
        
        if (PickableArrowsCore::chanceDependsOnSkills) {
            int skill = PickableArrowsCore::GetBestRangedSkill();
            chance = (float)skill / 100.0f;
        }
        
        float roll = (float)rand() / (float)RAND_MAX;
        return (roll <= chance);
    }
    
    void CreateDroppedArrow(int itemInstance, const zVEC3& position) {
        if (!zfactory || !ogame || !ogame->GetGameWorld()) return;
        
        oCItem* droppedArrow = zfactory->CreateItem(itemInstance);
        if (droppedArrow) {
            droppedArrow->SetPositionWorld(position);
            ogame->GetGameWorld()->AddVob(droppedArrow);
        }
    }
    
    static void (__thiscall *oCAIArrow_ReportCollisionToAI_Org)(oCAIArrow*, zCCollisionReport const&) = nullptr;
    
    void __fastcall oCAIArrow_ReportCollisionToAI_Hook(oCAIArrow* _this, void*, zCCollisionReport const& report) {
        if (PickableArrowsCore::enabled && _this && _this->arrow && _this->vob) {
            oCItem* arrow = _this->arrow;
            
            if (PickableArrowsCore::IsArrowOrBolt(arrow)) {
                zCCollisionObject* collObj = report.m_poCollObject1;
                oCNpc* hitNpc = nullptr;
                
                if (collObj && collObj->m_pvClientData) {
                    zCVob* hitVob = (zCVob*)collObj->m_pvClientData;
                    if (hitVob) {
                        hitNpc = dynamic_cast<oCNpc*>(hitVob);
                    }
                }
                
                if (hitNpc) {
                    if (PickableArrowsCore::allowNpcInventoryPickup && RollNpcRecoveryChance()) {
                        int itemInstance = arrow->GetInstance();
                        
                        if (PickableArrowsCore::requireDamageForNpcRecovery) {
                            NpcHitData hitData;
                            hitData.itemInstance = itemInstance;
                            hitData.hitNpc = hitNpc;
                            hitData.hpBefore = hitNpc->attribute[NPC_ATR_HITPOINTS];
                            npcHitsToProcess[_this] = hitData;
                        } else {
                            AddArrowToNpcInventory(itemInstance, hitNpc);
                        }
                    }
                } 
                else if (PickableArrowsCore::allowGroundPickup) {
                    ArrowConvertData data;
                    data.itemInstance = arrow->GetInstance();
                    data.createItem = RollGroundRecoveryChance();
                    arrowsToConvert[_this] = data;
                }
            }
        }
        
        if (oCAIArrow_ReportCollisionToAI_Org) {
            oCAIArrow_ReportCollisionToAI_Org(_this, report);
        }
    }
        
    static void (__thiscall *oCAIArrow_DoAI_Org)(oCAIArrow*, zCVob*, int&) = nullptr;
    
    void __fastcall oCAIArrow_DoAI_Hook(oCAIArrow* _this, void*, zCVob* vob, int& deleteVob) {
        auto npcIt = npcHitsToProcess.find(_this);
        if (npcIt != npcHitsToProcess.end()) {
            NpcHitData& hitData = npcIt->second;
            
            if (hitData.hitNpc) {
                int hpAfter = hitData.hitNpc->attribute[NPC_ATR_HITPOINTS];
                if (hpAfter < hitData.hpBefore) {
                    AddArrowToNpcInventory(hitData.itemInstance, hitData.hitNpc);
                }
            }
            
            npcHitsToProcess.erase(npcIt);
        }
        
        auto it = arrowsToConvert.find(_this);
        if (it != arrowsToConvert.end() && _this->vob) {
            if (!_this->trailActive) {
                zMAT4 savedTrafo = _this->vob->trafoObjToWorld;
                ArrowConvertData& data = it->second;
                
                if (ogame && ogame->GetGameWorld()) {
                    ogame->GetGameWorld()->RemoveVob(_this->vob);
                    _this->vob = nullptr;
                }
                
                _this->removeVob = 1;
                deleteVob = 1;
                
                if (data.createItem && zfactory && ogame && ogame->GetGameWorld()) {
                    oCItem* droppedArrow = zfactory->CreateItem(data.itemInstance);
                    if (droppedArrow) {
                        droppedArrow->SetTrafoObjToWorld(savedTrafo);
                        ogame->GetGameWorld()->AddVob(droppedArrow);
                    }
                }
                
                arrowsToConvert.erase(it);
                return;
            }
        }
        
        if (oCAIArrow_DoAI_Org) {
            oCAIArrow_DoAI_Org(_this, vob, deleteVob);
        }
        
        if (deleteVob) {
            arrowsToConvert.erase(_this);
            npcHitsToProcess.erase(_this);
        }
    }

    void Game_Entry() {
    }
    
    void Game_Init() {
        PickableArrowsCore::Init();
        
        DWORD* vtable = nullptr;
        
        #if ENGINE == Engine_G1
        vtable = (DWORD*)0x007DC9A4;  // G1 Classic
        #elif ENGINE == Engine_G1A
        vtable = (DWORD*)0x0081F8D0;  // G1 Addon
        #elif ENGINE == Engine_G2
        vtable = (DWORD*)0x0082CF08;  // G2 Classic
        #elif ENGINE == Engine_G2A
        vtable = (DWORD*)0x0083BFDC;  // G2 Addon
        #endif
        
        if (vtable) {
            oCAIArrow_DoAI_Org = (void (__thiscall*)(oCAIArrow*, zCVob*, int&))vtable[4];
            
            oCAIArrow_ReportCollisionToAI_Org = (void (__thiscall*)(oCAIArrow*, zCCollisionReport const&))vtable[6];
            
            DWORD oldProtect;
            VirtualProtect(&vtable[4], sizeof(DWORD) * 3, PAGE_EXECUTE_READWRITE, &oldProtect);
            vtable[4] = (DWORD)oCAIArrow_DoAI_Hook;
            vtable[6] = (DWORD)oCAIArrow_ReportCollisionToAI_Hook;
            VirtualProtect(&vtable[4], sizeof(DWORD) * 3, oldProtect, &oldProtect);
        }
    }

    void Game_Exit() {
    }

    void Game_PreLoop() {
    }

    void Game_Loop() {
        static int lastOptionsCheck = 0;
        lastOptionsCheck++;
        if (lastOptionsCheck >= 60) {
            PickableArrowsCore::ReadOptions();
            lastOptionsCheck = 0;
        }
    }

    void Game_PostLoop() {
    }

    void Game_MenuLoop() {
        static bool lastChanceDependsOnSkills = PickableArrowsCore::chanceDependsOnSkills;
        
        bool currentValue = false;
        if (zoptions) {
            currentValue = zoptions->ReadBool("PICKABLE_ARROWS", "ChanceDependsOnSkills", false);
        }
        
        if (lastChanceDependsOnSkills && !currentValue) {
            PickableArrowsCore::chanceDependsOnSkills = false;
            PickableArrowsCore::ResetChanceSliders();
        }
        
        lastChanceDependsOnSkills = currentValue;
        PickableArrowsCore::chanceDependsOnSkills = currentValue;
        
        PickableArrowsCore::UpdateMenu();
    }

    TSaveLoadGameInfo& SaveLoadGameInfo = UnionCore::SaveLoadGameInfo;

    void Game_SaveBegin() {
    }

    void Game_SaveEnd() {
    }

    void LoadBegin() {
    }

    void LoadEnd() {
    }

    void Game_LoadBegin_NewGame() {
        LoadBegin();
    }

    void Game_LoadEnd_NewGame() {
        LoadEnd();
    }

    void Game_LoadBegin_SaveGame() {
        LoadBegin();
    }

    void Game_LoadEnd_SaveGame() {
        LoadEnd();
    }

    void Game_LoadBegin_ChangeLevel() {
        LoadBegin();
    }

    void Game_LoadEnd_ChangeLevel() {
        LoadEnd();
    }

    void Game_LoadBegin_Trigger() {
    }
    
    void Game_LoadEnd_Trigger() {
    }
    
    void Game_Pause() {
    }
    
    void Game_Unpause() {
    }
    
    void Game_DefineExternals() {
    }

    void Game_ApplyOptions() {
        static bool lastChanceDependsOnSkills = PickableArrowsCore::chanceDependsOnSkills;
        
        PickableArrowsCore::ReadOptions();
        
        if (lastChanceDependsOnSkills && !PickableArrowsCore::chanceDependsOnSkills) {
            PickableArrowsCore::ResetChanceSliders();
        }
        
        lastChanceDependsOnSkills = PickableArrowsCore::chanceDependsOnSkills;
        PickableArrowsCore::UpdateMenu();
    }

#define AppDefault True
    CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
        Enabled( AppDefault ) Game_Entry,
        Enabled( AppDefault ) Game_Init,
        Enabled( AppDefault ) Game_Exit,
        Enabled( AppDefault ) Game_PreLoop,
        Enabled( AppDefault ) Game_Loop,
        Enabled( AppDefault ) Game_PostLoop,
        Enabled( AppDefault ) Game_MenuLoop,
        Enabled( AppDefault ) Game_SaveBegin,
        Enabled( AppDefault ) Game_SaveEnd,
        Enabled( AppDefault ) Game_LoadBegin_NewGame,
        Enabled( AppDefault ) Game_LoadEnd_NewGame,
        Enabled( AppDefault ) Game_LoadBegin_SaveGame,
        Enabled( AppDefault ) Game_LoadEnd_SaveGame,
        Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
        Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
        Enabled( AppDefault ) Game_LoadBegin_Trigger,
        Enabled( AppDefault ) Game_LoadEnd_Trigger,
        Enabled( AppDefault ) Game_Pause,
        Enabled( AppDefault ) Game_Unpause,
        Enabled( AppDefault ) Game_DefineExternals,
        Enabled( AppDefault ) Game_ApplyOptions
    );
}
