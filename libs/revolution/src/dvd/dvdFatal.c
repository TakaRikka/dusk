#include <revolution.h>
#include <revolution/dvd.h>
#include <revolution/sc.h>

#include "__dvd.h"

static void (*FatalFunc)();

const char* const __DVDErrorMessageDefault[] = {
    "\n"
    "\n"
    "\n"
    "エラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n"
    "\n"
    "\n"
    "An error has occurred.\n"
    "Press the Eject Button, remove the\n"
    "Game Disc, and turn off the power to the\n"
    "console. Please read the Wii Operations\n"
    "Manual for further instructions.",

    "\n"
    "\n"
    "\n"
    "Ein Fehler ist aufgetreten.\n"
    "Dr""\xFC""cke den Ausgabeknopf, entnimm die\n"
    "Disc und schalte die Wii-Konsole aus.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Une erreur est survenue.\n"
    "Appuyez sur le bouton EJECT, retirez\n"
    "le disque et ""\xE9""teignez la console.\n"
    "Veuillez vous r""\xE9""f""\xE9""rer au Mode d'emploi\n"
    "de la Wii pour plus de d""\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "Ocurri\xF3 un Error.\n"
    "Oprime el Bot\xF3n EJECT, saca el disco\n"
    "y apaga la consola. Consulta el manual\n"
    "de operaciones de la consola Wii para\n"
    "obtener m\xE1s informaci\xF3n.",

    "\n"
    "\n"
    "\n"
    "Si \xE8 verificato un errore.\n"
    "Premi il pulsante EJECT, estrai il disco\n"
    "e spegni la console. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is een fout opgetreden.\n"
    "Druk op de EJECT-knop, verwijder de\n"
    "disk en zet het Wii-systeem uit. Lees\n"
    "de Wii-handleiding voor meer informatie."
};

const char* const __DVDErrorMessageEurope[] = {
    "\n"
    "\n"
    "\n"
    "エラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n"
    "\n"
    "\n"
    "An error has occurred.\n"
    "Press the EJECT Button, remove the Disc,\n"
    "and turn off the power to the console.\n"
    "Please refer to the Wii Operations Manual\n"
    "for details.",

    "\n"
    "\n"
    "\n"
    "Ein Fehler ist aufgetreten.\n"
    "Dr""\xFC""cke den Ausgabeknopf, entnimm die\n"
    "Disc und schalte die Wii-Konsole aus.\n"
    "Bitte lies die Wii-Bedienungsanleitung,\n"
    "um weitere Informationen zu erhalten.",

    "\n"
    "\n"
    "\n"
    "Une erreur est survenue.\n"
    "Appuyez sur le bouton EJECT, retirez\n"
    "le disque et ""\xE9""teignez la console.\n"
    "Veuillez vous r""\xE9""f""\xE9""rer au mode d'emploi\n"
    "Wii pour plus de d""\xE9""tails.",

    "\n"
    "\n"
    "\n"
    "Se ha producido un error.\n"
    "Pulsa el Bot\xF3n EJECT, extrae el disco y\n"
    "apaga la consola. Consulta el manual de\n"
    "instrucciones de la consola Wii para\n"
    "obtener m\xE1s informaci\xF3n.",

    "\n"
    "\n"
    "\n"
    "Si \xE8 verificato un errore.\n"
    "Premi il pulsante EJECT, estrai il disco\n"
    "e spegni la console. Per maggiori\n"
    "informazioni, consulta il manuale di\n"
    "istruzioni della console Wii.",

    "\n"
    "\n"
    "\n"
    "Er is een fout opgetreden.\n"
    "Druk op de EJECT-knop, verwijder de\n"
    "disk en zet het Wii-systeem uit. Lees\n"
    "de Wii-handleiding voor meer informatie."
};

const char* __DVDErrorMessageChinaKorea [] = {
    "\n"
    "\n"
    "エラーコード１０４。\n"
    "エラーが発生しました。\n"
    "\n"
    "イジェクトボタンを押してディスクを取り出してか\n"
    "ら、本体の電源をOFFにして、本体の取扱説明書の\n"
    "指示に従ってください。",

    "\n"
    "\n"
    "Error #104,\n"
    "An error has occurred.\n"
    "Press the EJECT Button, remove the\n"
    "Game Disc, and turn the power off.\n"
    "Please read the Wii Operations Manual\n"
    "for more information."
};

void __DVDShowFatalMessage(void) {
    const char* message;
    const char* const* messageList;
    GXColor bg = { 0, 0, 0, 0 };
    GXColor fg = { 255, 255, 255, 0 };

    if (SCGetLanguage() == SC_LANG_JAPANESE) {
        OSSetFontEncode(OS_FONT_ENCODE_SJIS);
    } else {
        OSSetFontEncode(OS_FONT_ENCODE_ANSI);
    }

    switch (SCGetProductGameRegion()) {
    case 0:
    case 1:
    case 3:
    default:
        messageList = __DVDErrorMessageDefault;
        break;
    case 2:
        messageList = __DVDErrorMessageEurope;
        break;
    case 4:
    case 5:
        messageList = __DVDErrorMessageChinaKorea;
        break;
    }

    if (SCGetLanguage() > SC_LANG_DUTCH) {
        message = messageList[1];
    } else {
        message = messageList[SCGetLanguage()];
    }

    OSFatal(fg, bg, message);
}

BOOL DVDSetAutoFatalMessaging(BOOL enable) {
    BOOL enabled, prev;
    enabled = OSDisableInterrupts();
    prev = FatalFunc ? TRUE : FALSE;
    FatalFunc = enable ? __DVDShowFatalMessage : NULL;
    OSRestoreInterrupts(enabled);
    return prev;
}

BOOL __DVDGetAutoFatalMessaging(void) {
    return FatalFunc ? TRUE : FALSE;
}

void __DVDPrintFatalMessage(void) {
    if (FatalFunc) {
        FatalFunc();
    }
}
