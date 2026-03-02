#include <dolphin/dolphin.h>
#include <dolphin/dvd.h>

#include "__dvd.h"

static void (*FatalFunc)();

const char* Japanese =
    "\n\n\nエラーが発生しました。\n\n"
    "本体のパワーボタンを押して電源をOFFにし、\n"
    "本体の取扱説明書の指示に従ってください。";

const char* English = 
    "\n\n\nAn error has occurred.\n"
    "Turn the power off and refer to the\n"
    "Nintendo GameCube Instruction Booklet\n"
    "for further instructions.";

// TODO: need solution to compile special characters in a cleaner way
const char* const Europe[6] = {
    {
        "\n\n\nAn error has occurred.\n"
        "Turn the power off and refer to the\n"
        "Nintendo GameCube Instruction Booklet\n"
        "for further instructions."
    },
    {
        "\n\n\nEin Fehler ist aufgetreten.\n"
        "Bitte schalten Sie den Nintendo GameCube\n"
        "aus und lesen Sie die Bedienungsanleitung,\n"
        "um weitere Informationen zu erhalten."
    },
    {
        "\n\n\nUne erreur est survenue.\n"
        "Eteignez la console et r\xE9" "f" "\xE9rez-vous au\n"
        "manuel d'instructions Nintendo GameCube\n"
        "pour de plus amples informations."
    },
    {
        "\n\n\nSe ha producido un error.\n"
        "Apaga la consola y consulta el manual\n"
        "de instrucciones de Nintendo GameCube\n"
        "para obtener m\xE1" "s informaci\xF3" "n."
    },
    {
        "\n\n\nSi \xE8" " verificato un errore.\n"
        "Spegni (OFF) e controlla il manuale\n"
        "d'istruzioni del Nintendo GameCube\n"
        "per ulteriori indicazioni."
    },
    {
        "\n\n\nEr is een fout opgetreden.\n"
        "Zet de Nintendo GameCube uit en\n"
        "raadpleeg de handleiding van de\n"
        "Nintendo GameCube voor nadere\n"
        "instructies."
    },
};

static void ShowMessage(void) {
    const char* message;
    GXColor bg = {0x00, 0x00, 0x00, 0x00};
    GXColor fg = {0xFF, 0xFF, 0xFF, 0x00};

    if (VIGetTvFormat() == VI_NTSC) {
        if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
            message = Japanese;
        } else {
            message = English;
        }
    } else {
        message = Europe[OSGetLanguage()];
    }

    OSFatal(fg, bg, message);
}

int DVDSetAutoFatalMessaging(BOOL enable) {
    BOOL enabled;
    int prev;

    enabled = OSDisableInterrupts();

    prev = FatalFunc ? 1 : 0;
    FatalFunc = enable ? ShowMessage : NULL;

    OSRestoreInterrupts(enabled);
    return prev;
}

void __DVDPrintFatalMessage(void) {
    if (FatalFunc) {
        FatalFunc();
    }
}
