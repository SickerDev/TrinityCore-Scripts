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
	{ 2, 0, GOSSIP_ICON_DOT, 0, "============ Factions ============"},
	{ 2, 0, GOSSIP_ICON_DOT, 1156, "The Ashen Verdict"},
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