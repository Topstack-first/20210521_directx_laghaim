#ifndef _PACKET_ENUM_H_
#define _PACKET_ENUM_H_

enum RequestPacket
{
	ReqCharAttack = 0,		// 공격
	ReqChatMultiTarget,		// 일반/길드/파티/외치기/장사/길드전 채팅
	ReqChatTarget,			// 귓말/쪽지
	ReqCharUseSkill,		// 캐릭터 스킬 사용
	ReqEventLottoReg,		// 로또 이벤트 번호 등록
	ReqEventLottoList,		// 로또 이벤트 등록 번호 리스트 내리기
	ReqMagaPhone,			// 확성기
	ReqShopGetLaim,			// 개인상점 라임 회수
	ReqBingoInfo,			// 빙고 이벤트 정보 요청
	ReqBingoReg,			// 빙고 이벤트 번호 등록
	ReqBingoPresent,		// 빙고 이벤트 선물 요청
	ReqPromotion,			// 홍보 이벤트
	ReqPetSkillInit,		// 펫 스킬 초기화
	ReqGearUpgrade,			// 강화 이벤트
	ReqNoteMesaageViewBox,	// 쪽지함 보기
	ReqNoteMessageSend,		// 쪽지 보내기
	ReqNoteMessageDel,		// 쪽지 삭제
	ReqNoteMessageRead,		// 쪽지 읽기
	ReqGetAdmin,			// 운영자 강제 획득
	ReqGiftTime,			// 아이템 지급 이벤트 아이템 요청
	ReqBaseBallHit,			// 이벤트 야구 쳣다
	ReqGetBaseBallGift,		// 이벤트 야구 선물 가져가기
	ReqGetBaseBallGiftList,	// 이벤트 야구 선물 받을수 있는 리스트
	ReqLottery,				// 즉석복권 이벤트	
	ReqSnakeDiceThrow,		// 뱀 주사위 게임 던지기
	ReqSnakeDicePresentList,// 뱀 주사위 게임 선물 리스트
	ReqSnakeDiceGetPresent, // 뱀 주사위 게임 선물 받아가기
	
	ReqLogin,				// 로그인
	ReqCharNew,				// 캐릭터 생성
	ReqCharDel,				// 캐릭터 삭제
	ReqGameStart,			// 게임 시작 (캐릭터 선택 완료)
	ReqGamePlayReady,		// 게임 플레이 준비 완료 (로딩 완료)
	ReqWear,				// 장비 착용
	ReqEvWear,				// 코스튬 착용
	ReqInven,				// 인벤토리 아이템 넣기/빼기
	ReqQuick,				// 퀵슬롯 아이템 넣기/빼기
	ReqPulse,				// 일정 시간마다 해킹 체크
	ReqCharWalk,			// 캐릭터 걷기
	ReqCharPlace,			// 캐릭터 걷기중
	ReqCharStop,			// 캐릭터 걷기 중단
	ReqCharGoto,			// 캐릭터 맵내 이동	
	ReqCharPkState,			// PK 선언
	ReqCharStatusPointReset,// 캐릭터 스탯 리셋
	ReqGoWorld,				// 존이동
	ReqLagrushStart,		// 라그러시 시작
	ReqLagrushEnd,			// 라그러시 끝
	ReqHeroList,			// 영웅 리스트
	ReqHeroReward,			// 영웅 아이템 지급
	ReqPartyRequest,		// 파티 초대
	ReqPartyConsist,		// 파티 초대 허락
	ReqPartyDismiss,		// 파티원 해고
	ReqPartySecession,		// 파티 나가기
	ReqPartyEntrust,		// 파티장 위임
	ReqPartyDisband,		// 파티 해체
	ReqPartyVital,			// 파티원 채력 정보	
	ReqCharSubAttack,		// 보조아이템 공격
	ReqCharUseSubItem,		// 보조아이템 사용
	ReqCharCombine,			// 대형장비에 소형장비 넣기
	ReqCharSplit,			// 대형장비에 소형장비 빼기
	ReqCharProduct,			// 아이템 조합
	ReqCharSkyUpgrade,		// 비행석 레벨
	ReqGuildHuntOpen,		// 길드 헌트 창 열기
	ReqGuildHuntClose,		// 길드 헌트 창 닫기
	ReqGuildHuntRequest,	// 길드 헌트 신청
	ReqGuildHuntCancel,		// 길드 헌트 신청 취소
	ReqGuildHuntIn,			// 길드 헌트 입장
	ReqGuildHuntOut,		// 길드 헌트 퇴장
	ReqCharTeleport,		// 텔레포트
	ReqCharPlatinumItem,	// 플래티늄 아이템
	ReqCharSocialAction,	// 소셜 행동
	ReqCharInterChange,		// 인터체인지
	ReqExchangeRequest,		// 교환 요청
	ReqExchangeResponse,	// 교환 요청 응답
	ReqExchangeInven,		// 교환창 아이템 넣기/빼기
	ReqExchangeMoney,		// 교환창 돈 변경
	ReqExchangeTrade,		// 교환 확정
	ReqItemDrop,			// 아이템 드롭
	ReqItemPick,			// 아이템 줍기
	ReqItemQuick,			// 퀵슬롯 등록
	ReqItemUpgrade,			// 장착 강화
	ReqItemupgradeMs,		// 마석 강화	
	ReqCharStamina,			// 스테미너 변경
	ReqItemUseQuickItem,	// 퀵슬롯 아이템 사용
	ReqItemusePremiumItem,	// 프리미엄 아이템 사용
	ReqItemInvenToQuick,	// 아이템 이동(인벤->퀵슬롯)
	ReqItemInvenToPetInven,	// 아이템 이동(인벤->펫인벤)
	ReqItemSpShopBuy,		// SP 상점 구매
	ReqItemMultiSell,		// 상점 아이템 다수 판매
	ReqItemShopList,		// 상점 아이템 리스트
	ReqItemShopBuy,			// 상점 아이템 구매
	ReqItemShopSell,		// 상점 아이템 판매
	ReqItemPotionShopBuy,	// 포션 상점 구매
	ReqItemChipExchange,	// 칩 교환
	ReqItemGambleExchange,	// 갬블
	ReqEventParentDay,		// 어버이날 이벤트
	ReqCharChangeSex,		// 성별 변경
	ReqCharChangeHair,		// 머리 변경
	ReqEventItemExchange,	// 이벤트 아이템 변경
	ReqEventSummer,			// 여름 이벤트
	ReqItemJewelExchange,	// 보석 교환
	ReqItemSelfRefine,		// 셀프 정제
	ReqStashList,			// 창고 아이템 리스트
	ReqStashClose,			// 창고 닫기
	ReqStashClick,			// 창고 아이템 넣기/빼기
	ReqStashPut,			// 창고 다수 아이템 넣기
	ReqStashGet,			// 창고 다수 아이템 찾기
	ReqStashPuts,			// 창고 다수 아이템 넣기(라그러시)
	ReqStashGets,			// 창고 다수 아이템 찾기(라그러시)
	ReqExchangeMulti,		// 교환창 다수 아이템 넣기/빼기	
	ReqCharCombatState,		// 캐릭터 컴뱃 상태
	ReqCharRevive,			// 캐릭터 부활
	ReqCharStatusUp,		// 스테이터스 포인트 업
	ReqCharSkillUp,			// 스킬 포인트 업
	ReqCharFastMoveReport,	// Fast Move 사용 보고
	ReqChatSalesView,		// 장사채널 채팅 보기 On/Off
	ReqCastleTaxOpen,		// 성주 세금창 열기
	ReqCastleTaxClose,		// 성주 세금창 닫기
	ReqCastleTaxAccount,	// 성주 세금 찾기
	ReqCastleTaxRate,		// 성주 세율 변경
	ReqCastleDoorOpen,		// 성주 성문 열기
	ReqCastleDoorClose,		// 성주 성문 닫기
	ReqCastleTowerChange,	// 공성 타워 방어도 변경
	ReqCastleTowerControl,	// 공성 타워 제어
	ReqCastleBattleRequest,	// 공성 신청
	ReqCastleBattleTime,	// 공성 시간 변경
	ReqCastleBattleOpen,	// 공성 Open
	ReqTimeCheck,			// 서버 시간 확인
	ReqCastleEnergyUp,		// 에너지볼 투입
	ReqPetAttack,			// 펫 공격
	ReqPetComposite,		// 펫 조합
	ReqPetUseSkill,			// 펫 스킬 사용
	ReqPetChangePiece,		// 펫 피스 변경
	ReqPetBuyEgg,			// 펫 알 구입
	ReqPetLevelUp,			// 펫 레벨 업
	ReqPetKeep,				// 펫 보관소
	ReqPetSummon,			// 펫 소환
	ReqPetShop,				// 펫 아이템 구입
	ReqPetBag,				// 펫 가방
	ReqPetMode,				// 펫 모드 변경
	ReqPetSocial,			// 펫 소셜 행동
	ReqPetPick,				// 펫 아이템 줍기
	ReqPetDrop,				// 펫 드롭
	ReqPetPeragonDrop,		// 펫 페라곤 (성숙한 알 포데기)
	ReqWarZone,				// 팀배틀 위치로 이동
	ReqWarInfo,				// 팀배틀 정보
	ReqWarRequest,			// 팀배틀 신청
	ReqWarChar,				// 팀배틀 멤버 받기
	ReqWarAccept,			// 팀배틀 허락
	ReqWarCancel,			// 팀배틀 취소
	ReqGpkWho,				// 진행중인 길드전 정보
	ReqGpkRequest,			// 길드전 신청
	ReqGpkAccept,			// 길드전 수락
	ReqGpkCancel,			// 길드전 거절
	ReqGpkSurrender,		// 길드전 항복
	ReqItemBraceletUpgrade,	// 팔찌 업그레이드
	ReqItemNecklaceUpgrade,	// 목걸이 업그레이드
	ReqSupportRequest,		// 후견인 요청
	ReqSupportAccept,		// 후견인 수락/거절
	ReqSupportGiveup,		// 후견인 포기
	ReqSupportDisband,		// 후견인 해지
	ReqMinimap,				// 미니맵 지시 포인트
	ReqPartyCome,			// 파티원 소환
	ReqMatrixJoinTime,		// 뫼비우스 아레나 참여시간
	ReqQuestClick,			// 퀘스트 연계
	ReqQuestCancel,			// 퀘스트 취소
	ReqQuestSpecial,		// 특별 퀘스트
	ReqQuestExchange,		// 퀘스트 돈 교환
	ReqItemRingCombine,		// 반지 결합
	ReqPrize,				// 선물 받기
	ReqMapCheckGhostChar,	// 유령 몹/캐릭터 체크
	ReqItemNpcUpgrade,		// NPC를 통한 아이템 강화
	ReqItemEndurance,		// 아이템 내구도 충전
	ReqGuildFindPos,		// 길드 위치 찾기
	ReqUsershopSearch,		// 개인상점 검색
	ReqUsershopRegistry,	// 개인상점 아이템 등록
	ReqUsershopUnregistry,	// 개인상점 아이템 등록 취소
	ReqUsershopStart,		// 개인상점 시작
	ReqUsershopModify,		// 개인상점 아이템 수량 수정 요청
	ReqUsershopBuy,			// 개인상점 아이템 구매
	ReqUsershopOpen,		// 개인상점 열어 보기
	ReqUsershopLeave,		// 개인상점 떠나기
	ReqUsershopClose,		// 개인상점 닫기(끝내기)
	ReqUsershopRecall,		// 개인상점 회수
	ReqCharRotate,			// 캐릭터 회전
	ReqSoccerIn,			// 월드컵 선수 교체
	ReqCharUseEffect,		// 이펙트 사용
	ReqPetUpgrade,			// 펫 업그레이드
	ReqPetSkillUp,			// 펫 스킬 업그레이드
	ReqDungeonFloor,		// 인스턴스 던전 층 이동
	ReqDungeonOut,			// 인스턴스 던전 대기실로 이동
	ReqDungeonUseKey,		// 던전 문 열기
	ReqItemRepair,			// 아이템 내구도 수리
	ReqItemEnchant,			// 아이템 인챈트
	ReqCharStatusUpMulti,	// 스테이터스 포인트 업(멀티)
	ReqMinorDate,			// 마이너 공성 일자
	ReqMinorRequest,		// 마이너 공성 신청
	ReqMinorOut,			// 마이너 공성 길드 탈퇴
	ReqGuildCreateTest,		// 길드 생성 가능 테스트
	ReqGuildCreate,			// 길드 생성
	ReqGuildSearch,			// 길드 검색
	ReqGuildDisband,		// 길드 해산
	ReqGuildStashOpen,		// 길드 창고 열기
	ReqGuildStashPut,		// 길드 창고 아이템 넣기
	ReqGuildStashGet,		// 길드 창고 아이템 빼기
	ReqGuildStashPutMulti,	// 길드 창고 아이템 넣기(다중)
	ReqGuildStashGetMulti,	// 길드 창고 아이템 빼기(다중)
	ReqGuildLeave,			// 길드 탈퇴
	ReqGuildUpgrade,		// 길드 승급
	ReqGuildRankwarInvite,	// 길드 랭킹전 초대
	ReqGuildRankwarOpen,	// 길드 랭킹전 UI 열기
	ReqGuildRankwarList,	// 길드 랭킹전 리스트
	ReqGuildRankwarRequest,	// 길드 랭킹전 신청
	ReqGuildRankwarCancel,	// 길드 랭킹전 취소
	ReqGuildRankwarGet,		// 길드 랭킹전 아이템 지급
	ReqGuildRankwarComp,	// 길드 랭킹전 아이템 합성
	ReqGuildInvasionRequest,	// 길드 침략전 신청
	ReqGuildInvasionCancelReq,	// 길드 침략전 취소 요청
	ReqGuildInvasionCancelRes,	// 길드 침략전 취소 응답
	ReqGuildRecommandReq,		// 길드 추천 가입 요청
	ReqGuildRecommandRes,		// 길드 추천 가입 응답
	ReqGuildRecommandInfo,		// 길드 추천 가입 정보
	ReqGuildJoinReq,			// 길드 일반 가입 요청
	ReqGuildJoinRes,			// 길드 일반 가입 응답
	ReqGuildNick,				// 길드 계급명 변경
	ReqGuildNotice,				// 길드 공지
	ReqGuildAppoint,			// 길드 직책 관리
	ReqGuildFire,				// 길드원 강퇴
	ReqWantedRegistTest,		// 현상수배 등록 체크
	ReqWantedRegist,			// 현상수배 등록
	ReqWantedMyList,			// 내가 등록한 현상수배 리스트
	ReqWantedMyCancel,			// 내가 등록한 현상수배 리스트에서 제거
	ReqWantedList,				// 현상수배 리스트
	ReqWantedReward,			// 현상수배 보상
	ReqFreepvpReq,				// 자유대련 신청
	ReqFreepvpRes,				// 자유대련 신청 응답
	ReqFreepvpGiveup,			// 자유대련 포기
	ReqOccpTrans,				// 점령전 이동
	ReqOccpTransPortal,			// 점령전 포탈 이동
	ReqOccpTransGate,			// 점령전 게이트 이동
	ReqOccpList,				// 점령전 현황 리스트
	ReqOccpBattleReq,			// 점령전 요청
	ReqOccpBattleInfo,			// 점령전 신청 정보
	ReqOccpBattleStart,			// 점령전 시작
	ReqOccpBattleCancel,		// 점령전 요청 취소
	ReqOccpCashOpen,			// 점령전 세금 창 열기
	ReqOccpCashAccount,			// 점령전 세금 찾기
	ReqOccpCashRate,			// 점령전 세율 조정
	ReqBossraidKey,				// 보스 레이드 입장
	ReqBossraidLicense,			// 최종 보스존 입장권
	ReqChipRoulette,			// 겜블 룰렛
	ReqSupportRewardSupproter,	// 후견인 보상 받기
	ReqSupportRewardApprentice,	// 견습생 보상 받기
	ReqEventAttend,				// 출석 이벤트
	ReqEventGiftList,			// 선물 이벤트 리스트
	ReqEventGiftGet,			// 선물 이벤트 받기
	ReqGuildAttendCheck,		// 길드 출석 체크
	ReqLagrushBuy,				// 라그러시 전용 아이템 구매
	ReqItemSeparate,			// 아이템 분해
	ReqItemMakes,				// 아이템 재작
	ReqEventExchange,			// 이벤트 아이템 교환
	ReqFriendReq,				// 친구 추가 요청
	ReqFriendRes,				// 친구 추가 응답
	ReqFriendDel,				// 친구 삭제
	ReqFriendRefuseAll,			// 친구 거부(전체)
	ReqFriendRefuse,			// 친구 거부
	ReqItemRecycle,				// 아이템 휴지통에 버리기
	ReqYutThrow,				// 윷던지기
	ReqChatTellRefuse,			// 귓말 거부
	ReqProcessReq,				// 가공 요청
	ReqProcessReject,			// 가공 요청 거절
	ReqProcessCancel,			// 가공 취소
	ReqProcessAccept,			// 가공 요청 수락
	ReqProcessMoney,			// 가공 돈 변경
	ReqProcessInven,			// 가공 아이템 넣기/빼기
	ReqProcessStart,			// 가공 시작
	ReqItemMix,					// 무기 조합
	ReqRefineReq,				// 정제 요청
	ReqRefineReject,			// 정제 요청 거절
	ReqRefineCancel,			// 정제 취소
	ReqRefineAccept,			// 정제 요청 수락
	ReqRefineMoney,				// 정제 돈 변경
	ReqRefineInven,				// 정제 아이템 넣기/빼기
	ReqRefineStart,				// 정제 시작
	ReqPremiumCharMoveStep1,	// 프리미엄 캐릭터 이동 스텝 1
	ReqPremiumCharMoveStep3,	// 프리미엄 캐릭터 이동 스텝 3
	ReqPremiumCharMoveStop,		// 프리미엄 캐릭터 이동 스톱

	ReqPack_Max,
};

enum ResponsePacket
{
	ResMegaPhone = 0,		// 확성기
	ResShopGetLaim,			// 개인상점
	ResBingoInfo,			// 빙고 이벤트 정보 요청 응답
	ResBingoReg,			// 빙고 이벤트 번호 등록 응답
	ResBingoPresent,		// 빙고 이벤트 선물 요청 응답
	ResPromotion,			// 홍보 이벤트
	ResPetSkillInit,		// 펫 스킬 초기화
	ResGearUpgrade,			// 강화 이벤트
	ResViewBox,				// 쪽지 받은 쪽지함 보기
	ResNoteMessageSend,		// 쪽지 보내기
	ResNoteMessageDel,		// 쪽지 삭제
	ResNoteMessageRead,		// 쪽지 읽기
	ResGiftTime,			// 선물 지급 이벤트 선물 요청 응답
	ResBaseBallHit,			// 야구 이벤트 쳣다 에 대한 결과값
	ResGetBaseBallGift,		// 야구 이벤트 선물가져기 결과
	ResGetBaseBallGiftList, // 야구 이벤트 선물 받을수 있는 리스트
	ResLottery,				// 즉석복권 이벤트	
	ResSnakeDiceThrow,		// 뱀 주사위 게임 던지기
	ResSnakeDicePresentList,// 뱀 주사위 게임 선물 리스트
	ResSnakeDiceGetPresent, // 뱀 주사위 게임 선물 받아가기
		
	ResLogin,				// 로그인
	ResCharNew,				// 캐릭터 생성
	ResCharDel,				// 캐릭터 삭제
	ResGameStart,			// 게임 시작 (캐릭터 선택 완료)
	ResGamePlayReady,		// 게임 플레이 준비 완료 (로딩 완료)
	ResCharGoto,			// 캐릭터 맵내 이동
	ResGoWorld,				// 존이동
	ResHeroList,			// 영웅 리스트
	ResPartyRequest,		// 파티 초대
	ResPartyDismiss,		// 파티원 해고
	ResPartySecession,		// 파티 나가기
	ResPartyDisband,		// 파티 해체
	ResCharUseSubItem,		// 보조아이템 사용
	ResCharCombine,			// 대형장비에 소형장비 넣기
	ResCharSplit,			// 대형장비에 소형장비 빼기
	ResCharProduct,			// 아이템 조합
	ResGuildHuntOpen,		// 길드 헌트 창 열기
	ResGuildHuntClose,		// 길드 헌트 창 닫기
	ResGuildHuntRequest,	// 길드 헌트 신청
	ResGuildHuntCancel,		// 길드 헌트 신청 취소
	ResCharPlatinumItem,	// 플래티늄 아이템
	ResCharInterChange,		// 인터체인지
	ResExchangeRequest,		// 교환 요청
	ResExchangeResponse,	// 교환 요청 응답
	ResExchangeStart,		// 교환 시작
	ResExchangeInvenTo,		// 교환창에 아이템 추가
	ResExchangeInvenFrom,	// 교환창에서 아이템 제거
	ResExchangeMoney,		// 교환창 돈 변경
	ResExchangeTrade,		// 교환 확정(상대방)
	ResExchangeChange,		// 교환 처리
	ResExchangePet,			// 펫 교환 완료
	ResPetStatus,			// 펫 상태
	ResItemPick,			// 아이템 줍기
	ResItemPickExtra,		// 아이템 줍기(엑스트라 슬롯)
	ResItemQuick,			// 퀵슬롯 등록
	ResItemUpgrade,			// 장착 강화
	ResItemupgradeMs,		// 마석 강화
	ResItemInvenToQuick,	// 아이템 이동(인벤->퀵슬롯)
	ResItemInvenToPetInven,	// 아이템 이동(인벤->펫인벤, 사과)
	ResItemShopList,		// 상점 아이템 리스트
	ResItemShopRate,		// 상점 판매 정보
	ResItemSpShopList,		// SP 상점 리스트
	ResItemChipExchange,	// 칩 교환
	ResItemGambleExchange,	// 갬블
	ResEventParentDay,		// 어버이날 이벤트
	ResEventSummer,			// 여름 이벤트
	ResItemJewelExchange,	// 보석 교환
	ResItemSelfRefine,		// 셀프 정제
	ResStashList,			// 창고 아이템 리스트
	ResStashInfo,			// 창고 정보
	ResStashTo,				// 창고에 아이템 추가
	ResStashFrom,			// 창고에서 아이템 찾기
	ResStashPut,			// 창고 다수 아이템 넣기
	ResStashGet,			// 창고 다수 아이템 찾기
	ResStashPuts,			// 창고 다수 아이템 넣기(라그러시)
	ResStashGets,			// 창고 다수 아이템 찾기(라그러시)
	ResExchangeMulti,		// 교환창 다수 아이템 넣기/빼기	
	ResCharSkillLevel,		// 스킬 레벨
	ResCastleBattleTime,	// 공성 시간 정보
	ResCastleEnergyUp,		// 에너지볼 투입
	ResPetComposite,		// 펫 조합
	ResPetChangePiece,		// 펫 피스 변경
	ResPetBuyEgg,			// 펫 알 구입
	ResPetLevelUp,			// 펫 레벨 업
	ResPetKeep,				// 펫 보관소
	ResPetSummon,			// 펫 소환
	ResPetShop,				// 펫 아이템 구입
	ResPetDrop,				// 펫 드롭
	ResPetPeragonDrop,		// 펫 페라곤 (성숙한 알 포데기)
	ResWarInfo,				// 팀배틀 정보
	ResWarRequest,			// 팀배틀 신청
	ResWarHead,				// 팀배틀 대장 정보
	ResWarCancel,			// 팀배틀 취소
	ResGpkRequest,			// 길드전 신청
	ResGpkSurrender,		// 길드전 항복
	ResItemBraceletUpgrade,	// 팔찌 업그레이드
	ResItemNecklaceUpgrade,	// 목걸이 업그레이드
	ResSupportRequest,		// 후견인 요청
	ResSupportAccept,		// 후견인 수락/거절
	ResSupportDisband,		// 후견인 해지
	ResMinimap,				// 미니맵 지시 포인트
	ResQuestSpecial,		// 특별 퀘스트
	ResQuestExchange,		// 퀘스트 돈 교환
	ResItemRingCombine,		// 반지 결합
	ResItemNpcUpgrade,		// NPC를 통한 아이템 강화
	ResItemEndurance,		// 아이템 내구도 충전
	ResGuildFindPos,		// 길드 위치 찾기
	ResUsershopSearch,		// 개인상점 검색
	ResUsershopRegistry,	// 개인상점 아이템 등록
	ResUsershopUnregistry,	// 개인상점 아이템 등록 취소
	ResUsershopModify,		// 개인상점 아이템 수량 수정 요청
	ResUsershopStart,		// 개인상점 시작
	ResUsershopBuy,			// 개인상점 아이템 구매
	ResUsershopOpen,		// 개인상점 열어 보기
	ResUsershopList,		// 개인상점 아이템 리스트
	ResUsershopSell,		// 개인상점 판매 알림
	ResUsershopLeave,		// 개인상점 떠나기
	ResUsershopClose,		// 개인상점 닫기(끝내기)
	ResUsershopRecall,		// 개인상점 회수
	ResItemRepair,			// 아이템 내구도 수리
	ResItemEnchant,			// 아이템 인챈트
	ResMinorDate,			// 마이너 공성 일자
	ResGuildCreateTest,		// 길드 생성 가능 테스트
	ResGuildCreate,			// 길드 생성
	ResGuildSearch,			// 길드 검색
	ResGuildDisband,		// 길드 해산
	ResGuildStashOpen,		// 길드 창고 열기
	ResGuildStashPut,		// 길드 창고 아이템 넣기
	ResGuildStashGet,		// 길드 창고 아이템 빼기
	ResGuildStashPutMulti,	// 길드 창고 아이템 넣기(다중)
	ResGuildStashGetMulti,	// 길드 창고 아이템 빼기(다중)
	ResGuildLeave,			// 길드 탈퇴
	ResGuildUpgrade,		// 길드 승급
	ResGuildRankwarInvite,	// 길드 랭킹전 초대
	ResGuildRankwarOpen,	// 길드 랭킹전 UI 열기
	ResGuildRankwarList,	// 길드 랭킹전 리스트
	ResGuildRankwarRequest,	// 길드 랭킹전 신청
	ResGuildRankwarCancel,	// 길드 랭킹전 취소
	ResGuildRankwarGet,		// 길드 랭킹전 아이템 지급
	ResGuildRankwarComp,	// 길드 랭킹전 아이템 합성
	ResGuildInvasionRequest,	// 길드 침략전 신청
	ResGuildInvasionCancelReq,	// 길드 침략전 취소 요청
	ResGuildInvasionCancelRes,	// 길드 침략전 취소 응답
	ResGuildRecommandReq,		// 길드 추천 가입
	ResGuildRecommandRes,		// 길드 추천 가입 응답
	ResGuildRecommandInfo,		// 길드 추천 가입 정보
	ResGuildJoinReq,			// 길드 일반 가입 요청
	ResGuildJoinRes,			// 길드 일반 가입 응답
	ResGuildNick,				// 길드 계급명 변경
	ResGuildNotice,				// 길드 공지
	ResGuildAppoint,			// 길드 직책 관리
	ResGuildFire,				// 길드원 강퇴
	ResWantedRegistTest,		// 현상수배 등록 체크
	ResWantedRegist,			// 현상수배 등록
	ResWantedMyList,			// 내가 등록한 현상수배 리스트
	ResWantedMyCancel,			// 내가 등록한 현상수배 리스트에서 제거
	ResWantedList,				// 현상수배 리스트
	ResWantedReward,			// 현상수배 보상
	ResFreepvpReq,				// 자유대련 신청
	ResFreepvpRes,				// 자유대련 신청 응답
	ResOccpTrans,				// 점령전 이동
	ResOccpTransPortal,			// 점령전 포탈 이동
	ResOccpTransGate,			// 점령전 게이트 이동
	ResOccpList,				// 점령전 현황 리스트
	ResOccpBattleReq,			// 점령전 요청
	ResOccpBattleInfo,			// 점령전 신청 정보
	ResOccpBattleCancel,		// 점령전 요청 취소
	ResOccpCashOpen,			// 점령전 세금 창 열기
	ResBossraidLicense,			// 최종 보스존 입장권
	ResEventGiftList,			// 선물 이벤트 리스트
	ResEventGiftGet,			// 선물 이벤트 받기
	ResGuildAttendCheck,		// 길드 출석 체크
	ResLagrushBuy,				// 라그러시 전용 아이템 구매
	ResItemSeparate,			// 아이템 분해
	ResItemMakes,				// 아이템 재작
	ResYutThrow,				// 윷던지기
	ResCharMultiShot,			// 휴먼 연사 및 저격
	ResShopMultiBuy,			// 상점 구매
	ResPetResult,				// 펫 처리 결과
	ResItemUse,					// 아이템 사용
	ResProcessReq,				// 가공 요청
	ResProcessCancel,			// 가공 취소
	ResProcessAccept,			// 가공 요청 수락
	ResProcessMoney,			// 가공 돈 변경
	ResProcessInvenTo,			// 가공 아이템 넣기
	ResProcessInvenFrom,		// 가공 아이템 빼기
	ResProcessResult,			// 가공 결과
	ResItemMix,					// 무기 조합
	ResRefineReq,				// 정제 요청
	ResRefineCancel,			// 정제 취소
	ResRefineAccept,			// 정제 요청 수락
	ResRefineMoney,				// 정제 돈 변경
	ResRefineInvenTo,			// 정제 아이템 넣기
	ResRefineInvenFrom,			// 정제 아이템 빼기
	ResRefineResult,			// 가공 결과
	ResItemUsePremiumItemChar,	// 캐릭터명 변경
	ResItemUsePremiumItemGuild,	// 길드명 변경
	ResPremiumCharMoveStep1,	// 프리미엄 캐릭터 이동 스텝 1
	ResPremiumCharMoveStep2,	// 프리미엄 캐릭터 이동 스텝 2
	ResPremiumCharMoveStep4,	// 프리미엄 캐릭터 이동 스텝 4
	ResPremiumCharMoveStop,		// 프리미엄 캐릭터 이동 스톱
	ResPack_Max,
};

enum UpdatePacket
{
	UpdUIClose = ResPack_Max,	// UI 강제 닫기
	UpdSetItemStatus,			// 셋트 아이템 상태 정보
	UpdBingoInfo,				// 빙고 이벤트 정보
	UpdExtraDel,				// 마우스 위치에 있는 아이템 삭제
	UpdGetItemCaru,				// 카루 에게 아이템 지급
	UpdNewNoteMessage,			// 새로운 쪽지 알림 표시 ON / OFF
	UpdNoteMessageData,			// 쪽지 데이터
	UpdNoteMessageContent,		// 쪽지 내용
	UpdGetBaseBallGiftList,		// 야구 선물 수령 가능 내역
	UpdBaseBallInfoList,		// 야구 진행 상황 정보
	UpdBossUseSkill,
	UpdStopLagrush,
	UpdSnakeDice,				// 뱀 주사위 상태
	UpdSnakePresentList,		// 뱀 주사위 보상 리스트
	
	UpdFirmPassword,			// 개인정보 변경 확인
	UpdCharExist,			// 캐릭터 선택창 캐릭터 정보
	UpdUserExpired,			// 계정 만료
	UpdCharStatus,			// 캐릭터 스탯값
	UpdCharSkillLevel,		// 캐릭터 스킬 레벨 (단일)
	UpdCharSkillLevelAll,	// 캐릭터 스킬 레벨 (전체)
	UpdMyCharIndex,			// 내 캐릭터 인덱스
	UpdMyCharName,			// 내 캐릭터 이름
	UpdMyCharAdminLevel,	// 내 캐릭터 운영자 레벨
	UpdMyCharWearing,		// 내 캐릭터 장비 착용정보
	UpdMyCharEvWearing,		// 내 캐릭터 코스튬 착용정보
	UpdMyCharEvWearTime,	// 내 캐릭터 코스튬 시간 만료 정보
	UpdCharWear,			// 캐릭터 장비 착용
	UpdCharEvWear,			// 캐릭터 코스튬 착용
	UpdCharEvShowFlag,		// 코스튬 보기/안보기
	UpdMyCharInven,			// 내 캐릭터 인벤
	UpdMyCharExtraInven,	// 내 캐릭터 엑스트라 인벤
	UpdMyCharQuickSlot,		// 내 캐릭터 퀵슬롯 정보
	UpdMyCharLaghaimPoint,	// 라그하임 포인트(LP)
	UpdServerNumber,		// 서버 번호
	UpdMapAttribute,		// 맵 속성 변경
	UpdCharAt,				// 캐릭터 처음 시작 위치
	UpdMapHour,				// 맵 시간
	UpdMapWeather,			// 맵 날씨
	UpdMyCharLocateLP,		// 결제유형 및 획득 LP
	UpdMyCharFreeUser,		// 무료이벤트 유저
	UpdMyCharPrizeAlram,	// 선물 알람
	UpdInfoMessage,			// 공지 메시지 출력
	UpdCharMoveSpeed,		// 캐릭터 이동 속도
	UpdChannelNumber,		// 채널 번호
	UpdEventFeverTime,		// 피버타임
	UpdCharWalk,			// 걷기
	UpdCharPlace,			// 걷기중
	UpdCharStop,			// 걷기 중단
	UpdCharSkill,			// 스킬 발동
	UpdCharPkState,			// 캐릭터 PK 선언 상태
	UpdPetStatus,			// 펫 스탯
	UpdLagrushEnable,		// 라그러시 사용 가능
	UpdLagrushRemainTime,	// 라그러시 남은 시간
	UpdHeroRewardEnable,	// 영웅 보상 지급 가능 여부
	UpdHeroInfoMessage,		// 영웅 정보 메시지
	UpdPartyList,			// 파티 리스트
	UpdPartyTypeChange,		// 파티 분배 방식 변경
	UpdPartyVital,			// 파티원 체력
	UpdCharAttack,			// 공격
	UpdCharAttackVital,		// 공격 후 체력
	UpdCharSubAttack,		// 보조아이템 공격
	UpdCharSubAttackVital,	// 보조아이템 공격 후 체력
	UpdCharSky,				// 비행석 레벨
	UpdGuildHuntDoor,		// 길드 헌트 문 열림/닫침
	UpdCharSocialAction,	// 소셜 행동
	UpdItemDrop,			// 아이템 드롭
	UpdItemPick,			// 아이템 줍기
	UpdCharChangeSex,		// 성별 변경
	UpdCharChangeHair,		// 머리 변경
	UpdCharLevelUp,			// 레벨업 이펙트
	UpdCharCombatState,		// 캐릭터 컴뱃 상태
	UpdCharRevive,			// 캐릭터 부활
	UpdChatSales,			// 장사채널 채팅
	UpdCastleTaxInfo,		// 세금 정보
	UpdPetSocial,			// 펫 소셜 행동
	UpdWarChar,				// 팀배틀 멤버 받기
	UpdWarPoint,			// 팀배틀 포인트 정보
	UpdWarNotify,			// 팀배틀 알림 정보
	UpdGpkStart,			// 길드전 시작
	UpdGpkEnd,				// 길드전 끝
	UpdGpkPlay,				// 길드전 정보
	UpdGpkStop,				// 길드전 종료 정보
	UpdSupportSuccessFail,	// 후견인 성공/실패
	UpdSupportEnable,		// 후견인 가능 정보
	UpdSupportInfo,			// 후견인 정보
	UpdSupportConnect,		// 후견인 접속 정보
	UpdQuestGet,			// 퀘스트 습득
	UpdQuestEnd,			// 퀘스트 종료
	UpdQuestKing,			// 퀘스트 보스 체크
	UpdQuestKill,			// 퀘스트 몹 죽인 숫자
	UpdQuestQuick,			// 퀘스트 남은 개수
	UpdQuestFail,			// 퀘스트 실패
	UpdQuestEffect,			// 퀘스트 이펙트 출현
	UpdQuestTotalPoint,		// 퀘스트 총 포인트
	UpdQuestPoint,			// 퀘스트 포인트
	UpdQuestNext,			// 다음 퀘스트로
	UpdQuestBonus,			// 퀘스트 보상 배수 표시
	UpdPrize,				// 선물 정보
	UpdUsershopCharIn,		// 개인상점 캐릭터 맵 등장
	UpdUsershopCharOut,		// 개인상점 캐릭터 맵 아웃
	UpdUsershopNpcIn,		// 개인상점 NPC 맵 등장
	UpdUsershopNpcOut,		// 개인상점 NPC 맵 아웃
	UpdCharRotate,			// 캐릭터 회전
	UpdSoccerTime,			// 월드컵 시간
	UpdSoccerSocre,			// 월드컵 스코어
	UpdSoccerName,			// 월드컵 길드이름
	UpdCharUseEffect,		// 이펙트 사용
	UpdMinorGuildName,		// 마이너 공성 길드 이름
	UpdGuildRankwarInfo,	// 길드 랭킹전 정보
	UpdGuildRankwarReady,	// 길드 랭킹전 준비
	UpdGuildRankwarCountdown,	// 길드 랭킹전 카운트다운
	UpdGuildRankwarTime,	// 길드 랭킹전 시간
	UpdGuildRankwarKillCount,	// 길드 랭킹전 킬 카운트
	UpdGuildRankwarStart,	// 길드 랭킹전 시작
	UpdGuildRankwarEnd,		// 길드 랭킹전 종료
	UpdGuildRankwarGoworld,	// 길드 랭킹전 존 이동
	UpdGuildInvasionInfo,	// 길드 침략전 정보
	UpdGuildNotice,			// 길드 공지
	UpdGuildFire,			// 길드원 강퇴
	UpdGuildInfo,			// 길드 정보
	UpdGuildMemberInfo,		// 길드원 정보
	UpdGuildWarning,		// 길드 침략 정보 알림
	UpdGuildMsg,			// 길드 관련 출력 메시지
	UpdWantedMark,			// 현상수배 마크
	UpdFreepvpRing,			// 자유대련 링생성
	UpdFreepvpStart,		// 자유대련 시작
	UpdFreepvpEnd,			// 자유대련 종료
	UpdFreepvpResult,		// 자유대련 결과
	UpdFreepvpIn,			// 자유대련 캐릭터 등장
	UpdFreepvpVital,		// 자유대련 HP
	UpdFreepvpRecord,		// 자유대련 승패 기록
	UpdFreepvpError,		// 자유대련 오류 정보
	UpdOccpBattleStart,		// 점령전 시작
	UpdOccpBattleEnd,		// 점령전 종료
	UpdOccpBattleFail,		// 점령전 요청 오류
	UpdOccpInfo,			// 점령전 점령 정보
	UpdOccpBuff,			// 점령전 버프
	UpdOccpCount,			// 점령전 전투 정보
	UpdFriendReq,			// 친구 추가 요청
	UpdFriendAdd,			// 친구 추가
	UpdFriendDel,			// 친구 삭제
	UpdFriendRefuseAll,		// 친구 거부(전체)
	UpdFriendRefuse,		// 친구 거부
	UpdFriendList,			// 친구 목록
	UpdFriendRefuseList,	// 친구 거부 목록
	UpdFriendLogin,			// 친구 로그인
	UpdFriendLogout,		// 친구 로그아웃
	UpdFriendZone,			// 친구 존 이동
	UpdYutInfo,				// 윷던지기 정보
	UpdMapInChar,			// 캐릭터 등장
	UpdMapInNpc,			// NPC 등장
	UpdMapInPet,			// 펫 등장
	UpdMapInItem,			// 아이템 등장
	UpdMapOut,				// 아웃
	UpdMyItemRemoveAll,		// 내 인벤 모두 지우기
	UpdCharKill,			// 캐릭터 죽는 모션
	UpdCharWearRemove,		// 캐릭터 장착 아이템 제거
	UpdCharEvWearRemove,	// 캐릭터 장착 코스튬 제거
	UpdNotice,				// 공지 사항
	UpdEffect,				// 이펙트/모션 출력
	UpdGuildName,			// 길드 이름
	UpdGuildGrade,			// 길드 계급
	UpdCharMultiShot,		// 휴먼 연사 및 저격
	UpdEvent,				// 이벤트 효과
	UpdYouAre,				// 운영자 레벨 정보
	UpdMapOpenCastle,		// 성문 열기
	UpdMapCloseCastle,		// 성문 닫기
	UpdMapOpenDoor,			// 문 열기
	UpdMapBattleOn,			// 공성전 상황
	UpdMapBattleCount,		// 공성전 남은 시간
	UpdMapBattleCrystal,	// 수정 떨어짐
	UpdMapBattleEnd,		// 공성전 종료
	UpdMapBattleInfo,		// 공성전 수정 소유 길드 정보
	UpdPopupMsg,			// 팝업 메시지
	UpdPopupMsg2,			// 팝업 메시지(클라 스트링 번호)
	UpdGuildLevel,			// 길드 레벨
	UpdPetDel,				// 펫 삭제
	UpdPetClear,			// 펫 교환
	UpdMyPet,				// 나의 펫 정보
	UpdCharCondition,		// 캐릭터 상태 변경
	UpdNpcConDistruction,	// 몹 계열 파괴
	UpdCharSubEffect,		// 서브 아이템 효과
	UpdCharSubEffectMe,		// 서브 아이템 효과 나에게 걸기
	UpdCharSummon,			// 소환수 소환
	UpdCharChao,			// 카오 등급
	UpdCharJump,			// 존 이동 좌표 동기화
	UpdCharMultiAttack,		// 캐릭터 멀티 어택
	UpdEventNewYear,		// 새해 이벤트 시작
	UpdEventAncient,		// 이벤트 처리
	UpdEventArborDay,		// 식목일 이벤트
	UpdItemTeleport,		// 지정 전송 장치
	UpdItemCandy,			// 왕사탕
	UpdItemApple,			// 왕사과
	UpdItemStarCandy,		// 별사탕
	UpdCharAttackMiss,		// 공격 미스
	UpdCharAttackBlock,		// 공격 블락
	UpdEventPointCheck,		// 이벤트 포인트 체크
	UpdCharPkItemDrop,		// PK 아이템 드롭
	UpdEffectFirework,		// 폭죽 이펙트
	UpdCharDStone,			// 디바인스톤
	UpdMapTimeShopOpen,		// 화이트혼 던전 중앙광장 타임 샵	
	UpdCharCombo,			// 콤보 출력
	UpdItemExtraInfoTime,	// 아이템 추가 정보 (남은시간)
	UpdItemExtraInfoDel,	// 아이템 추가 정보 (시간만료)
	UpdItemExtraInfoPrice,	// 아이템 추가 정보 (특별지정 가격)
	UpdItemExtraInfoAll,	// 아이템 추가 정보 (시간제 및 특별지정 가격)
	UpdCharSummonSquad,		// Summon Squad
	UpdMyCharUpdateItem,	// 아이템 갱신
	UpdCharMovingSkill,		// 이동하며 쓰는 스킬
	UpdNpcWear,				// NPC 의상 입히기
	UpdBpShopList,			// Bp 상점 리스트
	UpdPetEffect,			// 펫 이펙트
	UpdSpiritStart,			// 스피릿 시작
	UpdSpiritPoint,			// 스피릿 포인트
	UpdSpiritTime,			// 스피릿 남은 시간
	UpdSpiritEnd,			// 스피릿 모드 종료
	UpdMapChangeAttribute,	// 맵 속성 변경
	UpdEventAction,			// 이벤트 특수 연출
	UpdSkillCooltime,		// 스킬 쿨타임 제어
	UpdCheckJoin,			// 다중 접속 알림
	UpdBossraidAttack,		// 보스 레이드 어택
	UpdBossraidKillFlag,	// 보스 레이드 킬 플래그
	UpdBossraidRankList,	// 보스 레이드 공헌도 순위
	UpdLpShopList,			// Lp 상점 리스트
	UpdCharFullsetFlag,		// 세트 아이템 효과 적용
	UpdInvenInit,			// 인벤 초기화
	UpdMyCharRemoveItem,	// 아이템 삭제
	UpdSupportRewardListApprentice,	// 견습생 보상 리스트	
	UpdItemBuff,			// 아이템 버프
	UpdSkillContinue,		// 유지 스킬
	UpdCharSize,			// 캐릭터 사이즈 변경
	UpdNpcVisible,			// 캐릭터 몹 보이기
	UpdChatNormal,			// 일반 채팅
	UpdChatTell,			// 귓말 채팅
	UpdChatGuild,			// 길드 채팅
	UpdChatParty,			// 파티 채팅

	UpdPack_Max,
};

enum NotifyPacket
{
	NtfMegaPhone = UpdPack_Max, // 확성기
};

#endif // _PACKET_ENUM_H_
