#ifndef NW4HBM_SND_MML_SEQ_TRACK_H
#define NW4HBM_SND_MML_SEQ_TRACK_H

#include "MmlParser.h"
#include "SeqTrack.h"

namespace nw4hbm {
    namespace snd {
        namespace detail {
            class MmlSeqTrack : public SeqTrack {
            public:
                typedef struct MmlParserParam {
                    /* 0x00 */ bool cmpFlag;
                    /* 0x01 */ bool noteWaitFlag;
                    /* 0x02 */ bool tieFlag;
                    /* 0x03 */ u8 loopCount[MmlParser::CALL_STACK_MAX_DEPTH];
                    /* 0x06 */ u8 callStackDepth;
                    /* 0x08 */ const u8* callStack[MmlParser::CALL_STACK_MAX_DEPTH];
                } MmlParserParam;

            public:
                MmlSeqTrack();

                /* 0x0C */ virtual ParseResult Parse(bool doNoteOn);

                void SetMmlParser(const MmlParser* pParser) { mParser = pParser; }
                MmlParserParam& GetMmlParserParam() { return mMmlParserParam; }

            private:
                /* 0xC0 */ const MmlParser* mParser;
                /* 0xC4 */ MmlParserParam mMmlParserParam;
            };

        }  // namespace detail
    }  // namespace snd
}  // namespace nw4hbm

#endif
