#pragma once

#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include <m_Do/m_Do_MemCardRWmng.h>
#include <m_Do/m_Do_MemCard.h>

void noAutoSave();
void triggerAutoSave();
void updateAutoSave();
void enterAutoSave();
void autoSaving();
void waitingForWrite();
void endAutoSave();
void toggleAutoSave(bool enabled);

#endif