#ifndef TRAINER_H
#define TRAINER_H

#include "MemoryManager.h"
#include <map>

struct Vector3 {
    float x, y, z;
};

struct CheatState {
    std::wstring name;
    bool enabled;
    uintptr_t address;
    std::vector<unsigned char> originalBytes;
    std::vector<unsigned char> patchBytes;
};

class Trainer {
private:
    MemoryManager mem;
    std::wstring pName;
    uintptr_t baseAddress;
    std::map<std::string, CheatState> cheats;
    Vector3 savedLocation;
    
    void RegisterCheats();
    bool ApplyPatch(const std::string& cheatKey);
    bool RestorePatch(const std::string& cheatKey);

public:
    Trainer();
    ~Trainer();
    
    bool Initialize(const std::wstring& processName);
    void Update();
    void ToggleCheat(const std::string& cheatKey);
    void SetMultiplier(const std::string& cheatKey, float value);
    void SetValue(const std::string& cheatKey, int value);
    void SavePosition();
    void LoadPosition();
    void TeleportToMarker();
    void ModifyItemAmount(int amount);
    bool IsCheatEnabled(const std::string& cheatKey);
};

#endif