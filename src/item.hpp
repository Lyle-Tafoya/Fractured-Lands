/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#ifndef FRACTURED_LANDS_ITEM_HPP_
#define FRACTURED_LANDS_ITEM_HPP_

#include <bitset>
#include <string>

enum weapon_type : uint8_t
{
  AXE,
  DISTANCE,
  KNIFE,
  MACE,
  SPEAR,
  STAFF,
  SWORD,
  THROWN,
  WHIP
};
struct weapon_component
{
  weapon_type type;
  int damage;
};

enum wearable_type : uint8_t
{
  GLOVES,
  SLEEVES,
  VAMBRACES,
  PANTS,
  GREAVES,
  BOOTS,
  HOOD,
  HELMET,
  SHIRT
};

enum item_material : uint8_t
{
  BONE,
  BRONZE,
  CLOTH,
  DRAGON,
  FLESH,
  GLASS,
  GOLD,
  IRON,
  LEATHER,
  MAGICAL,
  MITHRIL,
  PAPER,
  SILVER,
  STEEL,
  STONE,
  TEMPERED,
  TOOTH,
  WOOD
};

struct wearable_component
{
  wearable_type type;
  material material;
};
enum item_status
{
  BLESSED,
  BROKEN,
  CAN_BREAK,
  CAN_DECAY,
  CAN_OPEN,
  CONTAINER,
  CURSED,
  ENCHANTED,
  HIDDEN,
  IDENTIFIED,
  ILLUSION,
  LIGHT,
  LIT,
  LOCKED,
  MAGIC,
  NO_DROP,
  NO_GET,
  NO_SAVE,
  NO_SELL,
  OPEN,
  PREVENT_INSERT,
  RUNE,
  PROJECTILE,
  UNIQUE,
  WEAR,
  WIELDED,
  WORN
};

class Item
{
public:
  std::bitset<WORN+1> status;
  int32_t weight;
  int64_t value;
  std::string id;
  std::string name;
  std::string description;
  std::string read_text;

  int max_durability;
  int durability;
  int consumable_amount;
  int decay_timestamp;
  int fuel;

};

#endif
