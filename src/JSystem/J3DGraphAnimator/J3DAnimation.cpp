#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/JMath/JMath.h"

void J3DFrameCtrl::init(s16 endFrame) {
    mAttribute = EMode_LOOP;
    mState = 0;
    mStart = 0;
    mEnd = endFrame;
    mLoop = 0;
    mRate = 1.0f;
    mFrame = 0.0f;
}

int J3DFrameCtrl::checkPass(f32 passFrame) {
    f32 next_frame = mFrame + mRate;

    switch (mAttribute) {
    case 0:
    case 1:
        if (next_frame < mStart) {
            next_frame = mStart;
        }

        if (next_frame >= mEnd) {
            next_frame = mEnd - 0.001f;
        }

        if (mFrame <= next_frame) {
            if (mFrame <= passFrame && passFrame < next_frame) {
                return true;
            } else {
                return false;
            }
        }

        if (next_frame <= passFrame && passFrame < mFrame) {
            return true;
        }
        return false;
    case 2:
        if (mFrame < mStart) {
            while (next_frame < mStart) {
                if (mLoop - mStart <= 0.0f) {
                    break;
                }
                next_frame += mLoop - mStart;
            }

            if (next_frame <= passFrame && passFrame < mLoop) {
                return true;
            } else {
                return false;
            }
        } else if (mEnd <= mFrame) {
            while (next_frame >= mEnd) {            
                if (mEnd - mLoop <= 0.0f) {
                    break;
                }
                next_frame -= mEnd - mLoop;
            }

            if (mLoop <= passFrame && passFrame < next_frame) {
                return true;
            } else {
                return false;
            }
        } else if (next_frame < mStart) {
            while (next_frame < mStart) {            
                if (mLoop - mStart <= 0.0f) {
                    break;
                }
                next_frame += mLoop - mStart;
            }

            if ((mStart <= passFrame && passFrame < mFrame) || (next_frame <= passFrame && passFrame < mLoop)) {
                return true;
            } else {
                return false;
            }
        } else if (mEnd <= next_frame) {
            while (next_frame >= mEnd) {            
                if (mEnd - mLoop <= 0.0f) {
                    break;
                }

                next_frame -= mEnd - mLoop;
            }

            if ((mFrame <= passFrame && passFrame < mEnd) || (mLoop <= passFrame && passFrame < next_frame)) {
                return true;
            } else {
                return false;
            }
        } else if (mFrame <= next_frame) {
            if (mFrame <= passFrame && passFrame < next_frame) {
                return true;
            } else {
                return false;
            }
        } else if (next_frame <= passFrame && passFrame < mFrame) {
            return true;
        }
        return false;
    case 3:
    case 4:
        if (next_frame >= mEnd) {
            next_frame = mEnd - 0.001f;
        }

        if (next_frame < mStart) {
            next_frame = mStart;
        }

        if (mFrame <= next_frame) {
            if (mFrame <= passFrame && passFrame < next_frame) {
                return true;
            } else {
                return false;
            }
        }

        if (next_frame <= passFrame && passFrame < mFrame) {
            return true;
        }
        return false;
    default:
        return false;
    }
}


void J3DFrameCtrl::update() {
    mState = 0;
    mFrame += mRate;

    switch (mAttribute) {
    case EMode_NONE:
        if (mFrame < mStart) {
            mFrame = mStart;
            mRate = 0.0f;
            mState |= (u8)1;
        }
        if (mFrame >= mEnd) {
            mFrame = mEnd - 0.001f;
            mRate = 0.0f;
            mState |= (u8)1;
        }
        break;
    case EMode_RESET:
        if (mFrame < mStart) {
            mFrame = mStart;
            mRate = 0.0f;
            mState |= (u8)1;
        }
        if (mFrame >= mEnd) {
            mFrame = mStart;
            mRate = 0.0f;
            mState |= (u8)1;
        }
        break;
    case EMode_LOOP:
        while (mFrame < mStart) {
            mState |= (u8)2;
            if (mLoop - mStart <= 0.0f) {
                break;
            }
            mFrame += mLoop - mStart;
        }
        while (mFrame >= mEnd) {
            mState |= (u8)2;
            if (mEnd - mLoop <= 0.0f) {
                break;
            }
            mFrame -= mEnd - mLoop;
        }
        break;
    case EMode_REVERSE:
        if (mFrame >= mEnd) {
            mFrame = mEnd - (mFrame - mEnd);
            mRate = -mRate;
        }
        if (mFrame < mStart) {
            mFrame = mStart - (mFrame - mStart);
            mRate = 0.0f;
            mState |= (u8)1;
        }
        break;
    case EMode_LOOP_REVERSE:
        if (mFrame >= mEnd - 1.0f) {
            mFrame = (mEnd - 1.0f) - (mFrame - (mEnd - 1.0f));
            mRate = -mRate;
        }
        if (mFrame < mStart) {
            mFrame = mStart - (mFrame - mStart);
            mRate = -mRate;
            mState |= (u8)2;
        }
        break;
    }
}

J3DAnmTransform::J3DAnmTransform(s16 frameMax, f32* pScaleData, s16* pRotData, f32* pTransData)
 : J3DAnmBase(frameMax) {
    mScaleData = pScaleData;
    mRotData = pRotData;
    mTransData = pTransData;
    field_0x18 = 0;
    field_0x1a = 0;
    field_0x1c = 0;
    field_0x1e = 0;
}

void J3DAnmTransformFull::getTransform(u16 jointNo, J3DTransformInfo* pTransform) const {
    J3D_ASSERT_RANGE(317, jointNo >= 0 && jointNo < field_0x1e);
    J3D_ASSERT_NULLPTR(318, pTransform != NULL);

    u16 idx = jointNo * 3;
    J3DAnmTransformFullTable* entryX = &mAnmTable[idx];
    J3DAnmTransformFullTable* entryY = &mAnmTable[idx + 1];
    J3DAnmTransformFullTable* entryZ = &mAnmTable[idx + 2];

    if (mFrame < 0.0f) {
        pTransform->mScale.x = mScaleData[entryX->mScaleOffset];
        pTransform->mScale.y = mScaleData[entryY->mScaleOffset];
        pTransform->mScale.z = mScaleData[entryZ->mScaleOffset];

        pTransform->mRotation.x = mRotData[entryX->mRotationOffset];
        pTransform->mRotation.y = mRotData[entryY->mRotationOffset];
        pTransform->mRotation.z = mRotData[entryZ->mRotationOffset];

        pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset];
        pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset];
        pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset];
    } else {
        u32 frame_max;
        u32 frame = (int)(mFrame + 0.5f);

        frame_max = entryX->mScaleMaxFrame;
        if (frame >= frame_max) {
            pTransform->mScale.x = mScaleData[entryX->mScaleOffset + (frame_max - 1)];
        } else {
            pTransform->mScale.x = mScaleData[entryX->mScaleOffset + frame];
        }

        frame_max = entryX->mRotationMaxFrame;
        if (frame >= frame_max) {
            pTransform->mRotation.x = mRotData[entryX->mRotationOffset + (frame_max - 1)];
        } else {
            pTransform->mRotation.x = mRotData[entryX->mRotationOffset + frame];
        }

        frame_max = entryX->mTranslateMaxFrame;
        if (frame >= frame_max) {
            pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + (frame_max - 1)];
        } else {
            pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + frame];
        }

        frame_max = entryY->mScaleMaxFrame;
        if (frame >= frame_max) {
            pTransform->mScale.y = mScaleData[entryY->mScaleOffset + (frame_max - 1)];
        } else {
            pTransform->mScale.y = mScaleData[entryY->mScaleOffset + frame];
        }

        frame_max = entryY->mRotationMaxFrame;
        if (frame >= frame_max) {
            pTransform->mRotation.y = mRotData[entryY->mRotationOffset + (frame_max - 1)];
        } else {
            pTransform->mRotation.y = mRotData[entryY->mRotationOffset + frame];
        }

        frame_max = entryY->mTranslateMaxFrame;
        if (frame >= frame_max) {
            pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + (frame_max - 1)];
        } else {
            pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + frame];
        }

        frame_max = entryZ->mScaleMaxFrame;
        if (frame >= frame_max) {
            pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + (frame_max - 1)];
        } else {
            pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + frame];
        }

        frame_max = entryZ->mRotationMaxFrame;
        if (frame >= frame_max) {
            pTransform->mRotation.z = mRotData[entryZ->mRotationOffset + (frame_max - 1)];
        } else {
            pTransform->mRotation.z = mRotData[entryZ->mRotationOffset + frame];
        }

        frame_max = entryZ->mTranslateMaxFrame;
        if (frame >= frame_max) {
            pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + (frame_max - 1)];
        } else {
            pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + frame];
        }
    }
}

void J3DAnmTransformFullWithLerp::getTransform(u16 jointNo, J3DTransformInfo* pTransform) const {
    J3D_ASSERT_RANGE(421, jointNo >= 0 && jointNo < field_0x1e);
    J3D_ASSERT_NULLPTR(422, pTransform != NULL);

    u16 idx = jointNo * 3;
    J3DAnmTransformFullTable* entryX = &mAnmTable[idx];
    J3DAnmTransformFullTable* entryY = &mAnmTable[idx + 1];
    J3DAnmTransformFullTable* entryZ = &mAnmTable[idx + 2];

    if (mFrame < 0.0f) {
        pTransform->mScale.x = mScaleData[entryX->mScaleOffset];
        pTransform->mScale.y = mScaleData[entryY->mScaleOffset];
        pTransform->mScale.z = mScaleData[entryZ->mScaleOffset];

        pTransform->mRotation.x = mRotData[entryX->mRotationOffset];
        pTransform->mRotation.y = mRotData[entryY->mRotationOffset];
        pTransform->mRotation.z = mRotData[entryZ->mRotationOffset];

        pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset];
        pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset];
        pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset];
    } else {
        u32 frame_max;
        int frame = (int)mFrame;

        if (frame == mFrame) {
            frame_max = entryX->mScaleMaxFrame;
            if (frame >= frame_max) {
                pTransform->mScale.x = mScaleData[entryX->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.x = mScaleData[entryX->mScaleOffset + frame];
            }

            frame_max = entryX->mRotationMaxFrame;
            if (frame >= frame_max) {
                pTransform->mRotation.x = mRotData[entryX->mRotationOffset + (frame_max - 1)];
            } else {
                pTransform->mRotation.x = mRotData[entryX->mRotationOffset + frame];
            }

            frame_max = entryX->mTranslateMaxFrame;
            if (frame >= frame_max) {
                pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + frame];
            }

            frame_max = entryY->mScaleMaxFrame;
            if (frame >= frame_max) {
                pTransform->mScale.y = mScaleData[entryY->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.y = mScaleData[entryY->mScaleOffset + frame];
            }

            frame_max = entryY->mRotationMaxFrame;
            if (frame >= frame_max) {
                pTransform->mRotation.y = mRotData[entryY->mRotationOffset + (frame_max - 1)];
            } else {
                pTransform->mRotation.y = mRotData[entryY->mRotationOffset + frame];
            }

            frame_max = entryY->mTranslateMaxFrame;
            if (frame >= frame_max) {
                pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + frame];
            }

            frame_max = entryZ->mScaleMaxFrame;
            if (frame >= frame_max) {
                pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + frame];
            }

            frame_max = entryZ->mRotationMaxFrame;
            if (frame >= frame_max) {
                pTransform->mRotation.z = mRotData[entryZ->mRotationOffset + (frame_max - 1)];
            } else {
                pTransform->mRotation.z = mRotData[entryZ->mRotationOffset + frame];
            }

            frame_max = entryZ->mTranslateMaxFrame;
            if (frame >= frame_max) {
                pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + frame];
            }
        } else {
            f32 rate = mFrame - frame;
            f32 var_f30 = 1.0f - rate;

            u32 next_frame = frame + 1;

            frame_max = entryX->mScaleMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mScale.x = mScaleData[entryX->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.x = mScaleData[entryX->mScaleOffset + frame]
                    + rate * (mScaleData[entryX->mScaleOffset + next_frame]
                            - mScaleData[entryX->mScaleOffset + frame]);
            }

            frame_max = entryX->mRotationMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mRotation.x = mRotData[entryX->mRotationOffset + (frame_max - 1)];
            } else {
                u32 rot1 = (u16)mRotData[entryX->mRotationOffset + frame];
                u32 rot2 = (u16)mRotData[entryX->mRotationOffset + next_frame];
                int delta = rot2 - rot1;
                if (delta > 0x8000) {
                    rot1 += 0x10000;
                    delta -= 0x10000;
                } else if (-delta > 0x8000) {
                    delta += 0x10000;
                }
                pTransform->mRotation.x = (u32)((f32)rot1 + rate * (f32)delta);
            }

            frame_max = entryX->mTranslateMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.x = mTransData[entryX->mTranslateOffset + frame]
                    + rate * (mTransData[entryX->mTranslateOffset + next_frame]
                            - mTransData[entryX->mTranslateOffset + frame]);
            }

            frame_max = entryY->mScaleMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mScale.y = mScaleData[entryY->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.y = mScaleData[entryY->mScaleOffset + frame]
                    + rate * (mScaleData[entryY->mScaleOffset + next_frame]
                            - mScaleData[entryY->mScaleOffset + frame]);
            }

            frame_max = entryY->mRotationMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mRotation.y = mRotData[entryY->mRotationOffset + (frame_max - 1)];
            } else {
                u32 rot1 = (u16)mRotData[entryY->mRotationOffset + frame];
                u32 rot2 = (u16)mRotData[entryY->mRotationOffset + next_frame];
                int delta = rot2 - rot1;
                if (delta > 0x8000) {
                    rot1 += 0x10000;
                    delta -= 0x10000;
                } else if (-delta > 0x8000) {
                    delta += 0x10000;
                }
                pTransform->mRotation.y = (u32)((f32)rot1 + rate * (f32)delta);
            }

            frame_max = entryY->mTranslateMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.y = mTransData[entryY->mTranslateOffset + frame]
                    + rate * (mTransData[entryY->mTranslateOffset + next_frame]
                            - mTransData[entryY->mTranslateOffset + frame]);
            }

            frame_max = entryZ->mScaleMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + (frame_max - 1)];
            } else {
                pTransform->mScale.z = mScaleData[entryZ->mScaleOffset + frame]
                    + rate * (mScaleData[entryZ->mScaleOffset + next_frame]
                            - mScaleData[entryZ->mScaleOffset + frame]);
            }

            frame_max = entryZ->mRotationMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mRotation.z = mRotData[entryZ->mRotationOffset + (frame_max - 1)];
            } else {
                u32 rot1 = (u16)mRotData[entryZ->mRotationOffset + frame];
                u32 rot2 = (u16)mRotData[entryZ->mRotationOffset + next_frame];
                int delta = rot2 - rot1;
                if (delta > 0x8000) {
                    rot1 += 0x10000;
                    delta -= 0x10000;
                } else if (-delta > 0x8000) {
                    delta += 0x10000;
                }
                pTransform->mRotation.z = (u32)((f32)rot1 + rate * (f32)delta);
            }

            frame_max = entryZ->mTranslateMaxFrame;
            if (next_frame >= frame_max) {
                pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + (frame_max - 1)];
            } else {
                pTransform->mTranslate.z = mTransData[entryZ->mTranslateOffset + frame]
                    + rate * (mTransData[entryZ->mTranslateOffset + next_frame]
                            - mTransData[entryZ->mTranslateOffset + frame]);
            }
        }
    }
}

inline f32 J3DHermiteInterpolation(f32 p1, f32 const* p2, f32 const* p3, f32 const* p4,
                                   f32 const* p5, f32 const* p6, f32 const* p7) {
    return JMAHermiteInterpolation(p1, *p2, *p3, *p4, *p5, *p6, *p7);
}

inline f32 J3DHermiteInterpolation(__REGISTER f32 pp1, __REGISTER s16 const* pp2,
                                   __REGISTER s16 const* pp3, __REGISTER s16 const* pp4,
                                   __REGISTER s16 const* pp5, __REGISTER s16 const* pp6,
                                   __REGISTER s16 const* pp7) {
#ifdef __MWERKS__
    register f32 p1 = pp1;
    register f32 ff8;
    register f32 ff7;
    register f32 ff6;
    register f32 ff5;
    register f32 ff4;
    register f32 ff3;
    register f32 ff2;
    register f32 ff0;
    register f32 fout;
    register s16 const* p2 = pp2;
    register s16 const* p3 = pp3;
    register s16 const* p4 = pp4;
    register s16 const* p5 = pp5;
    register s16 const* p6 = pp6;
    register s16 const* p7 = pp7;
    // clang-format off
    asm {
        psq_l ff2, 0(p2), 0x1, 5
        psq_l ff0, 0(p5), 0x1, 5
        psq_l ff7, 0(p3), 0x1, 5
        fsubs ff5, ff0, ff2
        psq_l ff6, 0(p6), 0x1, 5
        fsubs ff3, p1, ff2
        psq_l ff0, 0(p7), 0x1, 5
        fsubs ff4, ff6, ff7
        fdivs ff3, ff3, ff5
        psq_l fout, 0(p4), 0x1, 5
        fmadds ff0, ff0, ff5, ff7
        fmuls ff2, ff3, ff3
        fnmsubs ff4, ff5, fout, ff4
        fsubs ff0, ff0, ff6
        fsubs ff0, ff0, ff4
        fmuls ff0, ff2, ff0
        fmadds fout, ff5, fout, ff0
        fmadds fout, fout, ff3, ff7
        fmadds fout, ff4, ff2, fout
        fsubs fout, fout, ff0
    }
    // clang-format on
    return fout;
#endif
}

template<typename T>
f32 J3DGetKeyFrameInterpolation(f32 frame, J3DAnmKeyTableBase* pKeyTable, T* pData) {
    J3D_ASSERT_NULLPTR(740, pData != NULL);

    if (frame < pData[0]) {
        return pData[1];
    }

    if (pKeyTable->mType == 0) {
        u32 idx = pKeyTable->mMaxFrame - 1;
        if (pData[idx * 3] <= frame) {
            return pData[idx * 3 + 1];
        }

        u32 uVar7 = pKeyTable->mMaxFrame;
        while (uVar7 > 1) {
            u32 uVar2 = uVar7 >> 1;
            u32 tmp = uVar2 * 3;
            if (frame >= pData[tmp]) {
                pData += tmp;
                uVar7 = uVar7 - uVar2;
            } else {
                uVar7 = uVar2;
            }
        }

        f32 interpolated = J3DHermiteInterpolation(frame, &pData[0], &pData[1], &pData[2], &pData[3], &pData[4], &pData[5]);
        return interpolated;
    } else {
        u32 idx = pKeyTable->mMaxFrame - 1;
        if (pData[idx * 4] <= frame) {
            return pData[idx * 4 + 1];
        }

        u32 var_r27 = pKeyTable->mMaxFrame;
        while (var_r27 > 1) {
            u32 var_r25 = var_r27 >> 1;
            u32 var_r23 = var_r25 * 4;
            if (frame >= pData[var_r23]) {
                pData += var_r23;
                var_r27 = var_r27 - var_r25;
            } else {
                var_r27 = var_r25;
            }
        }

        f32 interpolated = J3DHermiteInterpolation(frame, &pData[0], &pData[1], &pData[3], &pData[4], &pData[5], &pData[6]);
        return interpolated;
    }
}

void J3DAnmTransformKey::calcTransform(f32 frame, u16 jointNo, J3DTransformInfo* pTransform) const {
    J3D_ASSERT_RANGE(829, jointNo >= 0 && jointNo < field_0x1e);
    J3D_ASSERT_NULLPTR(830, pTransform != NULL);

    u16 idx = jointNo * 3;
    J3DAnmTransformKeyTable* entryX = &mAnmTable[idx];
    J3DAnmTransformKeyTable* entryY = &mAnmTable[idx + 1];
    J3DAnmTransformKeyTable* entryZ = &mAnmTable[idx + 2];

    switch (entryX->mScaleInfo.mMaxFrame) {
    case 0:
        pTransform->mScale.x = 1.0f;
        break;
    case 1:
        pTransform->mScale.x = mScaleData[entryX->mScaleInfo.mOffset];
        break;
    default:
        pTransform->mScale.x = J3DGetKeyFrameInterpolation(frame, &entryX->mScaleInfo,
                                                &mScaleData[entryX->mScaleInfo.mOffset]);
    }

    switch (entryY->mScaleInfo.mMaxFrame) {
    case 0:
        pTransform->mScale.y = 1.0f;
        break;
    case 1:
        pTransform->mScale.y = mScaleData[entryY->mScaleInfo.mOffset];
        break;
    default:
        pTransform->mScale.y = J3DGetKeyFrameInterpolation(frame, &entryY->mScaleInfo,
                                                &mScaleData[entryY->mScaleInfo.mOffset]);
    }

    switch (entryZ->mScaleInfo.mMaxFrame) {
    case 0:
        pTransform->mScale.z = 1.0f;
        break;
    case 1:
        pTransform->mScale.z = mScaleData[entryZ->mScaleInfo.mOffset];
        break;
    default:
        pTransform->mScale.z = J3DGetKeyFrameInterpolation(frame, &entryZ->mScaleInfo,
                                                &mScaleData[entryZ->mScaleInfo.mOffset]);
    }

    switch (entryX->mRotationInfo.mMaxFrame) {
    case 0:
        pTransform->mRotation.x = 0;
        break;
    case 1:
        pTransform->mRotation.x = mRotData[entryX->mRotationInfo.mOffset] << mDecShift;
        break;
    default:
        pTransform->mRotation.x = (int)J3DGetKeyFrameInterpolation(frame, &entryX->mRotationInfo,
                                            &mRotData[entryX->mRotationInfo.mOffset]) << mDecShift;
    }

    switch (entryY->mRotationInfo.mMaxFrame) {
    case 0:
        pTransform->mRotation.y = 0;
        break;
    case 1:
        pTransform->mRotation.y = mRotData[entryY->mRotationInfo.mOffset] << mDecShift;
        break;
    default:
        pTransform->mRotation.y = (int)J3DGetKeyFrameInterpolation(frame, &entryY->mRotationInfo,
                                            &mRotData[entryY->mRotationInfo.mOffset]) << mDecShift;
    }

    switch (entryZ->mRotationInfo.mMaxFrame) {
    case 0:
        pTransform->mRotation.z = 0;
        break;
    case 1:
        pTransform->mRotation.z = mRotData[entryZ->mRotationInfo.mOffset] << mDecShift;
        break;
    default:
        pTransform->mRotation.z = (int)J3DGetKeyFrameInterpolation(frame, &entryZ->mRotationInfo,
                                            &mRotData[entryZ->mRotationInfo.mOffset]) << mDecShift;
    }

    switch (entryX->mTranslateInfo.mMaxFrame) {
    case 0:
        pTransform->mTranslate.x = 0.0f;
        break;
    case 1:
        pTransform->mTranslate.x = mTransData[entryX->mTranslateInfo.mOffset];
        break;
    default:
        pTransform->mTranslate.x = J3DGetKeyFrameInterpolation(frame, &entryX->mTranslateInfo,
                                                    &mTransData[entryX->mTranslateInfo.mOffset]);
    }

    switch (entryY->mTranslateInfo.mMaxFrame) {
    case 0:
        pTransform->mTranslate.y = 0.0f;
        break;
    case 1:
        pTransform->mTranslate.y = mTransData[entryY->mTranslateInfo.mOffset];
        break;
    default:
        pTransform->mTranslate.y = J3DGetKeyFrameInterpolation(frame, &entryY->mTranslateInfo,
                                                    &mTransData[entryY->mTranslateInfo.mOffset]);
    }

    switch (entryZ->mTranslateInfo.mMaxFrame) {
    case 0:
        pTransform->mTranslate.z = 0.0f;
        break;
    case 1:
        pTransform->mTranslate.z = mTransData[entryZ->mTranslateInfo.mOffset];
        break;
    default:
        pTransform->mTranslate.z = J3DGetKeyFrameInterpolation(frame, &entryZ->mTranslateInfo,
                                                    &mTransData[entryZ->mTranslateInfo.mOffset]);
    }
}

J3DAnmTextureSRTKey::J3DAnmTextureSRTKey() {
    mDecShift = 0;
    mTrackNum = mScaleNum = mRotNum = mTransNum = 0;
    mAnmTable = NULL;
    mScaleData = mTransData = NULL;
    mRotData = NULL;
    field_0x4a = field_0x44 = field_0x46 = field_0x48 = 0;
    field_0x58 = NULL;
    field_0x4c = field_0x54 = NULL;
    field_0x50 = NULL;
    mTexMtxCalcType = 0;
}

void J3DAnmTextureSRTKey::calcTransform(f32 frame, u16 jointNo, J3DTextureSRTInfo* pTexSRTInfo) const {
    J3D_ASSERT_RANGE(992, jointNo >= 0 && jointNo < mTrackNum);
    J3D_ASSERT_NULLPTR(993, pTexSRTInfo != NULL);

    u16 idx = jointNo * 3;
    J3DAnmTransformKeyTable* entryX = &mAnmTable[idx];
    J3DAnmTransformKeyTable* entryY = &mAnmTable[idx + 1];
    J3DAnmTransformKeyTable* entryRot = &mAnmTable[idx + 2];

    switch (entryX->mScaleInfo.mMaxFrame) {
    case 0:
        pTexSRTInfo->mScaleX = 1.0f;
        break;
    case 1:
        pTexSRTInfo->mScaleX = mScaleData[entryX->mScaleInfo.mOffset];
        break;
    default:
        pTexSRTInfo->mScaleX = J3DGetKeyFrameInterpolation(frame, &entryX->mScaleInfo,
                                                &mScaleData[entryX->mScaleInfo.mOffset]);
    }

    switch (entryY->mScaleInfo.mMaxFrame) {
    case 0:
        pTexSRTInfo->mScaleY = 1.0f;
        break;
    case 1:
        pTexSRTInfo->mScaleY = mScaleData[entryY->mScaleInfo.mOffset];
        break;
    default:
        pTexSRTInfo->mScaleY = J3DGetKeyFrameInterpolation(frame, &entryY->mScaleInfo,
                                                &mScaleData[entryY->mScaleInfo.mOffset]);
    }

    switch (entryRot->mRotationInfo.mMaxFrame) {
    case 0:
        pTexSRTInfo->mRotation = 0;
        break;
    case 1:
        pTexSRTInfo->mRotation = mRotData[entryRot->mRotationInfo.mOffset] << mDecShift;
        break;
    default:
        pTexSRTInfo->mRotation = (int)J3DGetKeyFrameInterpolation(frame, &entryRot->mRotationInfo,
                                        &mRotData[entryRot->mRotationInfo.mOffset]) << mDecShift;
    }

    switch (entryX->mTranslateInfo.mMaxFrame) {
    case 0:
        pTexSRTInfo->mTranslationX = 0.0f;
        break;
    case 1:
        pTexSRTInfo->mTranslationX = mTransData[entryX->mTranslateInfo.mOffset];
        break;
    default:
        pTexSRTInfo->mTranslationX = J3DGetKeyFrameInterpolation(frame, &entryX->mTranslateInfo,
                                                    &mTransData[entryX->mTranslateInfo.mOffset]);
    }

    switch (entryY->mTranslateInfo.mMaxFrame) {
    case 0:
        pTexSRTInfo->mTranslationY = 0.0f;
        break;
    case 1:
        pTexSRTInfo->mTranslationY = mTransData[entryY->mTranslateInfo.mOffset];
        break;
    default:
        pTexSRTInfo->mTranslationY = J3DGetKeyFrameInterpolation(frame, &entryY->mTranslateInfo,
                                                    &mTransData[entryY->mTranslateInfo.mOffset]);
    }
}

f32 J3DAnmClusterFull::getWeight(u16 index) const {
    int maxFrame = mAnmTable[index].mMaxFrame;
    int frame = (int)(mFrame + 0.5f);

    if (mFrame < 0.0f) {
        return mWeight[mAnmTable[index].mOffset];
    } else if (frame >= (u16)maxFrame) {
        return mWeight[mAnmTable[index].mOffset + ((u16)maxFrame - 1)];
    } else {
        return mWeight[mAnmTable[index].mOffset + frame];
    }
}

f32 J3DAnmClusterKey::getWeight(u16 index) const {
    int maxFrame = (u16)mAnmTable[index].mWeightTable.mMaxFrame;
    switch (maxFrame) {
    case 0:
        return 1.0f;
    case 1:
        return mWeight[mAnmTable[index].mWeightTable.mOffset];
    default:
        return J3DGetKeyFrameInterpolation(mFrame, &mAnmTable[index].mWeightTable,
                                           &mWeight[mAnmTable[index].mWeightTable.mOffset]);
    }
}

J3DAnmVtxColor::J3DAnmVtxColor() {
    for (int i = 0; i < 2; i++) {
        mAnmTableNum[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        mAnmVtxColorIndexData[i] = NULL;
    }
}

J3DAnmVtxColorFull::J3DAnmVtxColorFull() {
    for (int i = 0; i < 2; i++) {
        mpTable[i] = NULL;
    }
}

void J3DAnmVtxColorFull::getColor(u8 tableNo, u16 index, GXColor* pColor) const {
    J3D_ASSERT_NULLPTR(1254, pColor != NULL);
    J3DAnmColorFullTable* entry = &mpTable[tableNo][index];

    if (mFrame < 0.0f) {
        pColor->r = mColorR[entry->mROffset];
        pColor->g = mColorG[entry->mGOffset];
        pColor->b = mColorB[entry->mBOffset];
        pColor->a = mColorA[entry->mAOffset];
    } else {
        int frame = (int)(mFrame + 0.5f);
        u16 maxFrame;

        maxFrame = entry->mRMaxFrame;
        if (frame >= maxFrame) {
            pColor->r = mColorR[entry->mROffset + (maxFrame - 1)];
        } else {
            pColor->r = mColorR[entry->mROffset + frame];
        }

        maxFrame = entry->mGMaxFrame;
        if (frame >= maxFrame) {
            pColor->g = mColorG[entry->mGOffset + (maxFrame - 1)];
        } else {
            pColor->g = mColorG[entry->mGOffset + frame];
        }

        maxFrame = entry->mBMaxFrame;
        if (frame >= maxFrame) {
            pColor->b = mColorB[entry->mBOffset + (maxFrame - 1)];
        } else {
            pColor->b = mColorB[entry->mBOffset + frame];
        }

        maxFrame = entry->mAMaxFrame;
        if (frame >= maxFrame) {
            pColor->a = mColorA[entry->mAOffset + (maxFrame - 1)];
        } else {
            pColor->a = mColorA[entry->mAOffset + frame];
        }
    }
}

J3DAnmVtxColorKey::J3DAnmVtxColorKey() {
    for (int i = 0; i < 2; i++) {
        mpTable[i] = 0;
    }
}

void J3DAnmVtxColorKey::getColor(u8 tableNo, u16 index, GXColor* pColor) const {
    J3D_ASSERT_NULLPTR(1338, pColor != NULL);
    J3DAnmColorKeyTable* entry = &mpTable[tableNo][index];

    f32 col;
    switch (entry->mRInfo.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mColorR[entry->mRInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mRInfo,
                                          &mColorR[entry->mRInfo.mOffset]);
        if (col <= 0.0f) {
            pColor->r = 0;
        } else if (col <= 255.0f) {
            OSf32tou8(&col, &pColor->r);
        } else {
            pColor->r = 255;
        }
    }

    switch (entry->mGInfo.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mColorG[entry->mGInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mGInfo,
                                          &mColorG[entry->mGInfo.mOffset]);
        if (col <= 0.0f) {
            pColor->g = 0;
        } else if (col <= 255.0f) {
            OSf32tou8(&col, &pColor->g);
        } else {
            pColor->g = 255;
        }
    }

    switch (entry->mBInfo.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mColorB[entry->mBInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mBInfo,
                                          &mColorB[entry->mBInfo.mOffset]);
        if (col <= 0.0f) {
            pColor->b = 0;
        } else if (col <= 255.0f) {
            OSf32tou8(&col, &pColor->b);
        } else {
            pColor->b = 255;
        }
    }

    switch (entry->mAInfo.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mColorA[entry->mAInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mAInfo,
                                          &mColorA[entry->mAInfo.mOffset]);
        if (col <= 0.0f) {
            pColor->a = 0;
        } else if (col <= 255.0f) {
            OSf32tou8(&col, &pColor->a);
        } else {
            pColor->a = 255;
        }
    }
}

J3DAnmColor::J3DAnmColor() : field_0xc(0), field_0xe(0), field_0x10(0),
    field_0x12(0), mUpdateMaterialNum(0), mUpdateMaterialID(NULL) {}


void J3DAnmColor::searchUpdateMaterialID(J3DMaterialTable* pMatTable) {
    for (u16 i = 0; i < mUpdateMaterialNum; i++) {
        int index = pMatTable->getMaterialName()->getIndex(mUpdateMaterialName.getName(i));
        if (index != -1) {
            mUpdateMaterialID[i] = index;
        } else {
            mUpdateMaterialID[i] = 0xffff;
        }
    }
}

J3DAnmColorFull::J3DAnmColorFull() {
    mColorR = NULL;
    mColorG = NULL;
    mColorB = NULL;
    mColorA = NULL;
    mAnmTable = NULL;
}

void J3DAnmColorFull::getColor(u16 index, GXColor* pColor) const {
    J3D_ASSERT_RANGE(1537, index >= 0 && index < mUpdateMaterialNum);
    J3D_ASSERT_NULLPTR(1538, pColor != NULL);
    J3DAnmColorFullTable* entry = &mAnmTable[index];

    if (mFrame < 0.0f) {
        pColor->r = mColorR[entry->mROffset];
        pColor->g = mColorG[entry->mGOffset];
        pColor->b = mColorB[entry->mBOffset];
        pColor->a = mColorA[entry->mAOffset];
    } else {
        int frame = (int)(mFrame + 0.5f);
        u16 maxFrame;

        maxFrame = entry->mRMaxFrame;
        if (frame >= maxFrame) {
            pColor->r = mColorR[entry->mROffset + (maxFrame - 1)];
        } else {
            pColor->r = mColorR[entry->mROffset + frame];
        }

        maxFrame = entry->mGMaxFrame;
        if (frame >= maxFrame) {
            pColor->g = mColorG[entry->mGOffset + (maxFrame - 1)];
        } else {
            pColor->g = mColorG[entry->mGOffset + frame];
        }

        maxFrame = entry->mBMaxFrame;
        if (frame >= maxFrame) {
            pColor->b = mColorB[entry->mBOffset + (maxFrame - 1)];
        } else {
            pColor->b = mColorB[entry->mBOffset + frame];
        }

        maxFrame = entry->mAMaxFrame;
        if (frame >= maxFrame) {
            pColor->a = mColorA[entry->mAOffset + (maxFrame - 1)];
        } else {
            pColor->a = mColorA[entry->mAOffset + frame];
        }
    }
}

J3DAnmColorKey::J3DAnmColorKey() {
    mColorR = NULL;
    mColorG = NULL;
    mColorB = NULL;
    mColorA = NULL;
    mAnmTable = NULL;
}

void J3DAnmColorKey::getColor(u16 index, GXColor* pColor) const {
    J3D_ASSERT_RANGE(1614, index >= 0 && index < mUpdateMaterialNum);
    J3D_ASSERT_NULLPTR(1615, pColor != NULL);
    J3DAnmColorKeyTable* entry = &mAnmTable[index];

    f32 col;
    switch (entry->mRInfo.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mColorR[entry->mRInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mRInfo,
                                          &mColorR[entry->mRInfo.mOffset]);
        if (col < 0.0f) {
            pColor->r = 0;
        } else if (col > 255.0f) {
            pColor->r = 255;
        } else {
            OSf32tou8(&col, &pColor->r);
        }
    }

    switch (entry->mGInfo.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mColorG[entry->mGInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mGInfo,
                                          &mColorG[entry->mGInfo.mOffset]);
        if (col < 0.0f) {
            pColor->g = 0;
        } else if (col > 255.0f) {
            pColor->g = 255;
        } else {
            OSf32tou8(&col, &pColor->g);
        }
    }

    switch (entry->mBInfo.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mColorB[entry->mBInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mBInfo,
                                          &mColorB[entry->mBInfo.mOffset]);
        if (col < 0.0f) {
            pColor->b = 0;
        } else if (col > 255.0f) {
            pColor->b = 255;
        } else {
            OSf32tou8(&col, &pColor->b);
        }
    }

    switch (entry->mAInfo.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mColorA[entry->mAInfo.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mAInfo,
                                          &mColorA[entry->mAInfo.mOffset]);
        if (col < 0.0f) {
            pColor->a = 0;
        } else if (col > 255.0f) {
            pColor->a = 255;
        } else {
            OSf32tou8(&col, &pColor->a);
        }
    }
}

J3DAnmTevRegKey::J3DAnmTevRegKey() {
    mCRegUpdateMaterialNum = mKRegUpdateMaterialNum = 0;

    mCRegDataCountR =
    mCRegDataCountG =
    mCRegDataCountB =
    mCRegDataCountA = 0;

    mKRegDataCountR =
    mKRegDataCountG =
    mKRegDataCountB =
    mKRegDataCountA = 0;

    mCRegUpdateMaterialID = mKRegUpdateMaterialID = NULL;
    
    mAnmCRegDataR =
    mAnmCRegDataG =
    mAnmCRegDataB =
    mAnmCRegDataA = NULL;

    mAnmKRegDataR =
    mAnmKRegDataG =
    mAnmKRegDataB =
    mAnmKRegDataA = NULL;
}


J3DAnmTexPattern::J3DAnmTexPattern() : mTextureIndex(NULL), mAnmTable(NULL),
    field_0x14(0), mUpdateMaterialNum(0), mUpdateMaterialID(NULL) {}

void J3DAnmTexPattern::getTexNo(u16 index, u16* pTexNo) const {
    u32 maxFrame = mAnmTable[index].mMaxFrame;
    J3D_ASSERT_RANGE(1743, index >= 0 && index < mUpdateMaterialNum);
    J3D_ASSERT_NULLPTR(1744, pTexNo != NULL);

    if (mFrame < 0.0f) {
        *pTexNo = mTextureIndex[mAnmTable[index].mOffset];
    } else if (mFrame >= (u16)maxFrame) {
        *pTexNo = mTextureIndex[mAnmTable[index].mOffset + ((u16)maxFrame - 1)];
    } else {
        *pTexNo = mTextureIndex[mAnmTable[index].mOffset + (int)mFrame];
    }
}

void J3DAnmTexPattern::searchUpdateMaterialID(J3DMaterialTable* pMatTable) {
    J3D_ASSERT_NULLPTR(1790, pMatTable != NULL);

    for (u16 i = 0; i < mUpdateMaterialNum; i++) {
        s32 index = pMatTable->getMaterialName()->getIndex(mUpdateMaterialName.getName(i));
        if (index != -1) {
            mUpdateMaterialID[i] = index;
        } else {
            mUpdateMaterialID[i] = -1;
        }
    }
}

void J3DAnmTexPattern::searchUpdateMaterialID(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(1813, pModelData != NULL);
    searchUpdateMaterialID(&pModelData->getMaterialTable());
}

void J3DAnmTextureSRTKey::searchUpdateMaterialID(J3DMaterialTable* pMatTable) {
    J3D_ASSERT_NULLPTR(1832, pMatTable != NULL);

    for (u16 i = 0; i < getUpdateMaterialNum(); i++) {
        s32 index = pMatTable->getMaterialName()->getIndex(mUpdateMaterialName.getName(i));
        if (index != -1) {
            mUpdateMaterialID[i] = index;
        } else {
            mUpdateMaterialID[i] = -1;
        }
    }

    for (u16 i = 0; i < getPostUpdateMaterialNum(); i++) {
        s32 index = pMatTable->getMaterialName()->getIndex(mPostUpdateMaterialName.getName(i));
        if (index != -1) {
            mPostUpdateMaterialID[i] = index;
        } else {
            mPostUpdateMaterialID[i] = -1;
        }
    }
}

void J3DAnmTextureSRTKey::searchUpdateMaterialID(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(1871, pModelData != NULL);
    searchUpdateMaterialID(&pModelData->getMaterialTable());
}

void J3DAnmTevRegKey::getTevColorReg(u16 index, GXColorS10* pColor) const {
    J3D_ASSERT_RANGE(1887, index >= 0 && index < mCRegUpdateMaterialNum);
    J3D_ASSERT_NULLPTR(1888, pColor != NULL);
    J3DAnmCRegKeyTable* entry = &mAnmCRegKeyTable[index];

    f32 col;
    switch (entry->mRTable.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mAnmCRegDataR[entry->mRTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mRTable,
                                          &mAnmCRegDataR[entry->mRTable.mOffset]);
        if (col < -0x400) {
            pColor->r = -0x400;
        } else if (col > 0x3FF) {
            pColor->r = 0x3FF;
        } else {
            OSf32tos16(&col, &pColor->r);
        }
    }

    switch (entry->mGTable.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mAnmCRegDataG[entry->mGTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mGTable,
                                          &mAnmCRegDataG[entry->mGTable.mOffset]);
        if (col < -0x400) {
            pColor->g = -0x400;
        } else if (col > 0x3FF) {
            pColor->g = 0x3FF;
        } else {
            OSf32tos16(&col, &pColor->g);
        }
    }

    switch (entry->mBTable.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mAnmCRegDataB[entry->mBTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mBTable,
                                          &mAnmCRegDataB[entry->mBTable.mOffset]);
        if (col < -0x400) {
            pColor->b = -0x400;
        } else if (col > 0x3FF) {
            pColor->b = 0x3FF;
        } else {
            OSf32tos16(&col, &pColor->b);
        }
    }

    switch (entry->mATable.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mAnmCRegDataA[entry->mATable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mATable,
                                          &mAnmCRegDataA[entry->mATable.mOffset]);
        if (col < -0x400) {
            pColor->a = -0x400;
        } else if (col > 0x3FF) {
            pColor->a = 0x3FF;
        } else {
            OSf32tos16(&col, &pColor->a);
        }
    }
}

void J3DAnmTevRegKey::getTevKonstReg(u16 index, GXColor* pColor) const {
    J3D_ASSERT_RANGE(1989, index >= 0 && index < mKRegUpdateMaterialNum);
    J3D_ASSERT_NULLPTR(1990, pColor != NULL);
    J3DAnmKRegKeyTable* entry = &mAnmKRegKeyTable[index];

    f32 col;
    switch (entry->mRTable.mMaxFrame) {
    case 0:
        pColor->r = 0;
        break;
    case 1:
        pColor->r = mAnmKRegDataR[entry->mRTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mRTable,
                                          &mAnmKRegDataR[entry->mRTable.mOffset]);
        if (col < 0) {
            pColor->r = 0;
        } else if (col > 0xFF) {
            pColor->r = 0xFF;
        } else {
            OSf32tou8(&col, &pColor->r);
        }
    }

    switch (entry->mGTable.mMaxFrame) {
    case 0:
        pColor->g = 0;
        break;
    case 1:
        pColor->g = mAnmKRegDataG[entry->mGTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mGTable,
                                          &mAnmKRegDataG[entry->mGTable.mOffset]);
        if (col < 0) {
            pColor->g = 0;
        } else if (col > 0xFF) {
            pColor->g = 0xFF;
        } else {
            OSf32tou8(&col, &pColor->g);
        }
    }

    switch (entry->mBTable.mMaxFrame) {
    case 0:
        pColor->b = 0;
        break;
    case 1:
        pColor->b = mAnmKRegDataB[entry->mBTable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mBTable,
                                          &mAnmKRegDataB[entry->mBTable.mOffset]);
        if (col < 0) {
            pColor->b = 0;
        } else if (col > 0xFF) {
            pColor->b = 0xFF;
        } else {
            OSf32tou8(&col, &pColor->b);
        }
    }

    switch (entry->mATable.mMaxFrame) {
    case 0:
        pColor->a = 0;
        break;
    case 1:
        pColor->a = mAnmKRegDataA[entry->mATable.mOffset];
        break;
    default:
        col = J3DGetKeyFrameInterpolation(mFrame, &entry->mATable,
                                          &mAnmKRegDataA[entry->mATable.mOffset]);
        if (col < 0) {
            pColor->a = 0;
        } else if (col > 0xFF) {
            pColor->a = 0xFF;
        } else {
            OSf32tou8(&col, &pColor->a);
        }
    }
}

void J3DAnmTevRegKey::searchUpdateMaterialID(J3DMaterialTable* pMatTable) {
    J3D_ASSERT_NULLPTR(2083, pMatTable != NULL);

    for (u16 i = 0; i < mCRegUpdateMaterialNum; i++) {
        s32 index = pMatTable->getMaterialName()->getIndex(mCRegUpdateMaterialName.getName(i));
        if (index != -1) {
            mCRegUpdateMaterialID[i] = index;
        } else {
            mCRegUpdateMaterialID[i] = -1;
        }
    }

    for (u16 i = 0; i < mKRegUpdateMaterialNum; i++) {
        s32 index = pMatTable->getMaterialName()->getIndex(mKRegUpdateMaterialName.getName(i));
        if (index != -1) {
            mKRegUpdateMaterialID[i] = index;
        } else {
            mKRegUpdateMaterialID[i] = -1;
        }
    }
}

void J3DAnmTevRegKey::searchUpdateMaterialID(J3DModelData* pModelData) {
    J3D_ASSERT_NULLPTR(2119, pModelData != NULL);
    searchUpdateMaterialID(&pModelData->getMaterialTable());
}
