#include <revolution/os.h>
#include <revolution/nand.h>
#include <revolution/fs.h>
#include <revolution/sc.h>

#define REGION_DEFAULT 0
#define REGION_EUROPE  1
#define REGION_CHNKOR  2

const char* const __NANDMaxBlocksErrorMessageDefault[] = {
    "\n\n\nWii本体保存メモリの空き容量が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThere is no more available space in\nWii system memory. Refer to the Wii\nOperations Manual for details.",
    "\n\n\nDer Speicher der Wii-Konsole ist belegt.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nIl n'y a pas assez d'espace libre\ndans la m駑oire de la console Wii.\nVeuillez vous r馭駻er au Mode d'emploi\nde la Wii pour plus de d騁ails.",
    "\n\n\nNo queda espacio libre en la memoria\nde la consola Wii. Consulta el manual\nde operaciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nNon c'\xE8 pi\xF9 spazio libero nella memoria\ndella console Wii. Per maggiori\ninformazioni, consulta il manuale di\nistruzioni della console Wii.",
    "\n\n\nEr is geen vrije ruimte meer in het\ninterne geheugen van het Wii-systeem.\nLees de handleiding voor meer informatie.",
};

const char* const __NANDMaxBlocksErrorMessageEurope[] = {
    "\n\n\nWii本体保存メモリの空き容量が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThere is no more available space in\nthe Wii System Memory. Please refer to\nthe Wii Operations Manual for details.",
    "\n\n\nDer Speicher der Wii-Konsole ist belegt.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nIl n'y a pas assez d'espace libre dans\nla m駑oire de la console Wii. Veuillez\nvous r馭駻er au mode d'emploi Wii pour\nplus de d騁ails.",
    "\n\n\nNo queda espacio libre en la memoria de\nla consola Wii. Consulta el manual de\ninstrucciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nNon c'\xE8 pi\xF9 spazio libero nella memoria\ndella console Wii. Per maggiori\ninformazioni, consulta il manuale di\nistruzioni della console Wii.",
    "\n\n\nEr is geen vrije ruimte meer in het\ninterne geheugen van het Wii-systeem.\nLees de handleiding voor meer informatie.",
};

const char* const __NANDMaxBlocksErrorMessageChinaKorea[] = {
    "\n\nエラーコード４０５。\n\nWii本体保存メモリの空き容量が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\nError #405,\n\nThere is no more available space in\nWii system memory. Refer to the Wii\nOperations Manual for details.",
};

const char* const __NANDMaxFilesErrorMessageDefault[] = {
    "\n\n\nWii本体保存メモリの空きファイル数が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThere is no more available space in\nWii system memory. Refer to the Wii\nOperations Manual for details.",
    "\n\n\nDer Speicher der Wii-Konsole ist belegt.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nIl n'y a pas assez d'espace libre dans\nla m駑oire de la console Wii. Veuillez\nvous r馭駻er au Mode d'emploi de la Wii\npour plus de d騁ails.",
    "\n\n\nNo queda espacio libre en la memoria de\nla consola Wii. Consulta el manual de\noperaciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nImpossibile salvare altri dati nella\nmemoria della console Wii. Per maggiori\ninformazioni, consulta il manuale di\nistruzioni della console Wii.",
    "\n\n\nEr is geen ruimte meer beschikbaar\nin het interne geheugen van het\nWii-systeem. Lees de handleiding voor\nmeer informatie.",
};

const char* const __NANDMaxFilesErrorMessageEurope[] = {
    "\n\n\nWii本体保存メモリの空きファイル数が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThere is no more available space in\nthe Wii System Memory. Please refer to\nthe Wii Operations Manual for details.",
    "\n\n\nDer Speicher der Wii-Konsole ist belegt.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nIl n'y a pas assez d'espace libre dans\nla m駑oire de la console Wii. Veuillez\nvous r馭駻er au mode d'emploi Wii pour\nplus de d騁ails.",
    "\n\n\nNo queda espacio libre en la memoria de\nla consola Wii. Consulta el manual de\ninstrucciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nImpossibile salvare altri dati nella\nmemoria della console Wii. Per maggiori\ninformazioni, consulta il manuale di\nistruzioni della console Wii.",
    "\n\n\nEr is geen ruimte meer beschikbaar\nin het interne geheugen van het\nWii-systeem. Lees de handleiding voor\nmeer informatie.",
};

const char* const __NANDMaxFilesErrorMessageChinaKorea[] = {
    "\n\nエラーコード４０６。\n\nWii本体保存メモリの空きファイル数が異常です。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\nError #406,\n\nThere is no more available space in\nWii system memory. Refer to the Wii\nOperations Manual for details.",
};

const char* const __NANDCorruptErrorMessageDefault[] = {
    "\n\n\nWii本体保存メモリが壊れました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThe Wii system memory has been damaged.\nRefer to the Wii Operations Manual for\ndetails.",
    "\n\n\nDer Speicher der Wii-Konsole\nist besch臈igt. Bitte lies die\nWii-Bedienungsanleitung, um weitere\nInformationen zu erhalten.",
    "\n\n\nLa m駑oire de la console Wii a 騁\xE9 \nendommag馥. Veuillez vous r馭駻er au\nMode d'emploi de la Wii pour plus de\nd騁ails.",
    "\n\n\nLa memoria de la consola Wii\nest\xE1 da\xF1""ada. Consulta el manual de\noperaciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nLa memoria della console Wii e\ndanneggiata. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nHet interne geheugen van het\nWii-systeem is beschadigd. Lees de\nWii-handleiding voor meer informatie.",
};

const char* const __NANDCorruptErrorMessageEurope[] = {
    "\n\n\nWii本体保存メモリが壊れました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nThe Wii System Memory has been damaged.\nPlease refer to the Wii Operations Manual\nfor details.",
    "\n\n\nDer Speicher der Wii-Konsole\nist beschadigt. Bitte lies die\nWii-Bedienungsanleitung, um weitere\nInformationen zu erhalten.",
    "\n\n\nLa m駑oire de la console Wii est\nendommag馥. Veuillez vous r馭駻er au\nmode d'emploi Wii pour plus de d騁ails.\n",
    "\n\n\nLa memoria de la consola Wii est\xE1 da\xF1""ada.\nConsulta el manual de instrucciones de la\nconsola Wii para obtener m疽 informaci\xF3n.",
    "\n\n\nLa memoria della console Wii e\ndanneggiata. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nHet interne geheugen van het\nWii-systeem is beschadigd. Lees de\nWii-handleiding voor meer informatie.",
};

const char* const __NANDCorruptErrorMessageChinaKorea[] = {
    "\n\nエラーコード４０８。\n\nWii本体保存メモリが壊れました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\nError #408,\n\nThe Wii system memory has been damaged.\nRefer to the Wii Operations Manual for\ndetails.",
};

const char* const __NANDBusyErrorMessageDefault[] = {
    "\n\n\nWii本体保存メモリの書き込み/読み出しが\nできませんでした。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nCould not access Wii system memory.\nRefer to the Wii Operations Manual for\ndetails.",
    "\n\n\nAuf den Speicher der Wii-Konsole konnte\nnicht zugegriffen werden. Bitte lies die\nWii-Bedienungsanleitung, um weitere\nInformationen zu erhalten.",
    "\n\n\nImpossible d'acc馘er \xE0 la m駑oire de\nla console Wii. Veuillez vous r馭駻er\nau Mode d'emploi de la Wii pour plus\nde d騁ails.",
    "\n\n\nNo se ha podido acceder a la memoria de\nla consola Wii. Consulta el manual de\noperaciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nImpossibile accedere alla memoria della\nconsole Wii. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nHet interne geheugen van het Wii-systeem\nkan niet worden gelezen of beschreven.\nLees de Wii-handleiding voor meer\ninformatie.",
};

const char* const __NANDBusyErrorMessageEurope[] = {
    "\n\n\nWii本体保存メモリの書き込み/読み出しが\nできませんでした。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nCould not access the Wii System Memory.Please refer to the Wii Operations Manual\nfor details.",
    "\n\n\nAuf den Speicher der Wii-Konsole konnte\nnicht zugegriffen werden. Bitte lies die\nWii-Bedienungsanleitung, um weitere\nInformationen zu erhalten.",
    "\n\n\nImpossible d'acc馘er \xE0 la m駑oire de la\nconsole Wii. Veuillez vous r馭駻er au\nmode d'emploi Wii pour plus de d騁ails.",
    "\n\n\nNo se ha podido acceder a la memoria de\nla consola Wii. Consulta el manual de\ninstrucciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nImpossibile accedere alla memoria della\nconsole Wii. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nHet interne geheugen van het Wii-systeem\nkan niet worden gelezen of beschreven.\nLees de Wii-handleiding voor meer\ninformatie.",
};

const char* const __NANDBusyErrorMessageChinaKorea[] = {
    "\n\nエラーコード４１１。\n\nWii本体保存メモリの書き込み/読み出しが\nできませんでした。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\nError #411,\n\nCould not access Wii system memory.\nRefer to the Wii Operations Manual for\ndetails.",
};

const char* const __NANDUnknownErrorMessageDefault[] = {
    "\n\n\nWii本体保存メモリの書き込み/読み出し中に\nエラーが発生しました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nAn error occurred while accessing Wii\nsystem memory. Refer to the Wii\nOperations Manual for details.",
    "\n\n\nBeim Zugriff auf den Speicher der\nWii-Konsole ist ein Fehler aufgetreten.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nUne erreur est survenue pendant le\nprocessus de lecture ou d'馗riture\ndans la m駑oire de la console Wii.\nVeuillez vous r馭駻er au Mode d'emploi\nde la Wii pour plus de d騁ails.",
    "\n\n\nSe ha producido un error al intentar\nacceder a la memoria de la consola Wii.\nConsulta el manual de operaciones\nde la consola Wii para obtener m疽\ninformaci\xF3n.",
    "\n\n\nSi \xE8 verificato un errore durante la\nlettura o la modifica dei dati\nall'interno della memoria della\nconsole Wii. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nEr is een fout opgetreden tijdens het\nlezen of beschrijven van het interne\ngeheugen van het Wii-systeem. Lees de\nWii-handleiding voor meer informatie.",
};

const char* const __NANDUnknownErrorMessageEurope[] = {
    "\n\n\nWii本体保存メモリの書き込み/読み出し中に\nエラーが発生しました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\n\nAn error occurred during the process of\nreading from or writing to the Wii System\nMemory. Please refer to the Wii Operations\nManual for details.",
    "\n\n\nBeim Zugriff auf den Speicher der\nWii-Konsole ist ein Fehler aufgetreten.\nBitte lies die Wii-Bedienungsanleitung,\num weitere Informationen zu erhalten.",
    "\n\n\nUne erreur est survenue avec la m駑oire\nde la console Wii pendant le processus\nde lecture ou d'馗riture. Veuillez vous\nr馭駻er au mode d'emploi Wii pour plus\nde d鑼ails.",
    "\n\n\nSe ha producido un error durante la\nlectura o escritura de la memoria de\nla consola Wii. Consulta el manual de\ninstrucciones de la consola Wii para\nobtener m疽 informaci\xF3n.",
    "\n\n\nSi \xE8 verificato un errore durante la\nlettura o la modifica dei dati\nall'interno della memoria della\nconsole Wii. Per maggiori informazioni,\nconsulta il manuale di istruzioni della\nconsole Wii.",
    "\n\n\nEr is een fout opgetreden tijdens het\nlezen of beschrijven van het interne\ngeheugen van het Wii-systeem. Lees de\nWii-handleiding voor meer informatie.",
};

const char* const __NANDUnknownErrorMessageChinaKorea[] = {
    "\n\nエラーコード４１２。\n\nWii本体保存メモリの書き込み/読み出し中に\nエラーが発生しました。\nくわしくはWii本体の取扱説明書をお読み\nください。",
    "\n\nError #412,\n\nAn error occurred while accessing Wii\nsystem memory. Refer to the Wii\nOperations Manual for details.",
};

static void (*NANDErrorFunc)(s32 errorCode);

const char* const* __NANDSetErrorMessageList(s32 errorCode, s32 region) {
    u8 i = 0;
    struct {
        int errorCode;
        int region;
        const char* const* messageList;
    } errorMessageList[] = {
        {ISFS_ERROR_MAXBLOCKS, REGION_DEFAULT, __NANDMaxBlocksErrorMessageDefault},
        {ISFS_ERROR_MAXFILES,  REGION_DEFAULT, __NANDMaxFilesErrorMessageDefault},
        {ISFS_ERROR_CORRUPT,   REGION_DEFAULT, __NANDCorruptErrorMessageDefault},
        {ISFS_ERROR_BUSY,      REGION_DEFAULT, __NANDBusyErrorMessageDefault},
        {IOS_ERROR_FAIL_ALLOC, REGION_DEFAULT, __NANDBusyErrorMessageDefault},
        {ISFS_ERROR_UNKNOWN,   REGION_DEFAULT, __NANDUnknownErrorMessageDefault},
        {IOS_ERROR_UNKNOWN,    REGION_DEFAULT, __NANDUnknownErrorMessageDefault},

        {ISFS_ERROR_MAXBLOCKS, REGION_EUROPE, __NANDMaxBlocksErrorMessageEurope},
        {ISFS_ERROR_MAXFILES,  REGION_EUROPE, __NANDMaxFilesErrorMessageEurope},
        {ISFS_ERROR_CORRUPT,   REGION_EUROPE, __NANDCorruptErrorMessageEurope},
        {ISFS_ERROR_BUSY,      REGION_EUROPE, __NANDBusyErrorMessageEurope},
        {IOS_ERROR_FAIL_ALLOC, REGION_EUROPE, __NANDBusyErrorMessageEurope},
        {ISFS_ERROR_UNKNOWN,   REGION_EUROPE, __NANDUnknownErrorMessageEurope},
        {IOS_ERROR_UNKNOWN,    REGION_EUROPE, __NANDUnknownErrorMessageEurope},

        {ISFS_ERROR_MAXBLOCKS, REGION_CHNKOR, __NANDMaxBlocksErrorMessageChinaKorea},
        {ISFS_ERROR_MAXFILES,  REGION_CHNKOR, __NANDMaxFilesErrorMessageChinaKorea},
        {ISFS_ERROR_CORRUPT,   REGION_CHNKOR, __NANDCorruptErrorMessageChinaKorea},
        {ISFS_ERROR_BUSY,      REGION_CHNKOR, __NANDBusyErrorMessageChinaKorea},
        {IOS_ERROR_FAIL_ALLOC, REGION_CHNKOR, __NANDBusyErrorMessageChinaKorea},
        {ISFS_ERROR_UNKNOWN,   REGION_CHNKOR, __NANDUnknownErrorMessageChinaKorea},
        {IOS_ERROR_UNKNOWN,    REGION_CHNKOR, __NANDUnknownErrorMessageChinaKorea},
    };

    while (i < 21) {
        if (region == errorMessageList[i].region) {
            if (errorCode == errorMessageList[i].errorCode) {
                return errorMessageList[i].messageList;
            }
            i++;
            continue;
        }

        i = i + 7;
    }

    return __NANDUnknownErrorMessageDefault;
}

void __NANDShowErrorMessage(s32 errorCode) {
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
        messageList = __NANDSetErrorMessageList(errorCode, REGION_DEFAULT);
        break;
    case 2:
        messageList = __NANDSetErrorMessageList(errorCode, REGION_EUROPE);
        break;
    case 4:
    case 5:
        messageList = __NANDSetErrorMessageList(errorCode, REGION_CHNKOR);
        break;
    }

    if (SCGetLanguage() > SC_LANG_DUTCH) {
        message = messageList[1];
    } else {
        message = messageList[SCGetLanguage()];
    }

    OSFatal(fg, bg, message);
}

BOOL NANDSetAutoErrorMessaging(BOOL show) {
    BOOL enabled = OSDisableInterrupts();
    BOOL var_r30 = NANDErrorFunc != NULL ? TRUE : FALSE;

    NANDErrorFunc = show ? __NANDShowErrorMessage : NULL;

    OSRestoreInterrupts(enabled);
    return var_r30;
}

void __NANDPrintErrorMessage(s32 errorCode) {
    if (NANDErrorFunc) {
        NANDErrorFunc(errorCode);
    }
}
