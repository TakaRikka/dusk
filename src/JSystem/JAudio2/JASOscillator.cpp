#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASOscillator.h"

JASOscillator::JASOscillator() {
    mData = NULL;
    _14 = 0;
    mDirectRelease = 0;
    _18 = 0;
    _1C = 0;
    _0C = 0.0f;
    _10 = 0.0f;
    _08 = 0.0f;
    _04 = 0.0f;
}


void JASOscillator::initStart(JASOscillator::Data const* data) {
    mData = data;
    _04 = 0.0f;
    _08 = 0.0f;
    _0C = 0.0f;
    _14 = 0;
    mDirectRelease = 0;
    if (!data) {
		_1C = 0;
        return;
	}

    if (mData->mTable == NULL) {
        _1C = 2;
        _08 = 1.0f;
        return;
    }

    _10 = mData->mTable[0]._4 / 32768.0f;
    _18 = mData->mTable[0]._0;
    _1C = 1;
}

void JASOscillator::incCounter(f32 param_0) {
    const short* v1;
	switch (_1C) {
	case 0:
	case 2:
		return;
	case 1:
		break;
	}
	_04 += param_0 * mData->_04;
    update();
}

f32 JASOscillator::getValue() const {
	return _08 * mData->mScale + mData->_14;
}

void JASOscillator::release() {
	if (mData->_0C == NULL && mDirectRelease == 0) {
        stop();
        return;
	}

	if (mDirectRelease != 0) {
		_04 = 0.0f;
        _0C = _08;
        _10 = 0.0f;
        _14 = 0;
        _18 = (mDirectRelease >> 14) & 3;
        _1C = 4;
        update();
        return;
	}
	
    if (mData->mTable != mData->_0C) {
        _04 = 0.0f;
        _0C = _08;
        _10 = mData->_0C[0]._4 / 32768.0f;
        _14 = 0;
        _18 = mData->_0C[0]._0;
    }

    _1C = 3;
    update();
}

void JASOscillator::update() {
    if (_1C == 4) {
        s16 x = mDirectRelease & 0x3FFF;
        if (_04 < x) {
            updateCurrentValue(x);
        } else {
            _08 = _10;
            _1C = 0;
        }
        return;
    }

    if (_1C == 2) return;

    const Point* psVar1;
    if (_1C == 1) {
        psVar1 = mData->mTable;
    } else {
        psVar1 = mData->_0C;
    }

    if (psVar1 == NULL) {
        _1C = 2;
        return;
    }
    
    while (_04 >= psVar1[_14]._2) {
        _04 -= psVar1[_14]._2;
        _08 = _10;
        _14++;
        _0C = _08;
        const s16* ps = &psVar1[_14]._0;
        switch(ps[0]) {
        case 0xf:
            _1C = 0;
            return;
        case 0xe:
            _1C = 2;
            return;
        case 0xd:
            _14 = ps[2];
            break;
        default:
            _18 = ps[0];
            _10 = ps[2] / 32768.0f;
            break;
        }
    }

    updateCurrentValue(psVar1[_14]._2);
}

f32 const JASOscillator::sCurveTableLinear[17] = {
    1.0, 0.9375, 0.875, 0.8125, 0.75, 0.6875, 0.625, 0.5625, 0.5,
    0.4375, 0.375, 0.3125, 0.25, 0.1875, 0.125, 0.0625, 0.0,
};

f32 const JASOscillator::sCurveTableSampleCell[17] = {
    1.0, 0.9704890251159668, 0.7812740206718445, 0.5462809801101685, 0.39979198575019836, 
    0.28931498527526855, 0.21210399270057678, 0.15747599303722382, 0.1126129999756813, 0.08178959786891937, 
    0.057985201478004456, 0.04364150017499924, 0.03082370012998581, 0.023712899535894394, 0.015259300358593464, 
    0.00915555004030466, 0.0
};

f32 const JASOscillator::sCurveTableSqRoot[17] = {
    1.0, 0.8789060115814209, 0.765625, 0.6601560115814209, 0.5625, 0.4726560115814209,
    0.390625, 0.3164060115814209, 0.25, 0.1914059966802597, 0.140625, 0.09765619784593582,
    0.0625, 0.03515620157122612, 0.015625, 0.00390625, 0.0
};

f32 const JASOscillator::sCurveTableSquare[17] = {
    1.0, 0.9682459831237793, 0.9354140162467957, 0.9013879895210266, 0.8660249710083008,
    0.8291559815406799, 0.790569007396698, 0.75, 0.7071070075035095, 0.66143798828125,
    0.6123719811439514, 0.55901700258255, 0.5, 0.433012992143631, 0.35355299711227417, 0.25, 0.0,
};

static f32* table_list[4] = {
    (f32*)JASOscillator::sCurveTableLinear,
    (f32*)JASOscillator::sCurveTableSquare,
    (f32*)JASOscillator::sCurveTableSqRoot,
    (f32*)JASOscillator::sCurveTableSampleCell,
};

void JASOscillator::updateCurrentValue(f32 param_0) {
    f32* table = table_list[_18];
    f32 fVar1 = 16.0f * (_04 / param_0);
    u32 index = (u32) fVar1;
    f32 fVar3 = (fVar1 - index);
    f32 fVar2 = (1.0f - fVar3) * table[index];
    f32 fVar4 = fVar2 + fVar3 * table[index + 1];
    _08 = _0C * fVar4 + _10 * (1.0f - fVar4);
}
