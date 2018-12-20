/* ####################################################### *\

	Sicker NPC - Sicker <World Trainer>
	 - Riding
	 - Skills
	
	Version: 1.0
	Status: Compiled and Tested on 335.64

	(C) 2018 Developed by Sicker
	All rights reserved.

\* ####################################################### */

/*

SQL QUERY:
DO NOT CHANGE!
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `unit_class`, `AIName`, `ScriptName`)
VALUES ('190051', '1542', 'Sicker', 'World Trainer', '80', '80', '0', '35', '65', '1.0', '1.8', '0.5', '1', 'SmartAI', 'SICKER_TRAINER_NPC');

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



class SICKER_TRAINER_NPC : public CreatureScript {
public: SICKER_TRAINER_NPC() : CreatureScript("SICKER_TRAINER_NPC") {}
		class SICKER_TRAINER_NPCAI : public ScriptedAI {
		public:  SICKER_TRAINER_NPCAI(Creature* creature) : ScriptedAI(creature) { }

			bool GossipHello(Player * player) override {
				return OnGossipHello(player, me);
			}

			bool OnGossipHello(Player * player, Creature* Sicker) {

				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "======= Training =======", GOSSIP_SENDER_MAIN, 90);
				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface\\Icons\\Spell_Nature_Swiftness:30|t Learn All Riding Skills", GOSSIP_SENDER_MAIN, 1);
				AddGossipItemFor(player, GOSSIP_ICON_CHAT, "|TInterface\\Icons\\Ability_DualWield:30|t Learn all Weapon Skills", GOSSIP_SENDER_MAIN, 2);
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
				if (SICKER_SENDER == GOSSIP_SENDER_MAIN) {
					switch (SICKER_ACTION) {
					case 0:
						player->PlayerTalkClass->SendCloseGossip();
						break;
					case 1:
						player->GetSession()->GetPlayer()->LearnSpell(33388, false);
						player->GetSession()->GetPlayer()->LearnSpell(33391, false);
						player->GetSession()->GetPlayer()->LearnSpell(34090, false);
						player->GetSession()->GetPlayer()->LearnSpell(34091, false);
						player->GetSession()->GetPlayer()->LearnSpell(34093, false);

						player->PlayerTalkClass->SendCloseGossip();
						break;
					case 2:
					case 21:
						switch (player->getClass()) {
						case CLASS_WARRIOR:
							player->LearnSpell(5011, false);
							player->LearnSpell(266, false);
							player->LearnSpell(264, false);
							player->LearnSpell(2567, false);
							player->LearnSpell(5009, false);
							player->LearnSpell(200, false);
							player->LearnSpell(199, false);
							player->LearnSpell(202, false);
							player->LearnSpell(197, false);
							player->LearnSpell(227, false);
							player->LearnSpell(198, false);
							player->LearnSpell(196, false);
							player->LearnSpell(201, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							player->LearnSpell(674, false);
							break;
						case CLASS_PALADIN:
							player->LearnSpell(200, false);
							player->LearnSpell(199, false);
							player->LearnSpell(202, false);
							player->LearnSpell(197, false);
							player->LearnSpell(198, false);
							player->LearnSpell(196, false);
							player->LearnSpell(201, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							break;
						case CLASS_DEATH_KNIGHT:
							player->LearnSpell(200, false);
							player->LearnSpell(199, false);
							player->LearnSpell(202, false);
							player->LearnSpell(197, false);
							player->LearnSpell(198, false);
							player->LearnSpell(196, false);
							player->LearnSpell(201, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							player->LearnSpell(674, false);
							break;
						case CLASS_ROGUE:
							player->LearnSpell(5011, false);
							player->LearnSpell(266, false);
							player->LearnSpell(264, false);
							player->LearnSpell(2567, false);
							player->LearnSpell(198, false);
							player->LearnSpell(196, false);
							player->LearnSpell(201, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							player->LearnSpell(674, false);
							break;
						case CLASS_DRUID:
							player->LearnSpell(200, false);
							player->LearnSpell(199, false);
							player->LearnSpell(197, false);
							player->LearnSpell(198, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							break;
						case CLASS_PRIEST:
							player->LearnSpell(5009, false);
							player->LearnSpell(227, false);
							player->LearnSpell(198, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							break;
						case CLASS_MAGE:
							player->LearnSpell(5009, false);
							player->LearnSpell(197, false);
							player->LearnSpell(201, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							break;
						case CLASS_WARLOCK:
							player->LearnSpell(5009, false);
							player->LearnSpell(197, false);
							player->LearnSpell(201, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							break;
						case CLASS_HUNTER:
							player->LearnSpell(5011, false);
							player->LearnSpell(266, false);
							player->LearnSpell(2567, false);
							player->LearnSpell(200, false);
							player->LearnSpell(202, false);
							player->LearnSpell(197, false);
							player->LearnSpell(227, false);
							player->LearnSpell(196, false);
							player->LearnSpell(201, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(204, false);
							player->LearnSpell(203, false);
							player->LearnSpell(674, false);
							break;
						case CLASS_SHAMAN:
							player->LearnSpell(199, false);
							player->LearnSpell(197, false);
							player->LearnSpell(198, false);
							player->LearnSpell(204, false);
							player->LearnSpell(1180, false);
							player->LearnSpell(227, false);
							player->LearnSpell(203, false);
							player->LearnSpell(196, false);
							player->LearnSpell(674, false);
							break;
						}

						player->UpdateWeaponsSkillsToMaxSkillsForLevel();
						player->PlayerTalkClass->SendCloseGossip();
						break;
					case 90:
						break;
					}
				}
				return true;
			}
		};
		CreatureAI* GetAI(Creature* creature) const override {
			return new SICKER_TRAINER_NPCAI(creature);
		}
};

void AddSC_SICKER_TRAINER_NPC() {
	new SICKER_TRAINER_NPC();
}
