#pragma once

#include <cstdint>

class fopAc_ac_c;

namespace dusk::mods {

uint8_t item_check(const char* name, uint8_t itemNo, fopAc_ac_c* giver);
uint8_t item_check_tagged(uint32_t giveTag, uint8_t itemNo, fopAc_ac_c* giver);

uint8_t item_check_chest(uint8_t boxNo, uint8_t itemNo, fopAc_ac_c* chest);
uint8_t item_check_boss(uint8_t itemNo, fopAc_ac_c* boss);
uint8_t item_check_freestanding(uint8_t bitNo, uint8_t itemNo, fopAc_ac_c* item);
uint8_t item_check_poe(uint8_t bitNo, uint8_t itemNo, fopAc_ac_c* poe);
uint8_t item_check_shop(uint8_t itemNo, fopAc_ac_c* giver);
uint8_t item_check_bug(uint8_t insectId, uint8_t itemNo, fopAc_ac_c* agitha);
uint8_t item_check_sky_character(uint8_t itemNo, fopAc_ac_c* statue);

uint32_t item_give_tag(const char* name);
uint32_t item_give_tag_chest(uint8_t boxNo);
uint32_t item_give_tag_boss();
uint32_t item_give_tag_freestanding(uint8_t bitNo);
uint32_t item_give_tag_poe(uint8_t bitNo);
uint32_t item_give_tag_shop(uint8_t itemNo);
uint32_t item_give_tag_bug(uint8_t insectId);
uint32_t item_give_tag_sky_character();

void item_check_enqueue(const char* name, uint8_t itemNo);
void item_check_enqueue_poe(uint8_t bitNo, uint8_t itemNo);

void item_granted(uint8_t itemNo, uint32_t giveTag, fopAc_ac_c* giver);

bool item_give_queue_dispatching();
uint32_t item_give_queue_take_tag();

}  // namespace dusk::mods
