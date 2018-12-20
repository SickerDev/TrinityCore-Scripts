/* ####################################################### *\

	Sicker NPC - Helper <Player Tools>
	 - Buffer
	 - Healer
	 - Level Changer

	(C) 2018 Developed by Sicker
	All rights reserved.

\* ####################################################### */

/*

SQL QUERY:
DO NOT CHANGE!
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `unit_class`, `AIName`, `ScriptName`)
VALUES ('190050', '1542', 'Helper', 'Player Tools', '80', '80', '0', '35', '65', '1.0', '1.8', '0.5', '1', 'SmartAI', 'SICKER_HELPER_NPC');

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

using namespace std;

struct SICKERMENU {
	uint32 PARENT;
	uint32 CHILD;
	uint32 ICON;
	uint32 ID;
	string NAME;
};

// { PARENT, CHILD, ICON, ID, TEXT IN MENU }
SICKERMENU FullForm[] = {
	{ 2, 0, GOSSIP_ICON_DOT, 0, "============ Buffs ============"},
	{ 2, 0, GOSSIP_ICON_DOT, 25898, "|TInterface\\Icons\\Spell_Magic_GreaterBlessingofKings:30|t Greater Blessing of Kings"},
	{ 2, 0, GOSSIP_ICON_DOT, 25899, "|TInterface\\Icons\\Spell_Holy_GreaterBlessingofSanctuary:30|t Greater Blessing of Sanctuary"},
	{ 2, 0, GOSSIP_ICON_DOT, 48938, "|TInterface\\Icons\\Spell_Holy_GreaterBlessingofWisdom:30|t Greater Blessing of Wisdom"},
	{ 2, 0, GOSSIP_ICON_DOT, 48934, "|TInterface\\Icons\\Spell_Holy_GreaterBlessingofKings:30|t Greate Blessing of Might"},
	{ 2, 0, GOSSIP_ICON_DOT, 48470, "|TInterface\\Icons\\Spell_Nature_GiftoftheWild:30|t Gift of the Wild"},
	{ 2, 0, GOSSIP_ICON_DOT, 43002, "|TInterface\\Icons\\Spell_Holy_ArcaneIntellect:30|t Arcane Intellect"},
	{ 2, 0, GOSSIP_ICON_DOT, 48162, "|TInterface\\Icons\\Spell_Holy_PrayerOfFortitute:30|t Prayer of Fortitude"},
	{ 2, 0, GOSSIP_ICON_DOT, 48074, "|TInterface\\Icons\\Spell_Holy_PrayerOfSpirit:30|t Prayer of Spirit"},
	{ 2, 0, GOSSIP_ICON_DOT, 48170, "|TInterface\\Icons\\Spell_Holy_PrayerOfShadowProtection:30|t Prayer of Shadow Protection"},
	{ 2, 0, GOSSIP_ICON_DOT, 47983, "|TInterface\\Icons\\Spell_Fire_FireArmor:30|t Fire Shield"},
	{ 2, 0, GOSSIP_ICON_DOT, 47440, "|TInterface\\Icons\\Ability_Warrior_RallyingCry:30|t Commanding Shout"},
	{ 2, 0, GOSSIP_ICON_DOT, 47436, "|TInterface\\Icons\\Ability_Warrior_BattleShout:30|t Battle Shout"},
	{ 2, 0, GOSSIP_ICON_DOT, 57399, "|TInterface\\Icons\\Spell_Misc_food:30|t Well fed"},
	{ 2, 0, GOSSIP_ICON_DOT, 53755, "|TInterface\\Icons\\INV_Alchemy_Endless_Flask_04:30|t Flask of the Frost Wyrm"},
	{ 2, 0, GOSSIP_ICON_DOT, 53760, "|TInterface\\Icons\\INV_Alchemy_Endless_Flask_06:30|t Flask of Endless rage"},

	{ 3, 0, GOSSIP_ICON_DOT, 26393, "|TInterface\\Icons\\INV_Misc_Gem_Pearl_02:30|t Elune's blessing"},
	{ 3, 0, GOSSIP_ICON_DOT, 23767, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Armor"},
	{ 3, 0, GOSSIP_ICON_DOT, 23737, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Stamina"},
	{ 3, 0, GOSSIP_ICON_DOT, 23738, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Spirit"},
	{ 3, 0, GOSSIP_ICON_DOT, 23736, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Agility"},
	{ 3, 0, GOSSIP_ICON_DOT, 23766, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Intelligence"},
	{ 3, 0, GOSSIP_ICON_DOT, 23735, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Strength"},
	{ 3, 0, GOSSIP_ICON_DOT, 23769, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Resistance"},
	{ 3, 0, GOSSIP_ICON_DOT, 23768, "|TInterface\\Icons\\INV_Misc_Orb_02:30|t Sayge's Dark Fortune of Damage"},
	{ 3, 0, GOSSIP_ICON_DOT, 35912, "|TInterface\\Icons\\Spell_Holy_MagicalSentry:30|t Master Buff (Magic)"},
	{ 3, 0, GOSSIP_ICON_DOT, 36001, "|TInterface\\Icons\\Spell_Holy_ArcaneIntellect:30|t Master Buff (Magic) 2"},
	{ 3, 0, GOSSIP_ICON_DOT, 38734, "|TInterface\\Icons\\Ability_Hunter_Pathfinding:30|t Master Ranged Buff"},
	{ 3, 0, GOSSIP_ICON_DOT, 31305, "|TInterface\\Icons\\Spell_Nature_Polymorph_Cow:30|t Crit buffs"},
	{ 3, 0, GOSSIP_ICON_DOT, 16609, "|TInterface\\Icons\\Spell_Arcane_TeleportOrgrimmar:30|t Warchief's Blessing"},
	{ 3, 0, GOSSIP_ICON_DOT, 30567, "|TInterface\\Icons\\INV_Enchant_EssenceAstralLarge:30|t Torment of the Worgen"},
	{ 3, 0, GOSSIP_ICON_DOT, 30557, "|TInterface\\Icons\\INV_Enchant_EssenceMysticalLarge:30|t Wrath of the Titans"},
	{ 3, 0, GOSSIP_ICON_DOT, 30550, "|TInterface\\Icons\\INV_Enchant_EssenceNetherLarge:30|t Redemption of the Fallen"},
	{ 3, 0, GOSSIP_ICON_DOT, 30562, "|TInterface\\Icons\\INV_Enchant_EssenceEternalLarge:30|t Legacy of the Mounatin King"},
	{ 3, 0, GOSSIP_ICON_DOT, 43816, "|TInterface\\Icons\\INV_Misc_Toy_04:30|t Charm of the Bloodletter"},
	{ 3, 0, GOSSIP_ICON_DOT, 43818, "|TInterface\\Icons\\INV_Misc_Toy_02:30|t Charm of Mighty Mojo"},
	{ 3, 0, GOSSIP_ICON_DOT, 43820, "|TInterface\\Icons\\INV_Misc_Toy_01:30|t Charm of the Witch Doctor"},
	{ 3, 0, GOSSIP_ICON_DOT, 43822, "|TInterface\\Icons\\INV_Misc_Toy_03:30|t Charm of the Raging Defender"},

};

class SICKER_HELPER_NPC : public CreatureScript {
public: SICKER_HELPER_NPC() : CreatureScript("SICKER_HELPER_NPC") {}
		class SICKER_HELPER_NPCAI : public ScriptedAI {
		public:  SICKER_HELPER_NPCAI(Creature* creature) : ScriptedAI(creature) { }

				 void PrepareMenu(Player* player, Creature* creature, uint32 menuId) {
					 player->PlayerTalkClass->ClearMenus();
					 for (uint8 i = 0; i < (sizeof(FullForm) / sizeof(*FullForm)); i++) {
						 if (FullForm[i].PARENT == menuId)
							 AddGossipItemFor(player, FullForm[i].ICON, FullForm[i].NAME, GOSSIP_SENDER_MAIN, i);
					 }
					 SendGossipMenuFor(player, 907, creature->GetGUID());
				 }
				 // Hello
				 bool GossipHello(Player * player) override {
					 return OnGossipHello(player, me);
				 }
				 // View NPC Menu
				 bool OnGossipHello(Player * player, Creature* Sicker) {
					 // Player tools

					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Spell_Holy_DivineIllumination:30|t  Heal me...", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Spell_Nature_GiftoftheWild:30|t Buffs", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Ability_hunter_pathfinding:30|t |cffFF0000 V.I.P|r buffs", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Achievement_Level_60:30|t Set my level 60", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Achievement_Level_70:30|t Set my level 70", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
					 AddGossipItemFor(player, GOSSIP_ICON_DOT, "|TInterface\\Icons\\Achievement_Level_80:30|t Set my level 80", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);

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
					 case GOSSIP_ACTION_INFO_DEF + 1:
						 player->PlayerTalkClass->ClearMenus();
						 player->CastSpell(player, 69693, TRIGGERED_IGNORE_POWER_AND_REAGENT_COST);
						 player->PlayerTalkClass->SendCloseGossip();
						 return false;
					 case GOSSIP_ACTION_INFO_DEF + 2:
						 player->PlayerTalkClass->ClearMenus();
						 PrepareMenu(player, Sicker, 2);
						 return false;
					 case GOSSIP_ACTION_INFO_DEF + 3:
						 player->PlayerTalkClass->ClearMenus();
						 PrepareMenu(player, Sicker, 3);
						 return false;
					 case GOSSIP_ACTION_INFO_DEF + 4:
						 player->GiveLevel(60);
						 player->InitTalentForLevel();
						 player->SetUInt32Value(PLAYER_XP, 0);
						 ChatHandler(player->GetSession()).PSendSysMessage("Now you on 60 level.");
						 player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
						 player->PlayerTalkClass->SendCloseGossip();
						 return false;
					 case GOSSIP_ACTION_INFO_DEF + 5:
						 player->GiveLevel(70);
						 player->InitTalentForLevel();
						 player->SetUInt32Value(PLAYER_XP, 0);
						 ChatHandler(player->GetSession()).PSendSysMessage("Now you on 70 level.");
						 player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
						 player->PlayerTalkClass->SendCloseGossip();
						 return false;
					 case GOSSIP_ACTION_INFO_DEF + 6:
						 player->GiveLevel(80);
						 player->InitTalentForLevel();
						 player->SetUInt32Value(PLAYER_XP, 0);
						 ChatHandler(player->GetSession()).PSendSysMessage("Now you on 80 level.");
						 player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_NO_XP_GAIN);
						 player->PlayerTalkClass->SendCloseGossip();
						 return false;
					 }


					 uint32 menuData = FullForm[SICKER_ACTION].CHILD;

					 if (menuData == 0) {
						 player->AddAura(FullForm[SICKER_ACTION].ID, player);
						 menuData = FullForm[SICKER_ACTION].PARENT;
					 }

					 PrepareMenu(player, Sicker, menuData);
					 return true;
				 }
		};
		CreatureAI* GetAI(Creature* creature) const override {
			return new SICKER_HELPER_NPCAI(creature);
		}
};

void AddSC_SICKER_HELPER_NPC() {
	new SICKER_HELPER_NPC();
}
