#include "drv/hitdrv.h"
#include <string.h>

HitEntry* lbl_80418408; //TODO: proper name

//local prototypes
inline BOOL tempfunc(HitCheckQuery* query, HitVector* vector);
inline BOOL tempfunc2(HitCheckQuery* query, HitVector* vector);
inline BOOL tempfunc3(HitCheckQuery* query, HitVector* vector);
BOOL checkTriVec_xz(HitCheckQuery* query, HitVector* vector);
BOOL chkFilterAttr(HitCheckQuery* query, HitEntry* entry);

void hitInit(void) {
	; //stubbed in retail
}

void hitReInit(void) {
	HitEntry* entry;
	MapEntry* map;
	s32 i, j;

	map = mapGetWork()->entries;
	for (i = 0; i < map->count; i++, map++) {
		entry = map->hitObjects;
		if (entry) {
			for (j = 0; j < map->hitNumJoints; j++) {

			}
		}
	}

}

void hitMain(void) {

}



HitEntry* hitNameToPtr(const char* name) {
	MapEntry* map;
	HitEntry* hit;
	s32 i, j;

	map = mapGetWork()->entries;
	if (!name) {
		return NULL;
	}
	for (i = 0; i < map->count; i++, map++) {
		hit = map->hitObjects;
		for (j = 0; j < map->hitNumJoints; j++, hit++) {
			if (!(hit->flags & 0x80) && !strcmp(hit->joint->name, name)) {
				return hit;
			}
		}
	}
	return NULL;
}





HitEntry* hitEntry(MapJoint* joint, Mtx mtx, BOOL group) {
	MapWork* wp;
	MapEntry* entry;
	u32 size;

	wp = mapGetWork();
	entry = &wp->entries[group];
	entry->hitNumJoints = mapGetJoints(joint);
	size = OSRoundUp32B(sizeof(HitEntry) * (entry->hitNumJoints + 128));
	memset(entry->hitObjects, 0, size);
	//return hitEntrySub(joint, 0, mtx, 1, group);
}








inline BOOL tempfunc(HitCheckQuery* query, HitVector* vector) {
    Vec sp80;
    Vec sp8C;
    Vec sp98;
    f32 dotProduct;
    f32 temp_f2;
    
    PSVECSubtract(&query->targetPos, &vector->unk0, &sp80);
    dotProduct = PSVECDotProduct(&vector->unk48, &sp80);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        if (((sp80.z * vector->unk24.x) - (sp80.x * vector->unk24.z)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp8C);
        if (((sp8C.z * vector->unk30.x) - (sp8C.x * vector->unk30.z)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp98);
        if (((sp98.z * vector->unk3C.x) - (sp98.x * vector->unk3C.z)) < 0.0f) {
            return 0;
        }
    } else {
        if ((vector->unk48.y * dotProduct) <= 0.0f) {
            return 0;
        }
        if ((dotProduct * ((sp80.z * vector->unk24.x) - (sp80.x * vector->unk24.z))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp8C);
        if ((dotProduct * ((sp8C.z * vector->unk30.x) - (sp8C.x * vector->unk30.z))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp98);
        if ((dotProduct * ((sp98.z * vector->unk3C.x) - (sp98.x * vector->unk3C.z))) < 0.0f) {
            return 0;
        }
    }
    temp_f2 = -vector->unk48.y;
    if ((query->targetDistance >= 0.0f) && ((-dotProduct / temp_f2) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / temp_f2;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y - query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->unk48;
    } else {
        PSVECScale(&vector->unk48, &query->hitNormal, -1.0f);
    }
    return 1;
}

inline BOOL tempfunc2(HitCheckQuery* query, HitVector* vector) {
    Vec sp5C;
    Vec sp68;
    Vec sp74;
    f32 dotProduct;
    f32 temp_f2;
    
    PSVECSubtract(&query->targetPos, &vector->unk0, &sp5C);
    dotProduct = PSVECDotProduct(&vector->unk48, &sp5C);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        if (((sp5C.z * vector->unk24.x) - (sp5C.x * vector->unk24.z)) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp68);
        if (((sp68.z * vector->unk30.x) - (sp68.x * vector->unk30.z)) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp74);
        if (((sp74.z * vector->unk3C.x) - (sp74.x * vector->unk3C.z)) > 0.0f) {
            return 0;
        }
    } else {
        if ((vector->unk48.y * dotProduct) >= 0.0f) {
            return 0;
        }
        if ((dotProduct * ((sp5C.z * vector->unk24.x) - (sp5C.x * vector->unk24.z))) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp68);
        if ((dotProduct * ((sp68.z * vector->unk30.x) - (sp68.x * vector->unk30.z))) > 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp74);
        if ((dotProduct * ((sp74.z * vector->unk3C.x) - (sp74.x * vector->unk3C.z))) > 0.0f) {
            return 0;
        }
    }
    temp_f2 = vector->unk48.y;
    if ((query->targetDistance >= 0.0f) && ((-dotProduct / temp_f2) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / temp_f2;
    query->hitPos.x = query->targetPos.x;
    query->hitPos.y = query->targetPos.y + query->targetDistance;
    query->hitPos.z = query->targetPos.z;
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->unk48;
    } else {
        PSVECScale(&vector->unk48, &query->hitNormal, -1.0f);
    }
    return 1;
}

inline BOOL tempfunc3(HitCheckQuery* query, HitVector* vector) {
    Vec sp8;
    Vec sp14;
    Vec sp20;
    Vec sp2C;
    Vec sp38;
    Vec sp44;
    Vec sp50;
    f32 dotProduct;
    f32 var_f27;
    
    PSVECSubtract(&query->targetPos, &vector->unk0, &sp8);
    dotProduct = PSVECDotProduct(&vector->unk48, &sp8);
    if (query->singleSided) {
        if (dotProduct < 0.0f) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->unk24, &sp2C);
        if (PSVECDotProduct(&sp8, &sp2C) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp14);
        PSVECCrossProduct(&query->targetDir, &vector->unk30, &sp38);
        if (PSVECDotProduct(&sp14, &sp38) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp20);
        PSVECCrossProduct(&query->targetDir, &vector->unk3C, &sp44);
        if (PSVECDotProduct(&sp20, &sp44) < 0.0f) {
            return 0;
        }
        var_f27 = PSVECDotProduct(&vector->unk48, &query->targetDir);
    } else {
        var_f27 = PSVECDotProduct(&vector->unk48, &query->targetDir);
        if ((var_f27 * dotProduct) >= 0.0f) {
            return 0;
        }
        PSVECCrossProduct(&query->targetDir, &vector->unk24, &sp2C);
        if ((dotProduct * PSVECDotProduct(&sp8, &sp2C)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp14);
        PSVECCrossProduct(&query->targetDir, &vector->unk30, &sp38);
        if ((dotProduct * PSVECDotProduct(&sp14, &sp38)) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp20);
        PSVECCrossProduct(&query->targetDir, &vector->unk3C, &sp44);
        if ((dotProduct * PSVECDotProduct(&sp20, &sp44)) < 0.0f) {
            return 0;
        }
    }
    
    if ((query->targetDistance >= 0.0f) && ((-dotProduct / var_f27) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -dotProduct / var_f27;
    PSVECScale(&query->targetDir, &sp50, query->targetDistance);
    PSVECAdd(&query->targetPos, &sp50, &query->hitPos);
    if (dotProduct >= 0.0f) {
        query->hitNormal = vector->unk48;
    } else {
        PSVECScale(&vector->unk48, &query->hitNormal, -1.0f);
    }
    return 1;
}

HitEntry* hitCheckVecFilter(HitCheckQuery* query, HitFilterCallback callback) { //1:1
    f32 temp;
    HitVector* vector;
    Vec position;
    Vec spBC;
    Vec normal;
    Vec spA4;
    HitEntry* var_r29;
    HitEntry* var_r28;
    int i;
    f32 distance;
    f32 var_f29;
    HitVector* var_r31;
    f32 temp_f28;

    mapGetWork();
    var_r28 = NULL;
    if (!PSVECSquareMag(&query->targetDir)) {
        return NULL;
    }
    var_f29 = -1.0f;
    temp = 0.5f;
    temp_f28 = query->targetDistance * temp;
    PSVECScale(&query->targetDir, &spA4, temp_f28);
    PSVECAdd(&query->targetPos, &spA4, &spA4);
    var_r29 = lbl_80418408;
    if (query->targetDir.y == 0.0f) {
        while (var_r29 != NULL) {
            if (callback == NULL || callback(query, var_r29)) {
                if (!(PSVECDistance(&spA4, &var_r29->unkC0) > temp_f28 + var_r29->unkCC)) {
                    if (var_r29->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    var_r31 = var_r29->unkAC;
                    for (i = 0; i < var_r29->unkA8; i++, var_r31++) {
                        if ((query->singleSided == 0) || !(PSVECDotProduct(&var_r31->unk48, &query->targetDir) >= 0.0f)) {
                            if (checkTriVec_xz(query, var_r31) && (var_f29 < 0.0f || var_f29 > query->targetDistance)) {
                                var_f29 = query->targetDistance;
                                var_r28 = var_r29;
                                distance = var_f29;
                                position = query->hitPos;
                                spBC = query->hitNormal;
                            }
                        }
                    }
                }
            }
            var_r29 = var_r29->next;
        }
    } else if (query->targetDir.x == 0.0f && query->targetDir.z == 0.0f) {
        if (query->targetDir.y == -1.0f) {
            while (var_r29 != NULL) {
                if (callback == NULL || callback(query, var_r29)) {
                    if (!(PSVECDistance(&spA4, &var_r29->unkC0) > temp_f28 + var_r29->unkCC)) {
                        if (var_r29->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = var_r29->unkAC;
                        for (i = 0; i < var_r29->unkA8; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->unk48.y <= 0.0f)) {
                                if (tempfunc(query, vector) && (var_f29 < 0.0f || var_f29 > query->targetDistance)) {
                                    var_f29 = query->targetDistance;
                                    var_r28 = var_r29;
                                    distance = var_f29;
                                    position = query->hitPos;
                                    spBC = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                var_r29 = var_r29->next;
            }
        } else if (query->targetDir.y == 1.0f) {
            while (var_r29 != NULL) {
                if (callback == NULL || callback(query, var_r29)) {
                    if (!(PSVECDistance(&spA4, &var_r29->unkC0) > temp_f28 + var_r29->unkCC)) {
                        if (var_r29->joint->drawMode->cullMode == 1) {
                            query->singleSided = 1;
                        } else {
                            query->singleSided = 0;
                        }
                        vector = var_r29->unkAC;
                        for (i = 0; i < var_r29->unkA8; i++, vector++) {
                            if (query->singleSided == 0 || !(vector->unk48.y >= 0.0f)) {
                                if (tempfunc2(query, vector) && (var_f29 < 0.0f || var_f29 > query->targetDistance)) {
                                    var_f29 = query->targetDistance;
                                    var_r28 = var_r29;
                                    distance = var_f29;
                                    position = query->hitPos;
                                    spBC = query->hitNormal;
                                }
                            }
                        }
                    }
                }
                var_r29 = var_r29->next;
            }
        } else {
            goto label_235;
        }
    } else {
label_235:
        while (var_r29 != NULL) {
            if (callback == NULL || callback(query, var_r29)) {
                if (!(PSVECDistance(&spA4, &var_r29->unkC0) > (temp_f28 + var_r29->unkCC))) {
                    if (var_r29->joint->drawMode->cullMode == 1) {
                        query->singleSided = 1;
                    } else {
                        query->singleSided = 0;
                    }
                    vector = var_r29->unkAC;
                    for (i = 0; i < var_r29->unkA8; i++, vector++) {
                        if (query->singleSided == 0 || !(PSVECDotProduct(&vector->unk48, &query->targetDir) >= 0.0f)) {
                            if (tempfunc3(query, vector) && (var_f29 < 0.0f || var_f29 > query->targetDistance)) {
                                var_f29 = query->targetDistance;
                                var_r28 = var_r29;
                                distance = var_f29;
                                position = query->hitPos;
                                spBC = query->hitNormal;
                            }
                        }
                    }
                }
            }
            var_r29 = var_r29->next;
        }
    }
    if (var_r28 == NULL) {
        return NULL;
    }
    PSVECNormalize(&spBC, &normal);
    query->targetDistance = distance;
    query->hitPos = position;
    query->hitNormal = normal;
    return var_r28;
}

HitEntry* hitCheckFilter(HitFilterCallback callback, f32* hitX, f32* hitY, f32* hitZ, f32* distance,
						f32* hitNX, f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) { //1:1
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry *entry;
    
    query->targetPos = (Vec){x, y, z};
    query->targetDir = (Vec){nx, ny, nz};
    query->targetDistance = *distance;
    entry = hitCheckVecFilter(query, callback);
    if (entry == NULL) {
        return NULL;
    }
    *distance = query->targetDistance;
    *hitX = query->hitPos.x;
    *hitY = query->hitPos.y;
    *hitZ = query->hitPos.z;
    *hitNX = query->hitNormal.x;
    *hitNY = query->hitNormal.y;
    *hitNZ = query->hitNormal.z;
    return entry;
}

BOOL checkTriVec_xz(HitCheckQuery* query, HitVector* vector) { //1:1
    Vec sp20;
    Vec sp14;
    Vec sp8;
    f32 temp_f2;
    f32 temp_f31;

    PSVECSubtract(&query->targetPos, &vector->unk0, &sp20);
    temp_f31 = PSVECDotProduct(&vector->unk48, &sp20);
    if (query->singleSided != 0) {
        if (temp_f31 < 0.0f) {
            return 0;
        }
        if (((sp20.z * (vector->unk24.y * query->targetDir.x)) + ((sp20.x * (-vector->unk24.y * query->targetDir.z))
            + (sp20.y * ((vector->unk24.x * query->targetDir.z) - (vector->unk24.z * query->targetDir.x))))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp14);
        if (((sp14.z * (vector->unk30.y * query->targetDir.x)) + ((sp14.x * (-vector->unk30.y * query->targetDir.z))
            + (sp14.y * ((vector->unk30.x * query->targetDir.z) - (vector->unk30.z * query->targetDir.x))))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp8);
        if (((sp8.z * (vector->unk3C.y * query->targetDir.x)) + ((sp8.x * (-vector->unk3C.y * query->targetDir.z))
            + (sp8.y * ((vector->unk3C.x * query->targetDir.z) - (vector->unk3C.z * query->targetDir.x))))) < 0.0f) {
            return 0;
        }
    } else {
        if ((temp_f31 * ((vector->unk48.x * query->targetDir.x) + (vector->unk48.z * query->targetDir.z))) >= 0.0f) {
            return 0;
        }
        if ((temp_f31 * ((sp20.z * (vector->unk24.y * query->targetDir.x)) + ((sp20.x * (-vector->unk24.y * query->targetDir.z))
            + (sp20.y * ((vector->unk24.x * query->targetDir.z) - (vector->unk24.z * query->targetDir.x)))))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unkC, &sp14);
        if ((temp_f31 * ((sp14.z * (vector->unk30.y * query->targetDir.x)) + ((sp14.x * (-vector->unk30.y * query->targetDir.z))
            + (sp14.y * ((vector->unk30.x * query->targetDir.z) - (vector->unk30.z * query->targetDir.x)))))) < 0.0f) {
            return 0;
        }
        PSVECSubtract(&query->targetPos, &vector->unk18, &sp8);
        if ((temp_f31 * ((sp8.z * (vector->unk3C.y * query->targetDir.x)) + ((sp8.x * (-vector->unk3C.y * query->targetDir.z))
            + (sp8.y * ((vector->unk3C.x * query->targetDir.z) - (vector->unk3C.z * query->targetDir.x)))))) < 0.0f) {
            return 0;
        }
    }
    temp_f2 = (vector->unk48.x * query->targetDir.x) + (vector->unk48.z * query->targetDir.z);
    if ((query->targetDistance >= 0.0f) && ((-temp_f31 / temp_f2) >= query->targetDistance)) {
        return 0;
    }
    query->targetDistance = -temp_f31 / temp_f2;
    query->hitPos.x = (query->targetDir.x * query->targetDistance) + query->targetPos.x;
    query->hitPos.y = query->targetPos.y;
    query->hitPos.z = (query->targetDir.z * query->targetDistance) + query->targetPos.z;
    if (temp_f31 >= 0.0f) {
        query->hitNormal = vector->unk48;
    } else {
        PSVECScale(&vector->unk48, &query->hitNormal, -1.0f);
    }
    return 1;
}

BOOL hitCheckVecHitObjXZ(HitCheckQuery* query, HitEntry* entry) { //1:1
    int i;
    HitVector* vector;

    if (entry->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = entry->unkAC;
    for (i = 0; i < entry->unkA8; i++, vector++) {
        if ((query->singleSided == 0 || !(PSVECDotProduct(&vector->unk48, &query->targetDir) >= 0.0f)) && checkTriVec_xz(query, vector)) {
            return 1;
        }
    }
    return 0;
}

BOOL chkFilterAttr(HitCheckQuery* query, HitEntry* entry) { //1:1
    return (entry->unk4 & query->user0) == 0;
}

HitEntry* hitCheckAttr(s32 user0, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX,
					  f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz) { //1:1
    HitCheckQuery temp;
    HitCheckQuery* query = &temp;
    HitEntry* entry;


    query->targetPos = (Vec){x, y, z};
    query->targetDir = (Vec){nx, ny, nz};
    query->targetDistance = *distance;
    query->user0 = user0;
    entry = hitCheckVecFilter(query, chkFilterAttr);
    if (entry == NULL) {
        return NULL;
    }
    *distance = query->targetDistance;
    *hitX = query->hitPos.x;
    *hitY = query->hitPos.y;
    *hitZ = query->hitPos.z;
    *hitNX = query->hitNormal.x;
    *hitNY = query->hitNormal.y;
    *hitNZ = query->hitNormal.z;
    return entry;
}

HitEntry* hitCheckSphereFilter(HitFilterCallback callback, f32 x, f32 y, f32 z, f32 distance) { //almost 1:1, stubborn regalloc
    MapWork* wp = mapGetWork();
    MapEntry* var_r31 = wp->entries;
    MapEntry* map;
    Vec sp68;
    int j;
    HitEntry* temp_r27;
    HitEntry* var_r23;
    HitVector* vector;
    f32 var_f28;
    f32 temp_f1;
    f32 var_f30;
    f32 var_f31;
    s32 entryId;
    s32 objectId;
    s32 var_r29;
    HitCheckQuery temp;
    HitCheckQuery* query;
    int i;

    var_f28 = -1.0f;
    objectId = -1;
    query = &temp;
    sp68 = (Vec){x, y, z};
    for (i = 0; i < var_r31->count; var_r31++, i++) {
        var_r23 = var_r31->hitObjects;
        for (j = 0; j < var_r31->hitNumJoints; j++, var_r23++) {
            if ((var_r23->unk4 & 0x80000000) && (var_r23->joint->partCount != 0)) {
                var_r29 = var_r23->flags;
                if (!(var_r29 & 1) && !(var_r29 & 0x80)) {
                    temp_f1 = PSVECDistance(&sp68, &var_r23->unkC0);
                    if (!(temp_f1 > (distance + var_r23->unkCC))) {
                        if (callback == NULL || callback(query, var_r23)) {
                            if (var_f28 < 0.0f || var_f28 > temp_f1) {
                                var_f28 = temp_f1;
                                var_f31 = var_r23->unkCC;
                                objectId = j;
                                entryId = i;
                            }
                        }
                    }
                }
            }
        }
    }
    if (objectId < 0) {
        return 0;
    }
    if (var_f28 < (0.5f * var_f31)) {
        return &wp->entries[entryId].hitObjects[objectId];
    }
    map = &wp->entries[entryId];
    temp_r27 = &map->hitObjects[objectId];
    PSVECSubtract(&temp_r27->unkC0, &sp68, &query->targetDir);
    PSVECNormalize(&query->targetDir, &query->targetDir);
    var_f30 = -1.0f;
    var_r29 = -1;
    query->targetPos = sp68;
    query->targetDistance = distance;
    if (temp_r27->joint->drawMode->cullMode == 1) {
        query->singleSided = 1;
    } else {
        query->singleSided = 0;
    }
    vector = temp_r27->unkAC;
    for (i = 0; i < temp_r27->unkA8; i++, vector++) {
        if ((query->singleSided == 0) || !(PSVECDotProduct(&vector->unk48, &query->targetDir) >= 0.0f)) {
            if (tempfunc3(query, vector) && ((var_f30 < 0.0f) || (var_f30 > query->targetDistance))) {
                var_f30 = query->targetDistance;
                var_r29 = 1;
            }
        }
    }
    if (var_f30 > distance) {
        return 0;
    }
    if (var_r29 < 0) {
        return 0;
    }
    return &map->hitObjects[objectId];
}

