#ifndef _PACKET_ENUM_H_
#define _PACKET_ENUM_H_

enum RequestPacket
{
	ReqCharAttack = 0,		// ����
	ReqChatMultiTarget,		// �Ϲ�/���/��Ƽ/��ġ��/���/����� ä��
	ReqChatTarget,			// �Ӹ�/����
	ReqCharUseSkill,		// ĳ���� ��ų ���
	ReqEventLottoReg,		// �ζ� �̺�Ʈ ��ȣ ���
	ReqEventLottoList,		// �ζ� �̺�Ʈ ��� ��ȣ ����Ʈ ������
	ReqMagaPhone,			// Ȯ����
	ReqShopGetLaim,			// ���λ��� ���� ȸ��
	ReqBingoInfo,			// ���� �̺�Ʈ ���� ��û
	ReqBingoReg,			// ���� �̺�Ʈ ��ȣ ���
	ReqBingoPresent,		// ���� �̺�Ʈ ���� ��û
	ReqPromotion,			// ȫ�� �̺�Ʈ
	ReqPetSkillInit,		// �� ��ų �ʱ�ȭ
	ReqGearUpgrade,			// ��ȭ �̺�Ʈ
	ReqNoteMesaageViewBox,	// ������ ����
	ReqNoteMessageSend,		// ���� ������
	ReqNoteMessageDel,		// ���� ����
	ReqNoteMessageRead,		// ���� �б�
	ReqGetAdmin,			// ��� ���� ȹ��
	ReqGiftTime,			// ������ ���� �̺�Ʈ ������ ��û
	ReqBaseBallHit,			// �̺�Ʈ �߱� ����
	ReqGetBaseBallGift,		// �̺�Ʈ �߱� ���� ��������
	ReqGetBaseBallGiftList,	// �̺�Ʈ �߱� ���� ������ �ִ� ����Ʈ
	ReqLottery,				// �Ｎ���� �̺�Ʈ	
	ReqSnakeDiceThrow,		// �� �ֻ��� ���� ������
	ReqSnakeDicePresentList,// �� �ֻ��� ���� ���� ����Ʈ
	ReqSnakeDiceGetPresent, // �� �ֻ��� ���� ���� �޾ư���
	
	ReqLogin,				// �α���
	ReqCharNew,				// ĳ���� ����
	ReqCharDel,				// ĳ���� ����
	ReqGameStart,			// ���� ���� (ĳ���� ���� �Ϸ�)
	ReqGamePlayReady,		// ���� �÷��� �غ� �Ϸ� (�ε� �Ϸ�)
	ReqWear,				// ��� ����
	ReqEvWear,				// �ڽ�Ƭ ����
	ReqInven,				// �κ��丮 ������ �ֱ�/����
	ReqQuick,				// ������ ������ �ֱ�/����
	ReqPulse,				// ���� �ð����� ��ŷ üũ
	ReqCharWalk,			// ĳ���� �ȱ�
	ReqCharPlace,			// ĳ���� �ȱ���
	ReqCharStop,			// ĳ���� �ȱ� �ߴ�
	ReqCharGoto,			// ĳ���� �ʳ� �̵�	
	ReqCharPkState,			// PK ����
	ReqCharStatusPointReset,// ĳ���� ���� ����
	ReqGoWorld,				// ���̵�
	ReqLagrushStart,		// ��׷��� ����
	ReqLagrushEnd,			// ��׷��� ��
	ReqHeroList,			// ���� ����Ʈ
	ReqHeroReward,			// ���� ������ ����
	ReqPartyRequest,		// ��Ƽ �ʴ�
	ReqPartyConsist,		// ��Ƽ �ʴ� ���
	ReqPartyDismiss,		// ��Ƽ�� �ذ�
	ReqPartySecession,		// ��Ƽ ������
	ReqPartyEntrust,		// ��Ƽ�� ����
	ReqPartyDisband,		// ��Ƽ ��ü
	ReqPartyVital,			// ��Ƽ�� ä�� ����	
	ReqCharSubAttack,		// ���������� ����
	ReqCharUseSubItem,		// ���������� ���
	ReqCharCombine,			// ������� ������� �ֱ�
	ReqCharSplit,			// ������� ������� ����
	ReqCharProduct,			// ������ ����
	ReqCharSkyUpgrade,		// ���༮ ����
	ReqGuildHuntOpen,		// ��� ��Ʈ â ����
	ReqGuildHuntClose,		// ��� ��Ʈ â �ݱ�
	ReqGuildHuntRequest,	// ��� ��Ʈ ��û
	ReqGuildHuntCancel,		// ��� ��Ʈ ��û ���
	ReqGuildHuntIn,			// ��� ��Ʈ ����
	ReqGuildHuntOut,		// ��� ��Ʈ ����
	ReqCharTeleport,		// �ڷ���Ʈ
	ReqCharPlatinumItem,	// �÷�Ƽ�� ������
	ReqCharSocialAction,	// �Ҽ� �ൿ
	ReqCharInterChange,		// ����ü����
	ReqExchangeRequest,		// ��ȯ ��û
	ReqExchangeResponse,	// ��ȯ ��û ����
	ReqExchangeInven,		// ��ȯâ ������ �ֱ�/����
	ReqExchangeMoney,		// ��ȯâ �� ����
	ReqExchangeTrade,		// ��ȯ Ȯ��
	ReqItemDrop,			// ������ ���
	ReqItemPick,			// ������ �ݱ�
	ReqItemQuick,			// ������ ���
	ReqItemUpgrade,			// ���� ��ȭ
	ReqItemupgradeMs,		// ���� ��ȭ	
	ReqCharStamina,			// ���׹̳� ����
	ReqItemUseQuickItem,	// ������ ������ ���
	ReqItemusePremiumItem,	// �����̾� ������ ���
	ReqItemInvenToQuick,	// ������ �̵�(�κ�->������)
	ReqItemInvenToPetInven,	// ������ �̵�(�κ�->���κ�)
	ReqItemSpShopBuy,		// SP ���� ����
	ReqItemMultiSell,		// ���� ������ �ټ� �Ǹ�
	ReqItemShopList,		// ���� ������ ����Ʈ
	ReqItemShopBuy,			// ���� ������ ����
	ReqItemShopSell,		// ���� ������ �Ǹ�
	ReqItemPotionShopBuy,	// ���� ���� ����
	ReqItemChipExchange,	// Ĩ ��ȯ
	ReqItemGambleExchange,	// ����
	ReqEventParentDay,		// ����̳� �̺�Ʈ
	ReqCharChangeSex,		// ���� ����
	ReqCharChangeHair,		// �Ӹ� ����
	ReqEventItemExchange,	// �̺�Ʈ ������ ����
	ReqEventSummer,			// ���� �̺�Ʈ
	ReqItemJewelExchange,	// ���� ��ȯ
	ReqItemSelfRefine,		// ���� ����
	ReqStashList,			// â�� ������ ����Ʈ
	ReqStashClose,			// â�� �ݱ�
	ReqStashClick,			// â�� ������ �ֱ�/����
	ReqStashPut,			// â�� �ټ� ������ �ֱ�
	ReqStashGet,			// â�� �ټ� ������ ã��
	ReqStashPuts,			// â�� �ټ� ������ �ֱ�(��׷���)
	ReqStashGets,			// â�� �ټ� ������ ã��(��׷���)
	ReqExchangeMulti,		// ��ȯâ �ټ� ������ �ֱ�/����	
	ReqCharCombatState,		// ĳ���� �Ĺ� ����
	ReqCharRevive,			// ĳ���� ��Ȱ
	ReqCharStatusUp,		// �������ͽ� ����Ʈ ��
	ReqCharSkillUp,			// ��ų ����Ʈ ��
	ReqCharFastMoveReport,	// Fast Move ��� ����
	ReqChatSalesView,		// ���ä�� ä�� ���� On/Off
	ReqCastleTaxOpen,		// ���� ����â ����
	ReqCastleTaxClose,		// ���� ����â �ݱ�
	ReqCastleTaxAccount,	// ���� ���� ã��
	ReqCastleTaxRate,		// ���� ���� ����
	ReqCastleDoorOpen,		// ���� ���� ����
	ReqCastleDoorClose,		// ���� ���� �ݱ�
	ReqCastleTowerChange,	// ���� Ÿ�� �� ����
	ReqCastleTowerControl,	// ���� Ÿ�� ����
	ReqCastleBattleRequest,	// ���� ��û
	ReqCastleBattleTime,	// ���� �ð� ����
	ReqCastleBattleOpen,	// ���� Open
	ReqTimeCheck,			// ���� �ð� Ȯ��
	ReqCastleEnergyUp,		// �������� ����
	ReqPetAttack,			// �� ����
	ReqPetComposite,		// �� ����
	ReqPetUseSkill,			// �� ��ų ���
	ReqPetChangePiece,		// �� �ǽ� ����
	ReqPetBuyEgg,			// �� �� ����
	ReqPetLevelUp,			// �� ���� ��
	ReqPetKeep,				// �� ������
	ReqPetSummon,			// �� ��ȯ
	ReqPetShop,				// �� ������ ����
	ReqPetBag,				// �� ����
	ReqPetMode,				// �� ��� ����
	ReqPetSocial,			// �� �Ҽ� �ൿ
	ReqPetPick,				// �� ������ �ݱ�
	ReqPetDrop,				// �� ���
	ReqPetPeragonDrop,		// �� ���� (������ �� ������)
	ReqWarZone,				// ����Ʋ ��ġ�� �̵�
	ReqWarInfo,				// ����Ʋ ����
	ReqWarRequest,			// ����Ʋ ��û
	ReqWarChar,				// ����Ʋ ��� �ޱ�
	ReqWarAccept,			// ����Ʋ ���
	ReqWarCancel,			// ����Ʋ ���
	ReqGpkWho,				// �������� ����� ����
	ReqGpkRequest,			// ����� ��û
	ReqGpkAccept,			// ����� ����
	ReqGpkCancel,			// ����� ����
	ReqGpkSurrender,		// ����� �׺�
	ReqItemBraceletUpgrade,	// ���� ���׷��̵�
	ReqItemNecklaceUpgrade,	// ����� ���׷��̵�
	ReqSupportRequest,		// �İ��� ��û
	ReqSupportAccept,		// �İ��� ����/����
	ReqSupportGiveup,		// �İ��� ����
	ReqSupportDisband,		// �İ��� ����
	ReqMinimap,				// �̴ϸ� ���� ����Ʈ
	ReqPartyCome,			// ��Ƽ�� ��ȯ
	ReqMatrixJoinTime,		// ����콺 �Ʒ��� �����ð�
	ReqQuestClick,			// ����Ʈ ����
	ReqQuestCancel,			// ����Ʈ ���
	ReqQuestSpecial,		// Ư�� ����Ʈ
	ReqQuestExchange,		// ����Ʈ �� ��ȯ
	ReqItemRingCombine,		// ���� ����
	ReqPrize,				// ���� �ޱ�
	ReqMapCheckGhostChar,	// ���� ��/ĳ���� üũ
	ReqItemNpcUpgrade,		// NPC�� ���� ������ ��ȭ
	ReqItemEndurance,		// ������ ������ ����
	ReqGuildFindPos,		// ��� ��ġ ã��
	ReqUsershopSearch,		// ���λ��� �˻�
	ReqUsershopRegistry,	// ���λ��� ������ ���
	ReqUsershopUnregistry,	// ���λ��� ������ ��� ���
	ReqUsershopStart,		// ���λ��� ����
	ReqUsershopModify,		// ���λ��� ������ ���� ���� ��û
	ReqUsershopBuy,			// ���λ��� ������ ����
	ReqUsershopOpen,		// ���λ��� ���� ����
	ReqUsershopLeave,		// ���λ��� ������
	ReqUsershopClose,		// ���λ��� �ݱ�(������)
	ReqUsershopRecall,		// ���λ��� ȸ��
	ReqCharRotate,			// ĳ���� ȸ��
	ReqSoccerIn,			// ������ ���� ��ü
	ReqCharUseEffect,		// ����Ʈ ���
	ReqPetUpgrade,			// �� ���׷��̵�
	ReqPetSkillUp,			// �� ��ų ���׷��̵�
	ReqDungeonFloor,		// �ν��Ͻ� ���� �� �̵�
	ReqDungeonOut,			// �ν��Ͻ� ���� ���Ƿ� �̵�
	ReqDungeonUseKey,		// ���� �� ����
	ReqItemRepair,			// ������ ������ ����
	ReqItemEnchant,			// ������ ��æƮ
	ReqCharStatusUpMulti,	// �������ͽ� ����Ʈ ��(��Ƽ)
	ReqMinorDate,			// ���̳� ���� ����
	ReqMinorRequest,		// ���̳� ���� ��û
	ReqMinorOut,			// ���̳� ���� ��� Ż��
	ReqGuildCreateTest,		// ��� ���� ���� �׽�Ʈ
	ReqGuildCreate,			// ��� ����
	ReqGuildSearch,			// ��� �˻�
	ReqGuildDisband,		// ��� �ػ�
	ReqGuildStashOpen,		// ��� â�� ����
	ReqGuildStashPut,		// ��� â�� ������ �ֱ�
	ReqGuildStashGet,		// ��� â�� ������ ����
	ReqGuildStashPutMulti,	// ��� â�� ������ �ֱ�(����)
	ReqGuildStashGetMulti,	// ��� â�� ������ ����(����)
	ReqGuildLeave,			// ��� Ż��
	ReqGuildUpgrade,		// ��� �±�
	ReqGuildRankwarInvite,	// ��� ��ŷ�� �ʴ�
	ReqGuildRankwarOpen,	// ��� ��ŷ�� UI ����
	ReqGuildRankwarList,	// ��� ��ŷ�� ����Ʈ
	ReqGuildRankwarRequest,	// ��� ��ŷ�� ��û
	ReqGuildRankwarCancel,	// ��� ��ŷ�� ���
	ReqGuildRankwarGet,		// ��� ��ŷ�� ������ ����
	ReqGuildRankwarComp,	// ��� ��ŷ�� ������ �ռ�
	ReqGuildInvasionRequest,	// ��� ħ���� ��û
	ReqGuildInvasionCancelReq,	// ��� ħ���� ��� ��û
	ReqGuildInvasionCancelRes,	// ��� ħ���� ��� ����
	ReqGuildRecommandReq,		// ��� ��õ ���� ��û
	ReqGuildRecommandRes,		// ��� ��õ ���� ����
	ReqGuildRecommandInfo,		// ��� ��õ ���� ����
	ReqGuildJoinReq,			// ��� �Ϲ� ���� ��û
	ReqGuildJoinRes,			// ��� �Ϲ� ���� ����
	ReqGuildNick,				// ��� ��޸� ����
	ReqGuildNotice,				// ��� ����
	ReqGuildAppoint,			// ��� ��å ����
	ReqGuildFire,				// ���� ����
	ReqWantedRegistTest,		// ������� ��� üũ
	ReqWantedRegist,			// ������� ���
	ReqWantedMyList,			// ���� ����� ������� ����Ʈ
	ReqWantedMyCancel,			// ���� ����� ������� ����Ʈ���� ����
	ReqWantedList,				// ������� ����Ʈ
	ReqWantedReward,			// ������� ����
	ReqFreepvpReq,				// ������� ��û
	ReqFreepvpRes,				// ������� ��û ����
	ReqFreepvpGiveup,			// ������� ����
	ReqOccpTrans,				// ������ �̵�
	ReqOccpTransPortal,			// ������ ��Ż �̵�
	ReqOccpTransGate,			// ������ ����Ʈ �̵�
	ReqOccpList,				// ������ ��Ȳ ����Ʈ
	ReqOccpBattleReq,			// ������ ��û
	ReqOccpBattleInfo,			// ������ ��û ����
	ReqOccpBattleStart,			// ������ ����
	ReqOccpBattleCancel,		// ������ ��û ���
	ReqOccpCashOpen,			// ������ ���� â ����
	ReqOccpCashAccount,			// ������ ���� ã��
	ReqOccpCashRate,			// ������ ���� ����
	ReqBossraidKey,				// ���� ���̵� ����
	ReqBossraidLicense,			// ���� ������ �����
	ReqChipRoulette,			// �׺� �귿
	ReqSupportRewardSupproter,	// �İ��� ���� �ޱ�
	ReqSupportRewardApprentice,	// �߽��� ���� �ޱ�
	ReqEventAttend,				// �⼮ �̺�Ʈ
	ReqEventGiftList,			// ���� �̺�Ʈ ����Ʈ
	ReqEventGiftGet,			// ���� �̺�Ʈ �ޱ�
	ReqGuildAttendCheck,		// ��� �⼮ üũ
	ReqLagrushBuy,				// ��׷��� ���� ������ ����
	ReqItemSeparate,			// ������ ����
	ReqItemMakes,				// ������ ����
	ReqEventExchange,			// �̺�Ʈ ������ ��ȯ
	ReqFriendReq,				// ģ�� �߰� ��û
	ReqFriendRes,				// ģ�� �߰� ����
	ReqFriendDel,				// ģ�� ����
	ReqFriendRefuseAll,			// ģ�� �ź�(��ü)
	ReqFriendRefuse,			// ģ�� �ź�
	ReqItemRecycle,				// ������ �����뿡 ������
	ReqYutThrow,				// ��������
	ReqChatTellRefuse,			// �Ӹ� �ź�
	ReqProcessReq,				// ���� ��û
	ReqProcessReject,			// ���� ��û ����
	ReqProcessCancel,			// ���� ���
	ReqProcessAccept,			// ���� ��û ����
	ReqProcessMoney,			// ���� �� ����
	ReqProcessInven,			// ���� ������ �ֱ�/����
	ReqProcessStart,			// ���� ����
	ReqItemMix,					// ���� ����
	ReqRefineReq,				// ���� ��û
	ReqRefineReject,			// ���� ��û ����
	ReqRefineCancel,			// ���� ���
	ReqRefineAccept,			// ���� ��û ����
	ReqRefineMoney,				// ���� �� ����
	ReqRefineInven,				// ���� ������ �ֱ�/����
	ReqRefineStart,				// ���� ����
	ReqPremiumCharMoveStep1,	// �����̾� ĳ���� �̵� ���� 1
	ReqPremiumCharMoveStep3,	// �����̾� ĳ���� �̵� ���� 3
	ReqPremiumCharMoveStop,		// �����̾� ĳ���� �̵� ����

	ReqPack_Max,
};

enum ResponsePacket
{
	ResMegaPhone = 0,		// Ȯ����
	ResShopGetLaim,			// ���λ���
	ResBingoInfo,			// ���� �̺�Ʈ ���� ��û ����
	ResBingoReg,			// ���� �̺�Ʈ ��ȣ ��� ����
	ResBingoPresent,		// ���� �̺�Ʈ ���� ��û ����
	ResPromotion,			// ȫ�� �̺�Ʈ
	ResPetSkillInit,		// �� ��ų �ʱ�ȭ
	ResGearUpgrade,			// ��ȭ �̺�Ʈ
	ResViewBox,				// ���� ���� ������ ����
	ResNoteMessageSend,		// ���� ������
	ResNoteMessageDel,		// ���� ����
	ResNoteMessageRead,		// ���� �б�
	ResGiftTime,			// ���� ���� �̺�Ʈ ���� ��û ����
	ResBaseBallHit,			// �߱� �̺�Ʈ ���� �� ���� �����
	ResGetBaseBallGift,		// �߱� �̺�Ʈ ���������� ���
	ResGetBaseBallGiftList, // �߱� �̺�Ʈ ���� ������ �ִ� ����Ʈ
	ResLottery,				// �Ｎ���� �̺�Ʈ	
	ResSnakeDiceThrow,		// �� �ֻ��� ���� ������
	ResSnakeDicePresentList,// �� �ֻ��� ���� ���� ����Ʈ
	ResSnakeDiceGetPresent, // �� �ֻ��� ���� ���� �޾ư���
		
	ResLogin,				// �α���
	ResCharNew,				// ĳ���� ����
	ResCharDel,				// ĳ���� ����
	ResGameStart,			// ���� ���� (ĳ���� ���� �Ϸ�)
	ResGamePlayReady,		// ���� �÷��� �غ� �Ϸ� (�ε� �Ϸ�)
	ResCharGoto,			// ĳ���� �ʳ� �̵�
	ResGoWorld,				// ���̵�
	ResHeroList,			// ���� ����Ʈ
	ResPartyRequest,		// ��Ƽ �ʴ�
	ResPartyDismiss,		// ��Ƽ�� �ذ�
	ResPartySecession,		// ��Ƽ ������
	ResPartyDisband,		// ��Ƽ ��ü
	ResCharUseSubItem,		// ���������� ���
	ResCharCombine,			// ������� ������� �ֱ�
	ResCharSplit,			// ������� ������� ����
	ResCharProduct,			// ������ ����
	ResGuildHuntOpen,		// ��� ��Ʈ â ����
	ResGuildHuntClose,		// ��� ��Ʈ â �ݱ�
	ResGuildHuntRequest,	// ��� ��Ʈ ��û
	ResGuildHuntCancel,		// ��� ��Ʈ ��û ���
	ResCharPlatinumItem,	// �÷�Ƽ�� ������
	ResCharInterChange,		// ����ü����
	ResExchangeRequest,		// ��ȯ ��û
	ResExchangeResponse,	// ��ȯ ��û ����
	ResExchangeStart,		// ��ȯ ����
	ResExchangeInvenTo,		// ��ȯâ�� ������ �߰�
	ResExchangeInvenFrom,	// ��ȯâ���� ������ ����
	ResExchangeMoney,		// ��ȯâ �� ����
	ResExchangeTrade,		// ��ȯ Ȯ��(����)
	ResExchangeChange,		// ��ȯ ó��
	ResExchangePet,			// �� ��ȯ �Ϸ�
	ResPetStatus,			// �� ����
	ResItemPick,			// ������ �ݱ�
	ResItemPickExtra,		// ������ �ݱ�(����Ʈ�� ����)
	ResItemQuick,			// ������ ���
	ResItemUpgrade,			// ���� ��ȭ
	ResItemupgradeMs,		// ���� ��ȭ
	ResItemInvenToQuick,	// ������ �̵�(�κ�->������)
	ResItemInvenToPetInven,	// ������ �̵�(�κ�->���κ�, ���)
	ResItemShopList,		// ���� ������ ����Ʈ
	ResItemShopRate,		// ���� �Ǹ� ����
	ResItemSpShopList,		// SP ���� ����Ʈ
	ResItemChipExchange,	// Ĩ ��ȯ
	ResItemGambleExchange,	// ����
	ResEventParentDay,		// ����̳� �̺�Ʈ
	ResEventSummer,			// ���� �̺�Ʈ
	ResItemJewelExchange,	// ���� ��ȯ
	ResItemSelfRefine,		// ���� ����
	ResStashList,			// â�� ������ ����Ʈ
	ResStashInfo,			// â�� ����
	ResStashTo,				// â�� ������ �߰�
	ResStashFrom,			// â���� ������ ã��
	ResStashPut,			// â�� �ټ� ������ �ֱ�
	ResStashGet,			// â�� �ټ� ������ ã��
	ResStashPuts,			// â�� �ټ� ������ �ֱ�(��׷���)
	ResStashGets,			// â�� �ټ� ������ ã��(��׷���)
	ResExchangeMulti,		// ��ȯâ �ټ� ������ �ֱ�/����	
	ResCharSkillLevel,		// ��ų ����
	ResCastleBattleTime,	// ���� �ð� ����
	ResCastleEnergyUp,		// �������� ����
	ResPetComposite,		// �� ����
	ResPetChangePiece,		// �� �ǽ� ����
	ResPetBuyEgg,			// �� �� ����
	ResPetLevelUp,			// �� ���� ��
	ResPetKeep,				// �� ������
	ResPetSummon,			// �� ��ȯ
	ResPetShop,				// �� ������ ����
	ResPetDrop,				// �� ���
	ResPetPeragonDrop,		// �� ���� (������ �� ������)
	ResWarInfo,				// ����Ʋ ����
	ResWarRequest,			// ����Ʋ ��û
	ResWarHead,				// ����Ʋ ���� ����
	ResWarCancel,			// ����Ʋ ���
	ResGpkRequest,			// ����� ��û
	ResGpkSurrender,		// ����� �׺�
	ResItemBraceletUpgrade,	// ���� ���׷��̵�
	ResItemNecklaceUpgrade,	// ����� ���׷��̵�
	ResSupportRequest,		// �İ��� ��û
	ResSupportAccept,		// �İ��� ����/����
	ResSupportDisband,		// �İ��� ����
	ResMinimap,				// �̴ϸ� ���� ����Ʈ
	ResQuestSpecial,		// Ư�� ����Ʈ
	ResQuestExchange,		// ����Ʈ �� ��ȯ
	ResItemRingCombine,		// ���� ����
	ResItemNpcUpgrade,		// NPC�� ���� ������ ��ȭ
	ResItemEndurance,		// ������ ������ ����
	ResGuildFindPos,		// ��� ��ġ ã��
	ResUsershopSearch,		// ���λ��� �˻�
	ResUsershopRegistry,	// ���λ��� ������ ���
	ResUsershopUnregistry,	// ���λ��� ������ ��� ���
	ResUsershopModify,		// ���λ��� ������ ���� ���� ��û
	ResUsershopStart,		// ���λ��� ����
	ResUsershopBuy,			// ���λ��� ������ ����
	ResUsershopOpen,		// ���λ��� ���� ����
	ResUsershopList,		// ���λ��� ������ ����Ʈ
	ResUsershopSell,		// ���λ��� �Ǹ� �˸�
	ResUsershopLeave,		// ���λ��� ������
	ResUsershopClose,		// ���λ��� �ݱ�(������)
	ResUsershopRecall,		// ���λ��� ȸ��
	ResItemRepair,			// ������ ������ ����
	ResItemEnchant,			// ������ ��æƮ
	ResMinorDate,			// ���̳� ���� ����
	ResGuildCreateTest,		// ��� ���� ���� �׽�Ʈ
	ResGuildCreate,			// ��� ����
	ResGuildSearch,			// ��� �˻�
	ResGuildDisband,		// ��� �ػ�
	ResGuildStashOpen,		// ��� â�� ����
	ResGuildStashPut,		// ��� â�� ������ �ֱ�
	ResGuildStashGet,		// ��� â�� ������ ����
	ResGuildStashPutMulti,	// ��� â�� ������ �ֱ�(����)
	ResGuildStashGetMulti,	// ��� â�� ������ ����(����)
	ResGuildLeave,			// ��� Ż��
	ResGuildUpgrade,		// ��� �±�
	ResGuildRankwarInvite,	// ��� ��ŷ�� �ʴ�
	ResGuildRankwarOpen,	// ��� ��ŷ�� UI ����
	ResGuildRankwarList,	// ��� ��ŷ�� ����Ʈ
	ResGuildRankwarRequest,	// ��� ��ŷ�� ��û
	ResGuildRankwarCancel,	// ��� ��ŷ�� ���
	ResGuildRankwarGet,		// ��� ��ŷ�� ������ ����
	ResGuildRankwarComp,	// ��� ��ŷ�� ������ �ռ�
	ResGuildInvasionRequest,	// ��� ħ���� ��û
	ResGuildInvasionCancelReq,	// ��� ħ���� ��� ��û
	ResGuildInvasionCancelRes,	// ��� ħ���� ��� ����
	ResGuildRecommandReq,		// ��� ��õ ����
	ResGuildRecommandRes,		// ��� ��õ ���� ����
	ResGuildRecommandInfo,		// ��� ��õ ���� ����
	ResGuildJoinReq,			// ��� �Ϲ� ���� ��û
	ResGuildJoinRes,			// ��� �Ϲ� ���� ����
	ResGuildNick,				// ��� ��޸� ����
	ResGuildNotice,				// ��� ����
	ResGuildAppoint,			// ��� ��å ����
	ResGuildFire,				// ���� ����
	ResWantedRegistTest,		// ������� ��� üũ
	ResWantedRegist,			// ������� ���
	ResWantedMyList,			// ���� ����� ������� ����Ʈ
	ResWantedMyCancel,			// ���� ����� ������� ����Ʈ���� ����
	ResWantedList,				// ������� ����Ʈ
	ResWantedReward,			// ������� ����
	ResFreepvpReq,				// ������� ��û
	ResFreepvpRes,				// ������� ��û ����
	ResOccpTrans,				// ������ �̵�
	ResOccpTransPortal,			// ������ ��Ż �̵�
	ResOccpTransGate,			// ������ ����Ʈ �̵�
	ResOccpList,				// ������ ��Ȳ ����Ʈ
	ResOccpBattleReq,			// ������ ��û
	ResOccpBattleInfo,			// ������ ��û ����
	ResOccpBattleCancel,		// ������ ��û ���
	ResOccpCashOpen,			// ������ ���� â ����
	ResBossraidLicense,			// ���� ������ �����
	ResEventGiftList,			// ���� �̺�Ʈ ����Ʈ
	ResEventGiftGet,			// ���� �̺�Ʈ �ޱ�
	ResGuildAttendCheck,		// ��� �⼮ üũ
	ResLagrushBuy,				// ��׷��� ���� ������ ����
	ResItemSeparate,			// ������ ����
	ResItemMakes,				// ������ ����
	ResYutThrow,				// ��������
	ResCharMultiShot,			// �޸� ���� �� ����
	ResShopMultiBuy,			// ���� ����
	ResPetResult,				// �� ó�� ���
	ResItemUse,					// ������ ���
	ResProcessReq,				// ���� ��û
	ResProcessCancel,			// ���� ���
	ResProcessAccept,			// ���� ��û ����
	ResProcessMoney,			// ���� �� ����
	ResProcessInvenTo,			// ���� ������ �ֱ�
	ResProcessInvenFrom,		// ���� ������ ����
	ResProcessResult,			// ���� ���
	ResItemMix,					// ���� ����
	ResRefineReq,				// ���� ��û
	ResRefineCancel,			// ���� ���
	ResRefineAccept,			// ���� ��û ����
	ResRefineMoney,				// ���� �� ����
	ResRefineInvenTo,			// ���� ������ �ֱ�
	ResRefineInvenFrom,			// ���� ������ ����
	ResRefineResult,			// ���� ���
	ResItemUsePremiumItemChar,	// ĳ���͸� ����
	ResItemUsePremiumItemGuild,	// ���� ����
	ResPremiumCharMoveStep1,	// �����̾� ĳ���� �̵� ���� 1
	ResPremiumCharMoveStep2,	// �����̾� ĳ���� �̵� ���� 2
	ResPremiumCharMoveStep4,	// �����̾� ĳ���� �̵� ���� 4
	ResPremiumCharMoveStop,		// �����̾� ĳ���� �̵� ����
	ResPack_Max,
};

enum UpdatePacket
{
	UpdUIClose = ResPack_Max,	// UI ���� �ݱ�
	UpdSetItemStatus,			// ��Ʈ ������ ���� ����
	UpdBingoInfo,				// ���� �̺�Ʈ ����
	UpdExtraDel,				// ���콺 ��ġ�� �ִ� ������ ����
	UpdGetItemCaru,				// ī�� ���� ������ ����
	UpdNewNoteMessage,			// ���ο� ���� �˸� ǥ�� ON / OFF
	UpdNoteMessageData,			// ���� ������
	UpdNoteMessageContent,		// ���� ����
	UpdGetBaseBallGiftList,		// �߱� ���� ���� ���� ����
	UpdBaseBallInfoList,		// �߱� ���� ��Ȳ ����
	UpdBossUseSkill,
	UpdStopLagrush,
	UpdSnakeDice,				// �� �ֻ��� ����
	UpdSnakePresentList,		// �� �ֻ��� ���� ����Ʈ
	
	UpdFirmPassword,			// �������� ���� Ȯ��
	UpdCharExist,			// ĳ���� ����â ĳ���� ����
	UpdUserExpired,			// ���� ����
	UpdCharStatus,			// ĳ���� ���Ȱ�
	UpdCharSkillLevel,		// ĳ���� ��ų ���� (����)
	UpdCharSkillLevelAll,	// ĳ���� ��ų ���� (��ü)
	UpdMyCharIndex,			// �� ĳ���� �ε���
	UpdMyCharName,			// �� ĳ���� �̸�
	UpdMyCharAdminLevel,	// �� ĳ���� ��� ����
	UpdMyCharWearing,		// �� ĳ���� ��� ��������
	UpdMyCharEvWearing,		// �� ĳ���� �ڽ�Ƭ ��������
	UpdMyCharEvWearTime,	// �� ĳ���� �ڽ�Ƭ �ð� ���� ����
	UpdCharWear,			// ĳ���� ��� ����
	UpdCharEvWear,			// ĳ���� �ڽ�Ƭ ����
	UpdCharEvShowFlag,		// �ڽ�Ƭ ����/�Ⱥ���
	UpdMyCharInven,			// �� ĳ���� �κ�
	UpdMyCharExtraInven,	// �� ĳ���� ����Ʈ�� �κ�
	UpdMyCharQuickSlot,		// �� ĳ���� ������ ����
	UpdMyCharLaghaimPoint,	// ������� ����Ʈ(LP)
	UpdServerNumber,		// ���� ��ȣ
	UpdMapAttribute,		// �� �Ӽ� ����
	UpdCharAt,				// ĳ���� ó�� ���� ��ġ
	UpdMapHour,				// �� �ð�
	UpdMapWeather,			// �� ����
	UpdMyCharLocateLP,		// �������� �� ȹ�� LP
	UpdMyCharFreeUser,		// �����̺�Ʈ ����
	UpdMyCharPrizeAlram,	// ���� �˶�
	UpdInfoMessage,			// ���� �޽��� ���
	UpdCharMoveSpeed,		// ĳ���� �̵� �ӵ�
	UpdChannelNumber,		// ä�� ��ȣ
	UpdEventFeverTime,		// �ǹ�Ÿ��
	UpdCharWalk,			// �ȱ�
	UpdCharPlace,			// �ȱ���
	UpdCharStop,			// �ȱ� �ߴ�
	UpdCharSkill,			// ��ų �ߵ�
	UpdCharPkState,			// ĳ���� PK ���� ����
	UpdPetStatus,			// �� ����
	UpdLagrushEnable,		// ��׷��� ��� ����
	UpdLagrushRemainTime,	// ��׷��� ���� �ð�
	UpdHeroRewardEnable,	// ���� ���� ���� ���� ����
	UpdHeroInfoMessage,		// ���� ���� �޽���
	UpdPartyList,			// ��Ƽ ����Ʈ
	UpdPartyTypeChange,		// ��Ƽ �й� ��� ����
	UpdPartyVital,			// ��Ƽ�� ü��
	UpdCharAttack,			// ����
	UpdCharAttackVital,		// ���� �� ü��
	UpdCharSubAttack,		// ���������� ����
	UpdCharSubAttackVital,	// ���������� ���� �� ü��
	UpdCharSky,				// ���༮ ����
	UpdGuildHuntDoor,		// ��� ��Ʈ �� ����/��ħ
	UpdCharSocialAction,	// �Ҽ� �ൿ
	UpdItemDrop,			// ������ ���
	UpdItemPick,			// ������ �ݱ�
	UpdCharChangeSex,		// ���� ����
	UpdCharChangeHair,		// �Ӹ� ����
	UpdCharLevelUp,			// ������ ����Ʈ
	UpdCharCombatState,		// ĳ���� �Ĺ� ����
	UpdCharRevive,			// ĳ���� ��Ȱ
	UpdChatSales,			// ���ä�� ä��
	UpdCastleTaxInfo,		// ���� ����
	UpdPetSocial,			// �� �Ҽ� �ൿ
	UpdWarChar,				// ����Ʋ ��� �ޱ�
	UpdWarPoint,			// ����Ʋ ����Ʈ ����
	UpdWarNotify,			// ����Ʋ �˸� ����
	UpdGpkStart,			// ����� ����
	UpdGpkEnd,				// ����� ��
	UpdGpkPlay,				// ����� ����
	UpdGpkStop,				// ����� ���� ����
	UpdSupportSuccessFail,	// �İ��� ����/����
	UpdSupportEnable,		// �İ��� ���� ����
	UpdSupportInfo,			// �İ��� ����
	UpdSupportConnect,		// �İ��� ���� ����
	UpdQuestGet,			// ����Ʈ ����
	UpdQuestEnd,			// ����Ʈ ����
	UpdQuestKing,			// ����Ʈ ���� üũ
	UpdQuestKill,			// ����Ʈ �� ���� ����
	UpdQuestQuick,			// ����Ʈ ���� ����
	UpdQuestFail,			// ����Ʈ ����
	UpdQuestEffect,			// ����Ʈ ����Ʈ ����
	UpdQuestTotalPoint,		// ����Ʈ �� ����Ʈ
	UpdQuestPoint,			// ����Ʈ ����Ʈ
	UpdQuestNext,			// ���� ����Ʈ��
	UpdQuestBonus,			// ����Ʈ ���� ��� ǥ��
	UpdPrize,				// ���� ����
	UpdUsershopCharIn,		// ���λ��� ĳ���� �� ����
	UpdUsershopCharOut,		// ���λ��� ĳ���� �� �ƿ�
	UpdUsershopNpcIn,		// ���λ��� NPC �� ����
	UpdUsershopNpcOut,		// ���λ��� NPC �� �ƿ�
	UpdCharRotate,			// ĳ���� ȸ��
	UpdSoccerTime,			// ������ �ð�
	UpdSoccerSocre,			// ������ ���ھ�
	UpdSoccerName,			// ������ ����̸�
	UpdCharUseEffect,		// ����Ʈ ���
	UpdMinorGuildName,		// ���̳� ���� ��� �̸�
	UpdGuildRankwarInfo,	// ��� ��ŷ�� ����
	UpdGuildRankwarReady,	// ��� ��ŷ�� �غ�
	UpdGuildRankwarCountdown,	// ��� ��ŷ�� ī��Ʈ�ٿ�
	UpdGuildRankwarTime,	// ��� ��ŷ�� �ð�
	UpdGuildRankwarKillCount,	// ��� ��ŷ�� ų ī��Ʈ
	UpdGuildRankwarStart,	// ��� ��ŷ�� ����
	UpdGuildRankwarEnd,		// ��� ��ŷ�� ����
	UpdGuildRankwarGoworld,	// ��� ��ŷ�� �� �̵�
	UpdGuildInvasionInfo,	// ��� ħ���� ����
	UpdGuildNotice,			// ��� ����
	UpdGuildFire,			// ���� ����
	UpdGuildInfo,			// ��� ����
	UpdGuildMemberInfo,		// ���� ����
	UpdGuildWarning,		// ��� ħ�� ���� �˸�
	UpdGuildMsg,			// ��� ���� ��� �޽���
	UpdWantedMark,			// ������� ��ũ
	UpdFreepvpRing,			// ������� ������
	UpdFreepvpStart,		// ������� ����
	UpdFreepvpEnd,			// ������� ����
	UpdFreepvpResult,		// ������� ���
	UpdFreepvpIn,			// ������� ĳ���� ����
	UpdFreepvpVital,		// ������� HP
	UpdFreepvpRecord,		// ������� ���� ���
	UpdFreepvpError,		// ������� ���� ����
	UpdOccpBattleStart,		// ������ ����
	UpdOccpBattleEnd,		// ������ ����
	UpdOccpBattleFail,		// ������ ��û ����
	UpdOccpInfo,			// ������ ���� ����
	UpdOccpBuff,			// ������ ����
	UpdOccpCount,			// ������ ���� ����
	UpdFriendReq,			// ģ�� �߰� ��û
	UpdFriendAdd,			// ģ�� �߰�
	UpdFriendDel,			// ģ�� ����
	UpdFriendRefuseAll,		// ģ�� �ź�(��ü)
	UpdFriendRefuse,		// ģ�� �ź�
	UpdFriendList,			// ģ�� ���
	UpdFriendRefuseList,	// ģ�� �ź� ���
	UpdFriendLogin,			// ģ�� �α���
	UpdFriendLogout,		// ģ�� �α׾ƿ�
	UpdFriendZone,			// ģ�� �� �̵�
	UpdYutInfo,				// �������� ����
	UpdMapInChar,			// ĳ���� ����
	UpdMapInNpc,			// NPC ����
	UpdMapInPet,			// �� ����
	UpdMapInItem,			// ������ ����
	UpdMapOut,				// �ƿ�
	UpdMyItemRemoveAll,		// �� �κ� ��� �����
	UpdCharKill,			// ĳ���� �״� ���
	UpdCharWearRemove,		// ĳ���� ���� ������ ����
	UpdCharEvWearRemove,	// ĳ���� ���� �ڽ�Ƭ ����
	UpdNotice,				// ���� ����
	UpdEffect,				// ����Ʈ/��� ���
	UpdGuildName,			// ��� �̸�
	UpdGuildGrade,			// ��� ���
	UpdCharMultiShot,		// �޸� ���� �� ����
	UpdEvent,				// �̺�Ʈ ȿ��
	UpdYouAre,				// ��� ���� ����
	UpdMapOpenCastle,		// ���� ����
	UpdMapCloseCastle,		// ���� �ݱ�
	UpdMapOpenDoor,			// �� ����
	UpdMapBattleOn,			// ������ ��Ȳ
	UpdMapBattleCount,		// ������ ���� �ð�
	UpdMapBattleCrystal,	// ���� ������
	UpdMapBattleEnd,		// ������ ����
	UpdMapBattleInfo,		// ������ ���� ���� ��� ����
	UpdPopupMsg,			// �˾� �޽���
	UpdPopupMsg2,			// �˾� �޽���(Ŭ�� ��Ʈ�� ��ȣ)
	UpdGuildLevel,			// ��� ����
	UpdPetDel,				// �� ����
	UpdPetClear,			// �� ��ȯ
	UpdMyPet,				// ���� �� ����
	UpdCharCondition,		// ĳ���� ���� ����
	UpdNpcConDistruction,	// �� �迭 �ı�
	UpdCharSubEffect,		// ���� ������ ȿ��
	UpdCharSubEffectMe,		// ���� ������ ȿ�� ������ �ɱ�
	UpdCharSummon,			// ��ȯ�� ��ȯ
	UpdCharChao,			// ī�� ���
	UpdCharJump,			// �� �̵� ��ǥ ����ȭ
	UpdCharMultiAttack,		// ĳ���� ��Ƽ ����
	UpdEventNewYear,		// ���� �̺�Ʈ ����
	UpdEventAncient,		// �̺�Ʈ ó��
	UpdEventArborDay,		// �ĸ��� �̺�Ʈ
	UpdItemTeleport,		// ���� ���� ��ġ
	UpdItemCandy,			// �ջ���
	UpdItemApple,			// �ջ��
	UpdItemStarCandy,		// ������
	UpdCharAttackMiss,		// ���� �̽�
	UpdCharAttackBlock,		// ���� ���
	UpdEventPointCheck,		// �̺�Ʈ ����Ʈ üũ
	UpdCharPkItemDrop,		// PK ������ ���
	UpdEffectFirework,		// ���� ����Ʈ
	UpdCharDStone,			// ����ν���
	UpdMapTimeShopOpen,		// ȭ��Ʈȥ ���� �߾ӱ��� Ÿ�� ��	
	UpdCharCombo,			// �޺� ���
	UpdItemExtraInfoTime,	// ������ �߰� ���� (�����ð�)
	UpdItemExtraInfoDel,	// ������ �߰� ���� (�ð�����)
	UpdItemExtraInfoPrice,	// ������ �߰� ���� (Ư������ ����)
	UpdItemExtraInfoAll,	// ������ �߰� ���� (�ð��� �� Ư������ ����)
	UpdCharSummonSquad,		// Summon Squad
	UpdMyCharUpdateItem,	// ������ ����
	UpdCharMovingSkill,		// �̵��ϸ� ���� ��ų
	UpdNpcWear,				// NPC �ǻ� ������
	UpdBpShopList,			// Bp ���� ����Ʈ
	UpdPetEffect,			// �� ����Ʈ
	UpdSpiritStart,			// ���Ǹ� ����
	UpdSpiritPoint,			// ���Ǹ� ����Ʈ
	UpdSpiritTime,			// ���Ǹ� ���� �ð�
	UpdSpiritEnd,			// ���Ǹ� ��� ����
	UpdMapChangeAttribute,	// �� �Ӽ� ����
	UpdEventAction,			// �̺�Ʈ Ư�� ����
	UpdSkillCooltime,		// ��ų ��Ÿ�� ����
	UpdCheckJoin,			// ���� ���� �˸�
	UpdBossraidAttack,		// ���� ���̵� ����
	UpdBossraidKillFlag,	// ���� ���̵� ų �÷���
	UpdBossraidRankList,	// ���� ���̵� ���嵵 ����
	UpdLpShopList,			// Lp ���� ����Ʈ
	UpdCharFullsetFlag,		// ��Ʈ ������ ȿ�� ����
	UpdInvenInit,			// �κ� �ʱ�ȭ
	UpdMyCharRemoveItem,	// ������ ����
	UpdSupportRewardListApprentice,	// �߽��� ���� ����Ʈ	
	UpdItemBuff,			// ������ ����
	UpdSkillContinue,		// ���� ��ų
	UpdCharSize,			// ĳ���� ������ ����
	UpdNpcVisible,			// ĳ���� �� ���̱�
	UpdChatNormal,			// �Ϲ� ä��
	UpdChatTell,			// �Ӹ� ä��
	UpdChatGuild,			// ��� ä��
	UpdChatParty,			// ��Ƽ ä��

	UpdPack_Max,
};

enum NotifyPacket
{
	NtfMegaPhone = UpdPack_Max, // Ȯ����
};

#endif // _PACKET_ENUM_H_
