/* ####################################################### *\

	Sicker NPC - Ambassador <Player Reputation Modifier>

	Version: 1.0
	Status: Compiled and tested on 335.64

	(C) 2018 Developed by Sicker
	All rights reserved.

\* ####################################################### */

/*

SQL QUERY:
DO NOT CHANGE!
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `unit_class`, `AIName`, `ScriptName`)
VALUES ('190052', '1542', 'Ambassador', 'Player Reputation Modifier', '80', '80', '0', '35', '65', '1.0', '1.8', '0.5', '1', 'SmartAI', 'REPUTATION_NPC');

*/
#include "../../../scripts/ScriptPCH.h"
#include "ScriptMgr.h"
#include "Log.h"
#include "Chat.h"
#include "WorldSession.h"
#include "ScriptedGossip.h"
#include "ReputationMgr.h"
#include "ScriptedCreature.h"
#include "GossipDef.h"
#include "Auras\SpellAuras.h"
#include "RBAC.h"
#include "Common.h"
#include "Player.h"

using namespace std;

struct SICKERREPMENU {
	uint32 PARENT;
	uint32 CHILD;
	uint32 ICON;
	uint32 FACID;
	string NAME;
};
SICKERREPMENU FullForm[] = {
	{ 2, 0, GOSSIP_ICON_BATTLE, 0, "============ Factions ============"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1156, "The Ashen Verdict"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 932, "The Aldor"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 749, "Hydraxian Waterlords"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 936, "City Shattrath"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1090, "Kirin Tor"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 933, "Consortsium"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1073, "The Kalu'ak"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 942, "Cenarion Expedition"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 92, "Gelkis Clan Centaur"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 93, "Magram Clan Centaur"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 576, "Magram Clan Centaur"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1077, "Shattered Sun Offensive"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 21, "Booty Bay"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 970, "Sporeggar"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1119, "Sons of Hoddir"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 910, "Brood of Nozdormu"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 529, "Argent dawn"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 369, "Gadgetzan"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 87, "Bloodsail Buccaneers"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 990, "The Scale of the Sands"},
	{ 2, 0, GOSSIP_ICON_BATTLE, 1031, "Heaven's Guardians Sha'tar"},
};
//SetReputation(FactionEntry const* factionEntry, int32 standing)

class REPUTATION_NPC : public CreatureScript {
public: REPUTATION_NPC() : CreatureScript("REPUTATION_NPC") {}
		class REPUTATION_NPCAI : public ScriptedAI {
		public:  REPUTATION_NPCAI(Creature* creature) : ScriptedAI(creature) { }
			void PrepareMenu(Player* player, Creature* creature, uint32 menuId) {
				player->PlayerTalkClass->ClearMenus();
				for (uint8 i = 0; i < (sizeof(FullForm) / sizeof(*FullForm)); i++) {
					if (FullForm[i].PARENT == menuId)
						AddGossipItemFor(player, FullForm[i].ICON, FullForm[i].NAME, GOSSIP_SENDER_MAIN, i);
				}
				SendGossipMenuFor(player, 907, creature->GetGUID());
			}
			bool GossipHello(Player * player) override { return OnGossipHello(player, me); }

			bool OnGossipHello(Player * player, Creature* Sicker) {

				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "======= Training =======", GOSSIP_SENDER_MAIN, 90);
				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Factions list", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "====================", GOSSIP_SENDER_MAIN, 90);

				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|cffFF0000|TInterface\\icons\\achievement_reputation_01:30 |t  Nevermind...|r", GOSSIP_SENDER_MAIN, 0);
				player->PlayerTalkClass->SendGossipMenu(907, Sicker->GetGUID());
				return true;
			}
			bool GossipSelect(Player* player, uint32, uint32 OptionID) override {
				uint32 UI_Sender = player->PlayerTalkClass->GetGossipOptionSender(OptionID);
				uint32 UI_Action = player->PlayerTalkClass->GetGossipOptionAction(OptionID);
				return OnGossipSelect(player, me, UI_Sender, UI_Action);
			}

			bool OnGossipSelect(Player* player, Creature* Sicker, uint32 SICKER_SENDER, uint32 SICKER_ACTION) {
				player->PlayerTalkClass->ClearMenus();
				switch (SICKER_ACTION) {
				case 0:
					player->PlayerTalkClass->SendCloseGossip();
					return false;
				case GOSSIP_ACTION_INFO_DEF + 2:
					player->PlayerTalkClass->ClearMenus();
					PrepareMenu(player, Sicker, 2);
					return false;
				}


				uint32 menuData = FullForm[SICKER_ACTION].CHILD;

				if (menuData == 0) {
					player->SetReputation(FullForm[SICKER_ACTION].FACID, 42999);
					//player->AddAura(FullForm[SICKER_ACTION].ID, player);
					menuData = FullForm[SICKER_ACTION].PARENT;
				}

				PrepareMenu(player, Sicker, menuData);
				return true;
			}
		};
		CreatureAI* GetAI(Creature* creature) const override {
			return new REPUTATION_NPCAI(creature);
		}
};
// Script not ready for use
void AddSC_REPUTATION_NPC() {
	new REPUTATION_NPC();
}