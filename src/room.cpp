/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "room.hpp"
#include "colors.hpp"

Room::Room(uint64_t id, std::string_view title, std::string_view description)
  : id(id), title(title), description(description)
{
}

constexpr Room::base_room_info Room::base_rooms[] =
{
  {' ',COLOR_NORMAL,"The void","All around is the void."},
  {'^',COLOR_BRIGHT_GREEN,"Hills","All around is the airy majesty of the beautiful hills."},
  {'+',COLOR_BRIGHT_GREEN,"Forest","All around is an imposing forest, its thick trees reaching into the sky."},
  {'!',COLOR_RED,"Tree tops","All around is a mass of branches and leaves."},
  {'^',COLOR_BRIGHT_BLACK,"Mountains","All around is the airy majesty of the mountain range."},
  {'&',COLOR_GREEN,"Marsh","All around is a dank, reed-filled marsh."},
  {'%',COLOR_GREEN,"A Jungle","All around is a thick, wild jungle, the trees dripping with moisture"},
  {'!',COLOR_RED,"The jungle canopy","All around is a leafy jungle canopy."},
  {'l',COLOR_BRIGHT_GREEN,"A field","All around is a grass-filled field, dotted with vegetation."},
  {'%',COLOR_BRIGHT_YELLOW,"A beach","All around is a sandy beach, speckled with shells."},
  {'&',COLOR_BRIGHT_YELLOW,"A desert","All around is a desert sand dune, weeping away with pure white-yellow sands."},
  {'I',COLOR_BRIGHT_WHITE,"An ice plain","All around is a snow-bound ice plain."},
  {'!',COLOR_RED,"The water's surface","All around is open water, awash with waves."},
  {'!',COLOR_RED,"Water just below the surface","All around is water, swirling with the wake from waves moving above you."},
  {'!',COLOR_RED,"Underwater","All around is water, swirling with hidden currents."},
  {'~',COLOR_BRIGHT_BLUE,"The sea bed","All around is the watery and cold sea bed."},
  {'!',COLOR_RED,"Mid air","All around is open air, with misty clouds moving slowly around you."},
  {'!',COLOR_RED,"Under ground","All around is solid earth and rock, pressing in."},
  {':',COLOR_YELLOW,"trail","You are on a trail running"},
  {':',COLOR_YELLOW,"track","You are on a track running"},
  {':',COLOR_BRIGHT_WHITE,"road","You are on a road running"},
  {'#',COLOR_BRIGHT_CYAN,"A structure","All around is structure."},
  {'?',COLOR_BRIGHT_CYAN,"Dungeon entrance","All around is dungeon."}
};
