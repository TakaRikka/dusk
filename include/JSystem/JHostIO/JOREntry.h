#ifndef JORENTRY_H
#define JORENTRY_H

#include "JSystem/JHostIO/JHIComm.h"
#include "dolphin/os.h"

template<typename T, int I>
class JHIpvector {
public:
    JHIpvector() { m_size = 0; }

    s32 size() const { return m_size; }
    T get(u32 i) const { return m_vector[i]; }

    s32 push_back(T p) {
        if (m_size >= I) {
            return 0;
        }

        m_vector[m_size++] = p;
        return 1;
    }

    /* 0x00 */ T m_vector[I];
    /* 0x28 */ s32 m_size;
};

template<typename T>
class JHIComPortManager {
public:
    JHIComPortManager() {
        field_0x10040 = 0;
        field_0x10041 = 0;
    }

    T& getRefPort() { return port; }

    void addTag(JHITag<T>* pTag) {
        pTag->mp_data = this;
        field_0xc.push_back(pTag);
    }

    JHITag<T>* find(u32 param_1) {
        for (u32 i = 0; i < field_0xc.size(); i++) {
            JHITag<T>* tag = field_0xc.get(i);
            if (tag->m_tag == param_1) {
                return tag;
            }
        }
        return NULL;
    }

    void dispatchMessage() {
        u32 r29 = port.available();
        while (true) {
            if (!field_0x10040) {
                if (r29 < 8) {
                    return;
                }
                port.receive(&field_0x10038, 8);
                r29 -= 8;
                field_0x10038 = JHIntohl(field_0x10038);
                field_0x1003c = JHIntohl(field_0x1003c);
                field_0x10040 = 1;
                JHITag<T>* r27 = find(field_0x10038);
                if (field_0x1003c > port.getMaxReaderableSize()) {
                    port.initBuffers();
                    OSReport("JHostIO::ERROR: JHIComportManager: invalid datasize ( port.initialized )\n");
                } else if (!r27) {
                    OSReport("JHostIO::ERROR: JHIComportManager: invalid tag message\n");
                }
            }
            if (!field_0x10040) {
                continue;
            }

            if (r29 < field_0x1003c) {
                return;
            }
            u32 r26 = port.receive(field_0x38, field_0x1003c);
            r29 -= r26;
            JHITag<T>* r28 = find(field_0x10038);
            field_0x10040 = 0;
            if (r28) {
                r28->receive(field_0x38, field_0x1003c);
            }
        }
    }

    static JHIComPortManager<T>* create() {
        if (instance == NULL) {
            instance = new JHIComPortManager<T>();
        }

        return instance;
    }

    static JHIComPortManager<T>* getInstance() { return instance; }

    static JHIComPortManager<T>* instance;

    /* 0x00000 */ T port;
    /* 0x0000C */ JHIpvector<JHITag<T>*, 10> field_0xc;
    /* 0x00038 */ char field_0x38[0x10000];
    /* 0x10038 */ u32 field_0x10038;
    /* 0x1003C */ u32 field_0x1003c;
    /* 0x10040 */ u8 field_0x10040;
    /* 0x10041 */ u8 field_0x10041;
};

void JORInit();

#endif /* JORENTRY_H */
