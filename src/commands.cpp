/*
Fractured Lands copyright (C) by Lyle Tafoya, 2022-2023

This file is part of Fractured Lands.

Fractured Lands is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Fractured Lands is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Fractured Lands. If not, see <https://www.gnu.org/licenses/>. 
*/
#include "creature.hpp"
#include "player.hpp"
#include <string_view>
#include <functional>

std::unordered_map<std::string, std::function<void(Creature &, std::string_view)>> commandMap
({
    // Information Commands
    { "brief", Player::briefCommand },
    { "l", Player::lookCommand },
    { "lo", Player::lookCommand },
    { "loo", Player::lookCommand },
    { "look", Player::lookCommand },
    { "prompt", Player::promptCommand },
    { "sc", Player::scoreCommand },
    { "sco", Player::scoreCommand },
    { "scor", Player::scoreCommand },
    { "score", Player::scoreCommand },
    { "who", Player::whoCommand },
    // Map commands
    { "areamap", Player::areamapCommand },
    { "minimap", Player::minimapCommand },
    { "worldmap", Player::worldmapCommand },
    // Movement commands
    { "n", Creature::northCommand },
    { "e", Creature::eastCommand },
    { "s", Creature::southCommand },
    { "w", Creature::westCommand },
    { "ne", Creature::northeastCommand },
    { "se", Creature::southeastCommand },
    { "sw", Creature::southwestCommand },
    { "nw", Creature::northwestCommand },
    { "u", Creature::upCommand },
    { "d", Creature::downCommand },
    { "cfn", Creature::cliffFaceNorthCommand },
    { "cfe", Creature::cliffFaceEastCommand },
    { "cfs", Creature::cliffFaceSouthCommand },
    { "cfw", Creature::cliffFaceWestCommand },
    { "cdn", Creature::cliffDropNorthCommand },
    { "cde", Creature::cliffDropEastCommand },
    { "cds", Creature::cliffDropSouthCommand },
    { "cdw", Creature::cliffDropWestCommand },
    // Chat commands
    { ":", Creature::emoteCommand },
    { "emote", Creature::emoteCommand },
    { "ooc", Creature::oocCommand },
    { "'", Creature::sayCommand },
    { "say", Creature::sayCommand },
    { "shout", Creature::shoutCommand },
    { "tell", Creature::tellCommand },
    // Socials
    { "bow", Creature::bowCommand },
    { "nod", Creature::nodCommand },
    // Combat commands
    { "action", Creature::actionCommand },
    { "reaction", Creature::reactionCommand },
    // Utility commands
    { "save", Player::saveCommand },
    { "quit", Player::quitCommand }
});
