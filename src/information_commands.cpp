#include "creature.hpp"
#include <list>

extern const std::string_view action_string(Creature::combat_action a);

constexpr std::string_view Creature::dexterity_descriptions[] =
{
  "slow and uncoodinated",
  "somewhat clumsy",
  "quite dexterous",
  "quick and nimble",
  "extremely agile",
  "blindingly fast"
};
constexpr std::string_view Creature::power_descriptions[] =
{
  "out of touch with the universe",
  "unlucky",
  "lucky",
  "in touch with the universe",
  "able to feel the power of the universe flowing through you",
  "as one with the power of the universe"
};
constexpr std::string_view Creature::intelligence_descriptions[] =
{
  "unable to solve difficult problems",
  "a little slow at learning",
  "a quick learner",
  "mentally agile",
  "unable to understand why most people are so stupid",
  "a genius"
};
constexpr std::string_view Creature::sanity_descriptions[] =
{
  "sure that everyone except you is insane",
  "unable to focus with mental clarity",
  "well-adjusted",
  "clear-headed",
  "lucid",
  "extremely sound of mind"
};
constexpr std::string_view Creature::chutzpah_descriptions[] =
{
  "bashful and uncommunicative",
  "feel shy around strangers",
  "quite persuasive",
  "bold",
  "daring and dashing",
  "fearless and audacious"
};
constexpr std::string_view Creature::wisdom_descriptions[] =
{
  "make terrible decisions",
  "tend to make poor judgements",
  "a good judge of character",
  "make sound judgements",
  "shrewd",
  "sagacious"
};
constexpr std::string_view Creature::strength_descriptions[] =
{
  "feeble",
  "weakly",
  "somewhat weakly",
  "of average strength",
  "reasonably strong",
  "quite muscular",
  "very muscular",
  "extraordinarily strong",
  "phenomenally strong"
};
constexpr std::string_view Creature::size_descriptions[] =
{
  "tiny",
  "small",
  "short",
  "of average size",
  "large",
  "huge",
  "XXX"
};
constexpr std::string_view Creature::constitution_descriptions[] =
{
  "skeletal",
  "sickly",
  "unhealthy",
  "in average health",
  "healthy",
  "robust"
};

size_t attr_to_index(uint16_t attr)
{
  if(attr < 6)
  {
    return 0;
  }
  else if(attr < 9)
  {
    return 1;
  }
  else if(attr < 14)
  {
    return 2;
  }
  else if(attr < 18)
  {
    return 3;
  }
  else if(attr < 23)
  {
    return 4;
  }
  else if(attr < 30)
  {
    return 5;
  }
  else
  {
    return 6;
  }
}
size_t strength_to_index(uint16_t strength)
{
  if(strength < 5)
  {
    return 0;
  }
  else if(strength < 8)
  {
    return 1;
  }
  else if(strength < 12)
  {
    return 2;
  }
  else if(strength < 15)
  {
    return 3;
  }
  else if(strength < 18)
  {
    return 4;
  }
  else if(strength < 22)
  {
    return 5;
  }
  else if(strength < 28)
  {
    return 6;
  }
  else if(strength < 38)
  {
    return 7;
  }
  else
  {
    return 8;
  }
}
size_t size_to_index(uint16_t size)
{
  if(size < 5)
  {
    return 0;
  }
  else if(size < 8)
  {
    return 1;
  }
  else if(size < 11)
  {
    return 2;
  }
  else if(size < 14)
  {
    return 3;
  }
  else if(size < 18)
  {
    return 4;
  }
  else if(size < 21)
  {
    return 5;
  }
  else
  {
    return 6;
  }
}
size_t constitution_to_index(uint16_t constitution)
{
  if(constitution < 4)
  {
    return 0;
  }
  else if(constitution < 7)
  {
    return 1;
  }
  else if(constitution < 10)
  {
    return 2;
  }
  else if(constitution < 15)
  {
    return 3;
  }
  else if(constitution < 20)
  {
    return 4;
  }
  else
  {
    return 5;
  }
}

void Player::scoreCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  std::string output;
  if(creature.soul->verbose)
  {
    std::string attribute_description = "You are ";
    attribute_description += Creature::strength_descriptions[strength_to_index(creature.strength)];
    attribute_description += ", ";
    attribute_description += Creature::size_descriptions[size_to_index(creature.size)];
    attribute_description += ", and ";
    attribute_description += Creature::constitution_descriptions[constitution_to_index(creature.constitution)];
    attribute_description += ".\r\n";
    attribute_description += "Also you are ";
    attribute_description += Creature::dexterity_descriptions[attr_to_index(creature.dexterity)];
    attribute_description += ", ";
    attribute_description += Creature::power_descriptions[attr_to_index(creature.power)];
    attribute_description += ", ";
    attribute_description += Creature::intelligence_descriptions[attr_to_index(creature.intelligence)];
    attribute_description += ", ";
    attribute_description += Creature::wisdom_descriptions[attr_to_index(creature.wisdom)];
    attribute_description += ", ";
    attribute_description += Creature::sanity_descriptions[attr_to_index(creature.sanity)];
    attribute_description += ", and ";
    attribute_description += Creature::chutzpah_descriptions[attr_to_index(creature.chutzpah)];
    attribute_description += ".";

    output = "|g----------------------------------------------------------------------------\r\n"
             "|nYou are |G" + creature.soul->getName() + "|n.\r\n"
             "You have |G" + std::to_string(creature.hp) + "|n hit points (of |g" + std::to_string(creature.max_hp) + "|n) and |G" + std::to_string(creature.sp) + "|n spell points (of |g" + std::to_string(creature.max_sp) + "|n).\r\n"
             "You are a godless male.\r\n"
             "Action: |r" + std::string(action_string(creature.action)) + " |nReaction: |g" + std::string(action_string(creature.reaction)) + "|n\r\n\r\n" +
             attribute_description + "\r\n\r\n"
             "To list your skills, type '|gskills|n'\r\n"
             "You are sober.\r\n"
             "You are really stuffed full of food!\r\n"
             "You can breathe air.\r\n"
             "age:\r\n"
             "|g----------------------------------------------------------------------------|n\r\n";
  }
  else
  {
    output = "|nYou are |G" + creature.soul->getName() + "|n.\r\n"
             "You have |G" + std::to_string(creature.hp) + "|n hit points (of |g" + std::to_string(creature.max_hp) + "|n) and |G" + std::to_string(creature.sp) + "|n spell points (of |g" + std::to_string(creature.max_sp) + "|n).\r\n";
  }
  creature.soul->receiveMessage(output);
}

const std::unordered_map<std::string,std::string> exit_abbreviations =
{
  {"north","n"},
  {"east","e"},
  {"south","s"},
  {"west","w"},
  {"up","u"},
  {"down","d"},
  {"northeast","ne"},
  {"southeast","se"},
  {"southwest","sw"},
  {"northwest","nw"},
  {"cliff-face-north","cfn"},
  {"cliff-face-east","cfe"},
  {"cliff-face-south","cfs"},
  {"cliff-face-west","cfw"}
};

void Player::lookCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  std::string message = "|W";
  message += creature.room->title;
  if(!creature.soul->verbose)
  {
    message += " |M<";
    for(const Room::exit_info &exit : creature.room->exits)
    {
      auto it = exit_abbreviations.find(exit.name);
      if(it == exit_abbreviations.end())
      {
        message += exit.name + " ";
      }
      else
      {
        message += it->second + " ";
      }
    }
    if(!creature.room->exits.empty())
    {
      message.pop_back();
    }
    message += ">|n.\r\n";
  }
  else
  {
    message += "|n\r\n";
    message += creature.room->description;
    message += "\r\n";
    if(creature.room->exits.empty())
    {
      message += "|MThere are no obvious exits.|n\r\n";
    }
    else
    {
      message += "|MThe obvious exits are";
      for(const Room::exit_info &exit : creature.room->exits)
      {
        message += " " + exit.name;
      }
      message += "|n\r\n";
    }
  }
  std::list<const Creature *> visible_creatures;
  for(const Creature *room_creature : creature.room->creatures)
  {
    if(&creature != room_creature)
    {
      visible_creatures.emplace_back(room_creature);
    }
  }
  if(!visible_creatures.empty())
  {
    message += "You see ";
    for(const Creature *visible_creature : visible_creatures)
    {
      if(visible_creature != visible_creatures.back())
      {
        message += "|g" + visible_creature->name + "|n, ";
      }
      else
      {
        message += "|g" + visible_creature->name + "|n";
      }
    }
    message += ".\r\n";
  }
  creature.soul->receiveMessage(message);
}

void Player::briefCommand(Creature &creature, std::string_view)
{
  if(!creature.soul)
  {
    return;
  }
  creature.soul->verbose = !creature.soul->verbose;
  if(creature.soul->verbose)
  {
    creature.soul->receiveMessage("Verbose mode.\r\n");
  }
  else
  {
    creature.soul->receiveMessage("Brief mode.\r\n");
  }
}

void Player::promptCommand(Creature &creature, std::string_view argument)
{
  if(!creature.soul)
  {
    return;
  }
  if(argument.empty())
  {
    creature.soul->receiveMessage("prompt <string>\r\n");
  }
  else
  {
    auto kv = prompt_templates.find(std::string(argument));
    if(kv == prompt_templates.end())
    {
      creature.soul->prompt = argument;
    }
    else
    {
      creature.soul->prompt = kv->second;
    }
  }
}
