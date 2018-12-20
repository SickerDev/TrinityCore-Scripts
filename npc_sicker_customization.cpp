/* ####################################################### *\

	Sicker NPC - Helper <Customization>
	- Change
		- Race
		- Name
		- Faction
		- Customization

	Version: 0.1 Not ready, idle.

	(C) 2018 Developed by Sicker
	All rights reserved.

\* ####################################################### */


/*

SQL QUERY:
DO NOT CHANGE!
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `unit_class`, `AIName`, `ScriptName`)
VALUES ('190053', '1542', 'Stylist', 'Character Stylist, '80', '80', '0', '35', '65', '1.0', '1.8', '0.5', '1', 'SmartAI', 'SICKER_CUSTOM_NPC');

*/

#include "../../../scripts/ScriptPCH.h"
#include "ScriptMgr.h"
#include "Log.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "GossipDef.h"
#include "Auras\SpellAuras.h"
#include "RBAC.h"
#include "Common.h"
#include "Player.h"
#include "SharedDefines.h"
#include <sstream>
#include <string>

struct SICKERMENU {
	uint32 PARENT;
	uint32 CHILD;
	uint32 ICON;
	uint32 ID;
	string NAME;
};

class SICKER_CUSTOM_NPC : public CreatureScript {
public: SICKER_CUSTOM_NPC() : CreatureScript("SICKER_CUSTOM_NPC") {}
		class SICKER_CUSTOM_NPCAI : public ScriptedAI {
		public:  SICKER_CUSTOM_NPCAI(Creature* creature) : ScriptedAI(creature) { }
		CreatureAI* GetAI(Creature* creature) const override {
			return new SICKER_CUSTOM_NPCAI(creature);
		}
		bool GossipHello(Player * player) override { return OnGossipHello(player, me); }
		bool OnGossipHello(Player * player, Creature* Sicker) {
			player->PlayerTalkClass->SendGossipMenu(907, Sicker->GetGUID());
 			AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cffFF0000|TInterface\\icons\\achievement_reputation_01:30 |t  Nevermind...|r", GOSSIP_SENDER_MAIN, 0);
			return true;
		}
	};

void AddSC_SICKER_CUSTOM_NPC() {
	new SICKER_CUSTOM_NPC();
}