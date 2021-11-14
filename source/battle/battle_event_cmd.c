#include "battle/battle_event_cmd.h"
#include "battle/battle_acrobat.h"
#include "battle/battle_disp.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "system.h"

extern BattleWork* _battleWorkPointer;

//s32* eventId
USERFUNC_DEF(btlevtcmd_WaitEventEnd) {
    s32 id;

    id = evtGetValue(evt, *evt->currCmdArgs);
    return evtCheckID(id) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE; //TODO: fix
}

//s32 retIndex, s32 battleFlags
USERFUNC_DEF(btlevtcmd_check_battleflag) { //TODO: regalloc
    s32* args = evt->currCmdArgs;
    s32 index = args[0];
    if (_battleWorkPointer->mBattleFlags & args[1]) {
        evtSetValue(evt, index, TRUE);
    }
    else {
        evtSetValue(evt, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

//BOOL* onoff, s32 flags
USERFUNC_DEF(btlevtcmd_onoff_battleflag) {
    s32* args = evt->currCmdArgs;
    s32 flags = args[1];
    if (evtGetValue(evt, args[0])) {
        _battleWorkPointer->mBattleFlags |= flags;
    }
    else {
        _battleWorkPointer->mBattleFlags &= ~flags;
    }
    return EVT_RETURN_DONE;
}

//s32 retIndex
USERFUNC_DEF(btlevtcmd_get_turn) {
    evtSetValue(evt, *evt->currCmdArgs, _battleWorkPointer->mTurnCount);
    return EVT_RETURN_DONE;
}

//no params
USERFUNC_DEF(btlevtcmd_reset_turn) {
    _battleWorkPointer->mBattleFlags |= 0x2000;
    return EVT_RETURN_DONE;
}

//no params
USERFUNC_DEF(btlevtcmd_reset_move_color_lv_all) {
    btlsubResetMoveColorLvAll(_battleWorkPointer);
    return EVT_RETURN_DONE;
}

//s32 retIndex, s32 phaseSeqId
USERFUNC_DEF(btlevtcmd_CheckPhase) {
    s32* args = evt->currCmdArgs;
    s32 index = args[0];
    s32 seq = args[1];
    if (seq == BattleGetSeq(_battleWorkPointer, SEQ_PHASE)) {
        evtSetValue(evt, index, TRUE);
    }
    else {
        evtSetValue(evt, index, FALSE);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USERFUNC_DEF(btlevtcmd_GetPos) {
    s32* args = evt->currCmdArgs;
    s32 indexX, indexY, indexZ;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetPos(unit, &x, &y, &z);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexX, (s32)x);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexY, (s32)y);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexZ, (s32)z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USERFUNC_DEF(btlevtcmd_GetPosFloat) {
    s32* args = evt->currCmdArgs;
    s32 indexX, indexY, indexZ;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;


    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetPos(unit, &x, &y, &z);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexX, x);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexY, y);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexZ, z);
    }
    return EVT_RETURN_DONE;
}

//s32* maxValue, s32 retIndex
USERFUNC_DEF(btlevtcmd_GetRandomValue) {
    s32* args = evt->currCmdArgs;
    s32 max, index;

    max = evtGetValue(evt, args[0]);
    index = args[1];
    evtSetValue(evt, index, irand(max));
    return EVT_RETURN_DONE;
}

//s32 retIndex, s32 countIndex, s32 sizeIndex[]
USERFUNC_DEF(btlevtcmd_DrawLots) {
    s32* args = evt->currCmdArgs;
    s32 *alloc, *args2;
    s32 i, index, countId, count, max;
    s32 ceiling, ceilcount, retcount;

    index = args[0];
    countId = args[1];
    args2 = &args[2];
    count = evtGetValue(evt, countId);
    max = 0;
    alloc = BattleAlloc(sizeof(s32) * count);
    for (i = 0; i < count; i++) {
        alloc[i] = evtGetValue(evt, (*args2)++);
        max += alloc[i];
    }
    ceiling = irand(max);
    ceilcount = 0;
    retcount = 0;
    for (i = 0; i < count; i++) {
        ceilcount += alloc[i];
        if (ceiling < ceilcount) {
            break;
        }
        retcount++;
    }
    evtSetValue(evt, index, retcount);
    BattleFree(alloc);
    return EVT_RETURN_DONE;
}

//s32 retIndex for unitId, BattleUnitSetup** setup, s32 spawnFlags
USERFUNC_DEF(btlevtcmd_SpawnUnit) {
    BattleUnitSetup* setup;
    BattleWorkUnit* unit;
    s32* args;
    s32 index;

    args = evt->currCmdArgs;
    index = args[0];
    setup = (BattleUnitSetup*)evtGetValue(evt, args[1]);
    unit = BtlUnit_Spawn(setup, args[2]);
    if (unit) {
        evtSetValue(evt, index, unit->mUnitId);
    }
    else {
        evtSetValue(evt, index, -1);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 flags
USERFUNC_DEF(btlevtcmd_KillUnit) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 id, flags;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    flags = args[1];
    if (!unit) {
        return EVT_RETURN_DONE;
    }
    unit->currentHp = 0;
    BtlUnit_ClearStatus(unit);
    if (flags & 2) {
        unit->mAttributeFlags |= 0x20000;
        return EVT_RETURN_DONE;
    }
    else {
        BtlUnit_SetStatus(unit, kStatusInstantKill, 1, 1);
        if (!(flags & 1)) {
            unit->mFlags |= 0x80000000;
        }
        return EVT_RETURN_DONE;
    }
}

//s32* battleId, s32* copyToPartId, BattleUnitKindPart* kind, s32* copyFromPartId
USERFUNC_DEF(btlevtcmd_ReplaceParts) {
    s32* args = evt->currCmdArgs;
    s32 id, transId, lookupPart, partNum;
    BattleUnitKindPart* kind;
    BattleWorkUnit* unit;
    BattleWorkUnitPart* part;

    id = evtGetValue(evt, args[0]);
    transId = BattleTransID(evt, id);
    lookupPart = evtGetValue(evt, args[1]);
    kind = (BattleUnitKindPart*)args[2];
    partNum = evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, transId);
    part = BattleGetUnitPartsPtr(transId, lookupPart);
    if (kind) {
        while (kind->partNum != partNum) {
            ;
        }
        part->mKindPartParams = kind;
        part->partName = part->mKindPartParams->partName;
        part->attributes = part->mKindPartParams->attributes;
        part->counterAttributes = part->mKindPartParams->counterAttributes;
        part->defense = part->mKindPartParams->defense;
        part->defenseAttr = part->mKindPartParams->defenseAttr;
        part->mPositionOffset = part->mKindPartParams->positionOffset;
        BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
        BtlUnit_SetPartsRotateOffset(part, 0.0f, 0.0f, 0.0f);
        part->mHitBaseOffset.x = part->mKindPartParams->hitBaseOffset.x;
        part->mHitBaseOffset.y = part->mKindPartParams->hitBaseOffset.y;
        part->mHitOffset.x = 0.0f;
        part->mHitOffset.y = 0.0f;
        part->field_0x1A0 = 0;
    }
    btlDispEntAnime(unit);
    return EVT_RETURN_DONE;
}

//s32 retIndex
USERFUNC_DEF(btlevtcmd_ChangeParty) {
    BattleWorkUnit* unit;
    s32 value, index;

    value = -1;
    index = *evt->currCmdArgs;
    unit = BattleChangeParty(_battleWorkPointer);
    if (unit) {
        value = unit->mUnitId;
    }
    evtSetValue(evt, index, value);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z (can be EVTDAT_ADDR_MIN to set current)
USERFUNC_DEF(btlevtcmd_SetPos) {
    f32 x, y, z, currX, currY, currZ;
    s32 indexX, indexY, indexZ, id;
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    indexX = args[1];
    x = (f32)evtGetValue(evt, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(evt, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(evt, indexZ);

    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPos(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_ADDR_MIN) {
        x = currX;
    }
    if (indexY == EVTDAT_ADDR_MIN) {
        y = currY;
    }
    if (indexZ == EVTDAT_ADDR_MIN) {
        z = currZ;
    }
    BtlUnit_SetPos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_AddPos) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddPos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ (can be EVTDAT_ADDR_MIN for noret)
USERFUNC_DEF(btlevtcmd_GetPartsPos) {
    s32* args = evt->currCmdArgs;
    s32 id, partId;
    s32 indexX, indexY, indexZ;
    f32 currX, currY, currZ;
    BattleWorkUnitPart* part;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetPartsWorldPos(part, &currX, &currY, &currZ);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexX, (s32)currX);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexY, (s32)currY);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexZ, (s32)currZ);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetPartsPos) {
    s32* args = evt->currCmdArgs;
    f32 x, y, z, currX, currY, currZ;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    if (part->attributes & 0x10000000) {
        BtlUnit_SetPartsPos(part, x, y, z);
    }
    else {
        BtlUnit_GetPos(unit, &currX, &currY, &currZ);
        BtlUnit_SetPartsOffsetPos(part, x - currX, y - currY, z - currZ);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_AddPartsPos) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    if (part->attributes & 0x10000000) {
        BtlUnit_AddPartsPos(part, x, y, z);
    }
    else {
        BtlUnit_AddPartsOffsetPos(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetDispOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetDispOffset(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetPartsDispOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id); //unused variable
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_AddPartsDispOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id); //unused variable
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsDispOffset(part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetTogeOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    if (x != (f32)EVTDAT_ADDR_MIN) {
        unit->togeOffset.x = x;
    }
    if (y != (f32)EVTDAT_ADDR_MIN) {
        unit->togeOffset.y = y;
    }
    if (z != (f32)EVTDAT_ADDR_MIN) {
        unit->togeOffset.z = z;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 indexX, s32 indexY, s32 indexZ
USERFUNC_DEF(btlevtcmd_GetHomePos) {
    s32* args = evt->currCmdArgs;
    s32 indexX, indexY, indexZ;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetHomePos(unit, &x, &y, &z);
    evtSetValue(evt, indexX, (s32)x);
    evtSetValue(evt, indexY, (s32)y);
    evtSetValue(evt, indexZ, (s32)z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetHomePos) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetHomePos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_AddHomePos) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddHomePos(unit, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetPartsHomePos) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsHomePos(part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, s32 indexX, s32 indexY, s32 indexZ
USERFUNC_DEF(btlevtcmd_GetHitPos) {
    s32* args = evt->currCmdArgs;
    s32 indexX, indexY, indexZ;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetHitPos(unit, part, &x, &y, &z);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexX, (s32)x);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexY, (s32)y);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexZ, (s32)z);
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetHitOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitOffset(unit, part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, f32* x, f32* y, f32* z
USERFUNC_DEF(btlevtcmd_SetHitCursorOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    BattleWorkUnit* unit;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    partId = evtGetValue(evt, args[1]);
    x = (f32)evtGetValue(evt, args[2]);
    y = (f32)evtGetValue(evt, args[3]);
    z = (f32)evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetHitCursorOffset(unit, part, x, y, z);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USERFUNC_DEF(btlevtcmd_GetWidth) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 btlId, id, index, width;

    btlId = evtGetValue(evt, args[0]);
    index = args[1];
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    width = BtlUnit_GetWidth(unit);
    evtSetValue(evt, index, width);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USERFUNC_DEF(btlevtcmd_GetHeight) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 btlId, id, index, height;

    btlId = evtGetValue(evt, args[0]);
    index = args[1];
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    height = BtlUnit_GetHeight(unit);
    evtSetValue(evt, index, height);
    return EVT_RETURN_DONE;
}

//s32* battleId, s16* height
USERFUNC_DEF(btlevtcmd_SetHeight) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 btlId, id;
    s16 height;

    btlId = evtGetValue(evt, args[0]);
    height = (s16)evtGetValue(evt, args[1]);
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetHeight(unit, height); //TODO: make sure extsh
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retIndex
USERFUNC_DEF(btlevtcmd_GetStatusMg) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 btlId, id, index;

    btlId = evtGetValue(evt, args[0]);
    index = args[1];
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetFloat(evt, index, unit->mSizeMultiplier);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_ADDR_MIN to not store)
USERFUNC_DEF(btlevtcmd_SetStatusIconOffset) {
    s32* args = evt->currCmdArgs;
    s32 id, btlId, xpos, ypos;
    BattleWorkUnit* unit;

    btlId = evtGetValue(evt, args[0]);
    xpos = evtGetValue(evt, args[1]);
    ypos = evtGetValue(evt, args[2]);
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (xpos != EVTDAT_ADDR_MIN) {
        unit->mStatusIconOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_ADDR_MIN) {
        unit->mStatusIconOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* xpos, s32* ypos (can be EVTDAT_ADDR_MIN to not store)
USERFUNC_DEF(btlevtcmd_SetHpGaugeOffset) {
    s32* args = evt->currCmdArgs;
    s32 id, btlId, xpos, ypos;
    BattleWorkUnit* unit;

    btlId = evtGetValue(evt, args[0]);
    xpos = evtGetValue(evt, args[1]);
    ypos = evtGetValue(evt, args[2]);
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (xpos != EVTDAT_ADDR_MIN) {
        unit->mHpGaugeOffset[0] = (s16)xpos;
    }
    if (ypos != EVTDAT_ADDR_MIN) {
        unit->mHpGaugeOffset[1] = (s16)ypos;
    }
    return EVT_RETURN_DONE;
}

//s32* battleId, s32 retX, s32 retY
USERFUNC_DEF(btlevtcmd_GetHpGaugeOffset) {
    s32* args = evt->currCmdArgs;
    s32 id, btlId, indexX, indexY;
    BattleWorkUnit* unit;

    btlId = evtGetValue(evt, args[0]);
    indexX = args[1];
    indexY = args[2];
    id = BattleTransID(evt, btlId);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    evtSetValue(evt, indexX, unit->mHpGaugeOffset[0]);
    evtSetValue(evt, indexY, unit->mHpGaugeOffset[1]);
    return EVT_RETURN_DONE;
}

//s32* battleId, s32* partId, u8* alpha
USERFUNC_DEF(btlevtcmd_SetAlpha) {
    s32* args = evt->currCmdArgs;
    s32 id, btlId, partId;
    u8 alpha;

    args = evt->currCmdArgs;
    btlId = evtGetValue(evt, args[0]);
    partId = evtGetValue(evt, args[1]);
    alpha = (u8)evtGetValue(evt, args[2]);
    id = BattleTransID(evt, btlId);
    BattleGetUnitPartsPtr(id, partId)->color.a = alpha;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetRGB) {
    s32* args = evt->currCmdArgs;
    s32 indexR, indexG, indexB;
    BattleWorkUnitPart* part;
    s32 id, btlId, partId;

    btlId = evtGetValue(evt, args[0]);
    partId = evtGetValue(evt, args[1]);
    id = BattleTransID(evt, btlId);
    part = BattleGetUnitPartsPtr(id, partId);
    indexR = args[2];
    indexG = args[3];
    indexB = args[4];
    evtSetValue(evt, indexR, part->color.r);
    evtSetValue(evt, indexG, part->color.g);
    evtSetValue(evt, indexB, part->color.b);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetRGB) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, btlId, partId;
    u8 r, g, b;

    btlId = evtGetValue(evt, args[0]);
    partId = evtGetValue(evt, args[1]);
    r = (u8)evtGetValue(evt, args[2]);
    g = (u8)evtGetValue(evt, args[3]);
    b = (u8)evtGetValue(evt, args[4]);
    id = BattleTransID(evt, btlId);
    part = BattleGetUnitPartsPtr(id, partId);
    part->color.r = r;
    part->color.g = g;
    part->color.b = b;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetRotate) {
    s32* args = evt->currCmdArgs;
    f32 rotateX, rotateY, rotateZ;
    s32 indexX, indexY, indexZ;
    BattleWorkUnit* unit;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetRotate(unit, &rotateX, &rotateY, &rotateZ);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexX, (s32)rotateX);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexY, (s32)rotateY);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, indexZ, (s32)rotateZ);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetRotate) {
    s32* args = evt->currCmdArgs;
    s32 id, indexX, indexY, indexZ;
    f32 x, y, z, currX, currY, currZ;
    BattleWorkUnit* unit;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    indexX = args[1];
    x = (f32)evtGetValue(evt, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(evt, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(evt, indexZ);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetRotate(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_ADDR_MIN) {
        x = currX;
    }
    if (indexY == EVTDAT_ADDR_MIN) {
        y = currY;
    }
    if (indexZ == EVTDAT_ADDR_MIN) {
        z = currZ;
    }
    BtlUnit_SetRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_AddRotate) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_AddRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetPartsRotate) {
    s32* args = evt->currCmdArgs;
    s32 id, partId, indexX, indexY, indexZ;
    BattleWorkUnitPart* part;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    part = BattleGetUnitPartsPtr(id, partId);
    indexX = args[2];
    indexY = args[3];
    indexZ = args[4];
    BtlUnit_GetPartsRotate(part, &x, &y, &z);
    evtSetValue(evt, indexX, (s32)x);
    evtSetValue(evt, indexY, (s32)y);
    evtSetValue(evt, indexZ, (s32)z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPartsRotate) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId, x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = evtGetValue(evt, args[2]);
    y = evtGetValue(evt, args[3]);
    z = evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotate(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_AddPartsRotate) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId, x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = evtGetValue(evt, args[2]);
    y = evtGetValue(evt, args[3]);
    z = evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotate(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetBaseRotate) {
    s32* args = evt->currCmdArgs;
    s32 id, indexX, indexY, indexZ;
    f32 x, y, z, currX, currY, currZ;
    BattleWorkUnit* unit;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    indexX = args[1];
    x = (f32)evtGetValue(evt, indexX);
    indexY = args[2];
    y = (f32)evtGetValue(evt, indexY);
    indexZ = args[3];
    z = (f32)evtGetValue(evt, indexZ);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetBaseRotate(unit, &currX, &currY, &currZ);
    if (indexX == EVTDAT_ADDR_MIN) {
        x = currX;
    }
    if (indexY == EVTDAT_ADDR_MIN) {
        y = currY;
    }
    if (indexZ == EVTDAT_ADDR_MIN) {
        z = currZ;
    }
    BtlUnit_SetBaseRotate(unit, x, y, z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPartsBaseRotate) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    f32 currX, currY, currZ;
    s32 id, partId;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = evtGetFloat(evt, args[2]);
    y = evtGetFloat(evt, args[3]);
    z = evtGetFloat(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_GetPartsBaseRotate(part, &currX, &currY, &currZ);
    if (x != (f32)EVTDAT_ADDR_MIN) {
        currX = x;
    }
    if (y != (f32)EVTDAT_ADDR_MIN) {
        currY = y;
    }
    if (z != (f32)EVTDAT_ADDR_MIN) {
        currZ = z;
    }
    BtlUnit_SetPartsBaseRotate(part, currX, currY, currZ);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetRotateOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetRotateOffset(unit, x, y, z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPartsRotateOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId, x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = evtGetValue(evt, args[2]);
    y = evtGetValue(evt, args[3]);
    z = evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_SetPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_AddPartsRotateOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId, x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    partId = evtGetValue(evt, args[1]);
    x = evtGetValue(evt, args[2]);
    y = evtGetValue(evt, args[3]);
    z = evtGetValue(evt, args[4]);
    part = BattleGetUnitPartsPtr(id, partId);
    BtlUnit_AddPartsRotateOffset(part, (f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetRotateOffsetFromCenterOffset) {
    BattleWorkUnit* unit;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, *evt->currCmdArgs));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_SetRotateOffset(unit, unit->offsetCenter.x, unit->offsetCenter.y, unit->offsetCenter.z);
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetCutBaseOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->cutBaseOffset.x = x;
    unit->cutBaseOffset.y = y;
    unit->cutBaseOffset.z = z;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetBintaHitOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->bintaHitOffset.x = x;
    unit->bintaHitOffset.y = y;
    unit->bintaHitOffset.z = z;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetKissHitOffset) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    x = (f32)evtGetValue(evt, args[1]);
    y = (f32)evtGetValue(evt, args[2]);
    z = (f32)evtGetValue(evt, args[3]);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    unit->kissHitOffset.x = x;
    unit->kissHitOffset.y = y;
    unit->kissHitOffset.z = z;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetCutWidth) {
    s32* args = evt->currCmdArgs;
    f32 value;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    value = (f32)evtGetValue(evt, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id)->cutWidth = value;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetCutHeight) {
    s32* args = evt->currCmdArgs;
    f32 value;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    value = (f32)evtGetValue(evt, args[1]);
    BattleGetUnitPtr(_battleWorkPointer, id)->cutHeight = value;
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPossessionItemOffset) {
    s32* args = evt->currCmdArgs;
    s32 id, x, y, z, indexX, indexY, indexZ;
    BattleWorkUnit* unit;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    x = evtGetValue(evt, indexX);
    indexY = args[2];
    y = evtGetValue(evt, indexY);
    indexZ = args[3];
    z = evtGetValue(evt, indexZ);
    if (indexX != EVTDAT_ADDR_MIN) {
        unit->heldItemOffset.x = (f32)x;
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        unit->heldItemOffset.y = (f32)y;
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        unit->heldItemOffset.z = (f32)z;
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetBaseScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id == -1) { //TODO: un-hardcode?
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    x = evtGetFloat(evt, args[1]);
    y = evtGetFloat(evt, args[2]);
    z = evtGetFloat(evt, args[3]);
    if (unit) {
        BtlUnit_SetBaseScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPartsBaseScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        part = BattleGetUnitPartsPtr(id, evtGetValue(evt, args[1]));
        x = evtGetFloat(evt, args[2]);
        y = evtGetFloat(evt, args[3]);
        z = evtGetFloat(evt, args[4]);
        BtlUnit_SetPartsBaseScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetScale) {
    s32* args = evt->currCmdArgs;
    s32 id, indexX, indexY, indexZ;
    BattleWorkUnit* unit;
    f32 x, y, z;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id == -1) {
        return EVT_RETURN_DONE;
    }
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    indexX = args[1];
    indexY = args[2];
    indexZ = args[3];
    BtlUnit_GetScale(unit, &x, &y, &z);
    if (indexX != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexX, x);
    }
    if (indexY != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexY, y);
    }
    if (indexZ != EVTDAT_ADDR_MIN) {
        evtSetFloat(evt, indexZ, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        x = evtGetFloat(evt, args[1]);
        y = evtGetFloat(evt, args[2]);
        z = evtGetFloat(evt, args[3]);
        BtlUnit_SetScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_AddScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    f32 x, y, z;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        x = evtGetFloat(evt, args[1]);
        y = evtGetFloat(evt, args[2]);
        z = evtGetFloat(evt, args[3]);
        BtlUnit_AddScale(unit, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_SetPartsScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    args = evt->currCmdArgs;
    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        partId = evtGetValue(evt, args[1]);
        part = BattleGetUnitPartsPtr(id, partId);
        x = evtGetFloat(evt, args[2]);
        y = evtGetFloat(evt, args[3]);
        z = evtGetFloat(evt, args[4]);
        BtlUnit_SetPartsScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_AddPartsScale) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnitPart* part;
    s32 id, partId;
    f32 x, y, z;

    args = evt->currCmdArgs;
    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        partId = evtGetValue(evt, args[1]);
        part = BattleGetUnitPartsPtr(id, partId);
        x = evtGetFloat(evt, args[2]);
        y = evtGetFloat(evt, args[3]);
        z = evtGetFloat(evt, args[4]);
        BtlUnit_AddPartsScale(part, x, y, z);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetHp) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        evtSetValue(evt, args[1], unit->currentHp);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetFp) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 id, index, value;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) {
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        index = args[1];
        value = BtlUnit_GetFp(unit);
        evtSetValue(evt, index, value);
    }
    return EVT_RETURN_DONE;
}

USERFUNC_DEF(btlevtcmd_GetMaxHp) {
    s32* args = evt->currCmdArgs;
    BattleWorkUnit* unit;
    s32 id;

    id = BattleTransID(evt, evtGetValue(evt, args[0]));
    if (id != -1) { //TODO: un-hardcode?
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        evtSetValue(evt, args[1], unit->maxHp);
    }
    return EVT_RETURN_DONE;
}






































































































//s32* battleId, s32* windowStartFrame, s32* windowEndFrame, s32* endFrame, s32* earlyFrames
USERFUNC_DEF(btlevtcmd_ACRStart) {
    s32* args = evt->currCmdArgs;
    BattleWork* wp = _battleWorkPointer;
    s32 id, windowStartFrame, windowEndFrame, endFrame, earlyFrames;

    if (isFirstCall) {
        id = BattleTransID(evt, evtGetValue(evt, args[0]));
        windowStartFrame = evtGetValue(evt, args[1]);
        windowEndFrame = evtGetValue(evt, args[2]);
        endFrame = evtGetValue(evt, args[3]);
        earlyFrames = evtGetValue(evt, args[4]);
        BattleAcrobatStart(wp, id, windowStartFrame, windowEndFrame, endFrame, earlyFrames);
    }
    return BattleAcrobatMain(wp);
}

//s32 resultRetIndex, s32 frameRetIndex (can be EVTDAT_ADDR_MIN to skip)
USERFUNC_DEF(btlevtcmd_ACRGetResult) {
    s32* args = evt->currCmdArgs;
    s32 index1, index2, frame, result;

    args = evt->currCmdArgs;
    index1 = args[0];
    index2 = args[1];
    BattleAcrobatGetResult(_battleWorkPointer, &result, &frame);
    if (index1 != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, index1, result);
    }
    if (index2 != EVTDAT_ADDR_MIN) {
        evtSetValue(evt, index2, frame);
    }
    return EVT_RETURN_DONE;
}
