//
// JASCmdStack
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASCmdStack.h"
#include "dolphin/os.h"

JASPortCmd::TPortHead JASPortCmd::sCommandListOnce;

JASPortCmd::TPortHead JASPortCmd::sCommandListStay;

void JASPortCmd::execAllCommand() {
	sCommandListOnce.execCommandOnce();
	sCommandListStay.execCommandStay();
}

void JASPortCmd::TPortHead::execCommandOnce() {
	volatile BOOL interrupts = OSDisableInterrupts();
	JSULink<JASPortCmd>* next;
	for (JSULink<JASPortCmd>* link = getFirst(); link != NULL; link = next) {
		next = link->getNext();
		link->getObject()->_10(link->getObject()->_14);
		remove(link);
	}
	OSRestoreInterrupts(interrupts);
}

void JASPortCmd::TPortHead::execCommandStay() {
	volatile int interrupts = OSDisableInterrupts();
	for (JSULink<JASPortCmd>* link = getFirst(); link != NULL; link = link->getNext()) {
		link->getObject()->_10(link->getObject()->_14);
	}
	OSRestoreInterrupts(interrupts);
}
