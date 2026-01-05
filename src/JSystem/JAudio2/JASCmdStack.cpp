//
// JASCmdStack
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASCmdStack.h"
#include "JSystem/JAudio2/JASCriticalSection.h"
#include "dolphin/os.h"

JASPortCmd::TPortHead JASPortCmd::sCommandListOnce;

JASPortCmd::TPortHead JASPortCmd::sCommandListStay;

bool JASPortCmd::addPortCmdOnce() {
	JASCriticalSection cs;
	return sCommandListOnce.append(this);
}

bool JASPortCmd::setPortCmd(Command func, JASPortArgs* args) {
	JUT_ASSERT(81, func != NULL);
	if (getSupervisor()) {
		return false;
	}
	mFunc = func;
	mArgs = args;
	return true;
}

void JASPortCmd::execAllCommand() {
	sCommandListOnce.execCommandOnce();
	sCommandListStay.execCommandStay();
}

void JASPortCmd::TPortHead::execCommandOnce() {
	JASCriticalSection cs;
	JSULink<JASPortCmd>* next;
	for (JSULink<JASPortCmd>* link = getFirst(); link != NULL; link = next) {
		next = link->getNext();
		JASPortCmd* cmd = link->getObject();
		cmd->getFunc()(cmd->getArgs());
		remove(link);
	}
}

void JASPortCmd::TPortHead::execCommandStay() {
	JASCriticalSection cs;
	for (JSULink<JASPortCmd>* link = getFirst(); link != NULL; link = link->getNext()) {
		JASPortCmd* cmd = link->getObject();
		cmd->getFunc()(cmd->getArgs());
	}
}
