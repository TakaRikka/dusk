#ifndef DOLPHIN_DVD_EMU_H
#define DOLPHIN_DVD_EMU_H

#include "dolphin/types.h"
#include <string>

// PC-Emulation der DVD-Funktionen
namespace DvdEmu {

// Basis-Pfad zum Datenordner (relativ zur .exe)
void setBasePath(const char* path);
const char* getBasePath();

// Konvertiert GameCube-Pfad zu PC-Pfad
// z.B. "/res/Object/LogoUs.arc" -> "C:/Games/Dusk/data/res/Object/LogoUs.arc"
std::string convertPath(const char* gcPath);

// Prüft ob Datei existiert
bool fileExists(const char* gcPath);

// Lädt Datei komplett in Speicher
// Gibt Pointer zurück, Größe wird in outSize geschrieben
// Caller muss Speicher mit free() freigeben
void* loadFile(const char* gcPath, u32* outSize, void* heap = nullptr);

// Lädt Datei in vorhandenen Buffer
// Gibt gelesene Bytes zurück
u32 loadFileToBuffer(const char* gcPath, void* buffer, u32 bufferSize, u32 offset = 0);

// Datei-Größe abfragen
u32 getFileSize(const char* gcPath);

} // namespace DvdEmu

// Ersatz für DVDConvertPathToEntrynum
// Gibt einen "Fake" Entry-Number zurück (Hash des Pfads) oder -1 wenn nicht gefunden
s32 DVDConvertPathToEntrynum_Emu(const char* path);

// Speichert Pfad für Entry-Number (für späteres Laden)
void DVDRegisterPath(s32 entryNum, const char* path);

// Holt Pfad für Entry-Number
const char* DVDGetPathForEntry(s32 entryNum);

#endif // DOLPHIN_DVD_EMU_H
