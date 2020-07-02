#pragma once
#include "nmsg_base_id.h"
#include "nmsg_char.h"
#include "nmsg_chat.h"
#include "nmsg_gm_command.h"
#include "nmsg_login.h"
#include "nmsg_map.h"
#include "nmsg_movement.h"
#include "nmsg_loading.h"
#include "nmsg_spell.h"
#include "nmsg_stat.h"

using namespace System;

namespace NexusEngine
{
	//public enum class NetMessageSection : System::UInt16
	//{
	//	Login		= nexus::Msg_Client_Login,
	//	Char		= nexus::Msg_Client_Char,	
	//	Map			= nexus::Msg_Client_Map,
	//	Chat		= nexus::Msg_Client_Chat,	
	//	Movement	= nexus::Msg_Client_Movement,
	//	GM			= nexus::Msg_Client_GM,	
	//};

	public enum class NetErrorCode : System::Int32
	{
		Success			= nexus::EError_Success,
		SystemError		= nexus::EError_SystemError,
	};

	public enum class LoginConst : System::Int32
	{
		MaxAccountLen	= nexus::MAX_ACCOUNT_LEN,
		MaxPasswordLen	= nexus::MAX_PASSWORD_LEN,
		MaxMibaoLen		= nexus::MAX_MIBAO_LEN,
		MaxMD5Size		= nexus::MAX_MD5_SIZE
	};

	public enum class LoginErrorCode : System::Int32
	{
		No_Match			= nexus::ELoginProof_No_Match,			//���ʺŷǷ�
		In_Use				= nexus::ELoginProof_In_Use,			//���ʺŵ�ǰ�Ѿ���ʹ��
		Freezed				= nexus::ELoginProof_Freezed,			//���ʺű�ͣ��
		Wrong_Version		= nexus::ELoginProof_Wrong_Version,		//�汾�Ų���
		Disabled			= nexus::ELoginProof_Disabled,			//���ʺ��Ѿ�����ͣ
		Queue				= nexus::ELoginProof_Queue,				//��ʼ�Ŷ�	
		Psd_Error			= nexus::ELoginProof_Psd_Error,			//�������
		Mibao_Error			= nexus::ELoginProof_Mibao_Error,		//�ܱ�����
		Wrong_Proof_Type	= nexus::ELoginProof_Wrong_Proof_Type,	//�Ƿ���֤����
		Proofing			= nexus::ELoginProof_Proofing,			//������֤���Ѿ���֤���
		WorldName_Wrong		= nexus::ELoginProof_WorldName_Wrong,	//�Ƿ�����Ϸ������
		World_No_Use		= nexus::ELoginProof_World_No_Use,		//��Ϸ���粻����
		SUCCESS				= nexus::ELoginProof_SUCCESS,			//��֤�ɹ�
	};
	
	public enum class NetLogin : System::UInt16
	{
		CS_LoginProof		= nexus::C2S_LoginProof,
		SC_LoginProofResult	= nexus::S2C_LoginProofResult,
		CS_MiBao			= nexus::C2S_MiBao,
		SC_MiBao			= nexus::S2C_MiBao,
	};

	public enum class NetCharacter : System::UInt16
	{
		SC_CharError		= nexus::S2C_CharError,		
		CS_CharEnum			= nexus::C2S_CharEnum,
		SC_CharEnum			= nexus::S2C_CharEnum,
		CS_CharCreate		= nexus::C2S_CharCreate,
		SC_CharCreate		= nexus::S2C_CharCreate,
		CS_CharDelete		= nexus::C2S_CharDelete,
		SC_CharDelete		= nexus::S2C_CharDelete,
		CS_CharEnterGame	= nexus::C2S_CharEnterGame,
		SC_CharEnterGame	= nexus::S2C_CharEnterGame,
		CS_CharLeaveGame	= nexus::C2S_CharLeaveGame,
	};	

	public enum class NetMap : System::UInt16
	{
		SC_MapError			= nexus::S2C_MapError,	
		CS_MapEnter			= nexus::C2S_MapEnter,
		SC_MapEnter			= nexus::S2C_MapEnter,
		SC_TeleportTo		= nexus::S2C_TeleportTo,
		CS_TeleportAck		= nexus::C2S_TeleportAck,
	};

	// nexus::EMapError
	public enum class MapError : System::UInt16
	{
		MapInvalid		= nexus::EMR_MapInvalid,
	};

	public enum class NetChat : System::UInt16
	{
		SC_ChatError		= nexus::S2C_ChatError,	
		CS_ChatMessage		= nexus::C2S_ChatMessage,	
		SC_ChatMessage		= nexus::S2C_ChatMessage,	
	};	

	public enum class ChatType : System::UInt16
	{
		System					= nexus::ECT_System,				// ϵͳ
		Say						= nexus::ECT_Say,					// ��ǰ
		Party					= nexus::ECT_Party,					// ����
		Guild					= nexus::ECT_Guild,					// ����
		Whisper					= nexus::ECT_Whisper,				// ˽��
		Yell					= nexus::ECT_Yell,					// ��
		Emote					= nexus::ECT_Emote,					// ����
		MonsterSay				= nexus::ECT_MonsterSay,			// npc
		Channel					= nexus::ECT_Channel,				// �Զ���Ƶ��
		PartyLeader				= nexus::ECT_PartyLeader,			// С�Ӷӳ�
		BattleGround			= nexus::ECT_BattleGround,			// ս��
		BattleGroundLeader		= nexus::ECT_BattleGroundLeader,	// ս������
		BossEmote				= nexus::ECT_BossEmote,				// boss����
		BossWhisper				= nexus::ECT_BossWhisper,			// boss
		BG_SystemNeutral		= nexus::ECT_BG_SystemNeutral,		// ս��ϵͳ
		BG_SystemRed			= nexus::ECT_BG_SystemRed,			// ս��ϵͳ�췽
		BG_SystemBlue			= nexus::ECT_BG_SystemBlue,			// ս��ϵͳ����
		Map						= nexus::ECT_Map,					// ��ǰ��ͼ
		
		Client					= nexus::ECT_Client,				// �ͻ���������Ƶ����ʾ��
		Loot					= nexus::ECT_Loot,
		Money					= nexus::ECT_Money,
		Skill					= nexus::ECT_Skill,
	};

	public enum class ChatConstant : System::Byte
	{
		P_MAX_MSG_LEN			= nexus::MAX_MSG_LEN,
	};

	// ref nmsg_movement.h
	public enum class NetMovement : System::UInt16
	{
		SC_MoveError			= nexus::S2C_MoveError,		
		SC_PlayerEnterAoi		= nexus::S2C_PlayerEnterAoi,
		SC_CreatureEnterAoi		= nexus::S2C_CreatureEnterAoi,
		SC_GameObjectEnterAoi	= nexus::S2C_GameObjectEnterAoi,
		SC_ObjectLeaveAoi		= nexus::S2C_ObjectLeaveAoi,
		CS_Move					= nexus::C2S_Move,
		SC_Move					= nexus::S2C_Move,
		CS_CheckTime			= nexus::C2S_CheckTime,
		SC_CheckTime			= nexus::S2C_CheckTime,
	};	

	// nexus::EMoveError
	// ref nmsg_movement.h
	public enum class MoveError : System::UInt16
	{
		Success		= nexus::EMRC_Success,		// �ƶ��ɹ�
		CantArrive	= nexus::EMRC_CantArrive,	// ���ܵ���
		Invalid		= nexus::EMRC_Invalid,		// �Ƿ�����
		Fear		= nexus::EMRC_Fear,		// �־�״̬
		Root		= nexus::EMRC_Root,		// ����״̬
		Dead		= nexus::EMRC_Dead,		// ����״̬
		Casting		= nexus::EMRC_Casting,		// �ͷż���
	};

	public enum class NetGM : System::UInt16
	{
		CS_GMCommand			= nexus::C2S_GMCommand,			
		SC_GMCommandResult		= nexus::S2C_GMCommandResult,
	};	

	// ��ɫ�������ݼ�����Ϣ, id����[Msg_Client_Loading, Msg_Client_Loading + 50]
	// ref native nmsg_loading.h
	public enum class NetLoading : System::UInt16
	{
		SC_LoadingError		= nexus::S2C_LoadingError,	
		CS_GetInitStat		= nexus::C2S_GetInitStat,
		SC_GetInitStat		= nexus::S2C_GetInitStat,
		CS_GetInitSpell		= nexus::C2S_GetInitSpell,
		SC_GetInitSpell		= nexus::S2C_GetInitSpell,
		CS_GetInitAura		= nexus::C2S_GetInitAura,
		SC_GetInitAura		= nexus::S2C_GetInitAura,
	};	

	// ref native nmsg_spell.h
	// ����buff�����Ϣ id����[Msg_Client_Spell, Msg_Client_Spell + 50]
	public enum class NetSpell : System::UInt16
	{
		SC_SpellError		= nexus::S2C_SpellError,		
		CS_CastSpell		= nexus::C2S_CastSpell,
		SC_CastSpellResult	= nexus::S2C_CastSpellResult,
		CS_CancelCast		= nexus::C2S_CancelCast,
		SC_SpellPrepare		= nexus::S2C_SpellPrepare,
		SC_SpellCasting		= nexus::S2C_SpellCasting,
		SC_SpellFailed		= nexus::S2C_SpellFailed,
		
		SC_SpellHit			= nexus::S2C_SpellHit,
		CS_CancelAura		= nexus::C2S_CancelAura,
		SC_AddAura			= nexus::S2C_AddAura,
		SC_RemoveAura		= nexus::S2C_RemoveAura,
		SC_UpdateAura		= nexus::S2C_UpdateAura,
	};

	// ESpellCastResult
	// �����ͷŽ��ö��ֵ
	// ref native nmsg_spell.h
	public enum class SpellCastResult : System::Byte
	{
		Success					= nexus::ESCR_Success,
		Failed_NoSpell			= nexus::ESCR_Failed_NoSpell,			// �����ͷŵļ���û��ѧ��
		Failed_InvalidSpell		= nexus::ESCR_Failed_InvalidSpell,		// �����ͷŵļ��ܷǷ�(��������, ����id������Ч)
		Failed_SpellInProgress	= nexus::ESCR_Failed_SpellInProgress,	// �ͷ�������ִ����һ����
		Failed_CoolDown			= nexus::ESCR_Failed_CoolDown,			// ������ȴδ����
		Failed_TooClose			= nexus::ESCR_Failed_TooClose,			// ָ��Ŀ��̫Զ��
		Failed_OutofRange		= nexus::ESCR_Failed_OutofRange,		// ָ��Ŀ��̫����
		Failed_CasterDead		= nexus::ESCR_Failed_CasterDead,		// �ͷ����Ѿ�����
		Failed_Interrupted		= nexus::ESCR_Failed_Interrupted,		// ���ܱ����(������
		Failed_NoMana			= nexus::ESCR_Failed_NoMana,			// ħ��ֵ����
		Failed_NoEnergy			= nexus::ESCR_Failed_NoEnergy,			// ����ֵ����
		Failed_LineofSight		= nexus::ESCR_Failed_LineofSight,		// �����赲
		Failed_InvalidTargets	= nexus::ESCR_Failed_InvalidTargets,	// ָ��Ŀ��Ƿ�
		Failed_InCombat			= nexus::ESCR_Failed_InCombat,			// ����ս��״̬
		Failed_InFear			= nexus::ESCR_Failed_InFear,			// ���ڿ־�״̬
		Failed_InSilence		= nexus::ESCR_Failed_InSilence,			// ���ڳ�Ĭ״̬
		Failed_InRoot			= nexus::ESCR_Failed_InRoot,			// ���ڶ���״̬
		Failed_CantRepeat		= nexus::ESCR_Failed_CantRepeat,		// �ü��ܲ��������ͷ�
		Failed_Cancel			= nexus::ESCR_Failed_Cancel,			// ���ܱ����(����ȡ��)
		Failed_Move				= nexus::ESCR_Failed_Move,				// �ƶ��в����ͷ�
	};			

	// ref native nmsg_spell.h
	// �������н��ö��ֵ
	[System::FlagsAttribute]
	public enum class SpellHitResult : System::Byte
	{
		Hit		= nexus::ESHR_Hit,			// ����
		Crit	= nexus::ESHR_Crit,			// ����
		Miss	= nexus::ESHR_Miss,			// δ����
		Immune	= nexus::ESHR_Immune,		// ����
		Absorb	= nexus::ESHR_Absorb,		// ����
		Reflect	= nexus::ESHR_Reflect,		// ����
		Evade	= nexus::ESHR_Evade,		// ���
	};

	// ref native nmsg_spell.h
	public enum class SpellHitType : System::Byte
	{
		Spell	= nexus::ESHT_Spell,		// ����
		Aura	= nexus::ESHT_Aura,			// buff
	};

	// ���������Ϣ id����[Msg_Client_Stat, Msg_Client_Stat + 20]
	// ref native nmsg_stat.h
	public enum class NetStat : System::UInt16
	{
		SC_StatError		= nexus::S2C_StatError,
		SC_StatUpdate		= nexus::S2C_StatUpdate,
		SC_RemoteStatUpdate	= nexus::S2C_RemoteStatUpdate,
	};

}									