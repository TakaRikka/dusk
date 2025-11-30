/**
 * d_a_tag_smk_emt.cpp
 * Tag - Smoke Emit
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_smk_emt.h"
#include "d/d_procname.h"

daTagSmkEmt_c::daTagSmkEmt_c() {
    memset(&mParticles,0,0xc);
}

daTagSmkEmt_c::~daTagSmkEmt_c() {}

int daTagSmkEmt_c::create() {
    fopAcM_ct(this, daTagSmkEmt_c);
    createSmkEmt();
    return cPhs_COMPLEATE_e;
}

int daTagSmkEmt_c::Delete() {
    deleteSmkEmt();
    return 1;
}

int daTagSmkEmt_c::draw() {
    return 1;
}

int daTagSmkEmt_c::execute() {
    return 1;
}

static u16 const PARTICLE_NAME[3] = {
    0x87E7,
    0x87E8,
    0x87E9
};

void daTagSmkEmt_c::createSmkEmt() {
    cXyz pos(current.pos);
    csXyz angle(0,0,0);
    cXyz pos2(1.0f,1.0f,1.0f);

    for (int i = 0; i < 3; i++) {
        mParticles[i] = dComIfGp_particle_set(PARTICLE_NAME[i],&pos,&angle,&pos2);
    }
}

void daTagSmkEmt_c::deleteSmkEmt() {
    for (int i = 0; i < 3; i++) {
        if (mParticles[i]) {
            mParticles[i]->becomeInvalidEmitter();
            mParticles[i] = 0;
        }
    }
}

static int daTagSmkEmt_create(daTagSmkEmt_c* i_this) {
    fopAcM_ct(i_this, daTagSmkEmt_c);
    return i_this->create();
}

static int daTagSmkEmt_Delete(daTagSmkEmt_c* i_this) {
    return i_this->Delete();
}

static int daTagSmkEmt_execute(daTagSmkEmt_c* i_this) {
    return i_this->execute();
}

static int daTagSmkEmt_draw(daTagSmkEmt_c* i_this) {
    return i_this->draw();
}

static actor_method_class daTagSmkEmt_METHODS = {
    (process_method_func)daTagSmkEmt_create,
    (process_method_func)daTagSmkEmt_Delete,
    (process_method_func)daTagSmkEmt_execute,
    (process_method_func)0,
    (process_method_func)daTagSmkEmt_draw
};

extern actor_process_profile_definition g_profile_Tag_SmkEmt = {
    fpcLy_CURRENT_e,         // mLayerID    
    7,                       // mListID  
    fpcPi_CURRENT_e,         // mListPrio   
    PROC_Tag_SmkEmt,         // mProcName                
    &g_fpcLf_Method.base,   // sub_method                      
    sizeof(daTagSmkEmt_c),   // mSize                       
    0,                       // mSizeOther  
    0,                       // mParameters         
    &g_fopAc_Method.base,    // sub_method                     
    743,                     // mPriority       
    &daTagSmkEmt_METHODS,    // sub_method                     
    0x40000,                 // mStatus            
    fopAc_ACTOR_e,           // mActorType  
    fopAc_CULLBOX_CUSTOM_e,  // cullType     
};
