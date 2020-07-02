#include "nspell.h"
#include "nspell_aura.h"
#include "nunit.h"
#include "nmap.h"
#include "nlog_mt.h"
#include "ninfo_data.h"

namespace nexus {

	nspell::nspell_effect_handler nspell::m_spell_effect_handler[ESET_Max] =
	{
		&nspell::_handle_null,										// 0,	ESET_Null,					��Ч��
		&nspell::_handle_trigger_spell,								// 1,	ESET_TriggerSpell,			��������
		&nspell::_handle_apply_aura,								// 2,	ESET_ApplyAura,				Ӧ��buff��չ����Ч��
		&nspell::_handle_instant_kill,								// 3,	ESET_Instantkill,			��ɱ
		&nspell::_handle_melee_damage,								// 4,	ESET_MeleeDamage,			��ͨ�˺�
		&nspell::_handle_kungfu_damage,								// 5,	ESET_KungFuDamage,			�����˺�
		&nspell::_handle_spell_damage,								// 6,	ESET_SpellDamage,			�����˺�
		&nspell::_handle_heal,										// 7,	ESET_Heal,					ֱ������
		&nspell::_handle_mod_power,									// 8,	ESET_ModPower,				�޸�����ֵ, (����, ��ʶ)
		&nspell::_handle_heal_pct,									// 9,	ESET_HealPct				���ٷֱ�ֱ������
		&nspell::_handle_mod_power_pct,								// 10,	ESET_ModPowerPct,			���ٷֱ��޸�����ֵ, (����, ��ʶ)
		&nspell::_handle_dispel,									// 11,	ESET_Dispel,				��ɢ
		&nspell::_handle_resurrect,									// 12,	ESET_Resurrect,				����
		&nspell::_handle_knock_down,								// 13,	ESET_KnockDown,				����
		&nspell::_handle_knock_back,								// 14,	ESET_KnockBack,				���˻���
	};

	void nspell::_handle_trigger_spell(uint32 index)
	{
		m_caster_ptr->cast_spell(get_spell_id(), m_target_id, m_target_pos, 0.0f, 0, m_caster_item_id, m_original_caster_id, m_spell_info_ptr, NULL);
	}

	void nspell::_handle_apply_aura(uint32 index)
	{
		if (!m_unit_target)
		{
			return;
		}

		nspell_aura_info const* aura_info_ptr = s_spell_aura_info.get_info(m_effect_info_ptr[index]->apply_aura_id.name_crc);

		if (!aura_info_ptr)
		{
			NLOG_DEBUG(_T("s_spell_aura_info.get_info error, id = %s"), m_effect_info_ptr[index]->apply_aura_id.name_str.c_str());
			return;
	}

		naura* aura_ptr = naura::construct(m_unit_target, m_caster_ptr->get_id(), aura_info_ptr, m_spell_info_ptr, index);
		m_unit_target->add_aura(aura_ptr);
	}

	void nspell::_handle_instant_kill(uint32 index)
	{
		m_damage += m_unit_target->get_data().get_uint32_value(EUF_UnitMaxHealth);
	}

	void nspell::_handle_melee_damage(uint32 index)
	{
		m_damage += m_caster_ptr->calculate_melee_damage(m_unit_target, m_hit_info);
	}

	void nspell::_handle_kungfu_damage(uint32 index)
	{
		m_damage += m_caster_ptr->calculate_kungfu_damage(m_unit_target, m_hit_info, m_effect_info_ptr[index]->value1, m_effect_info_ptr[index]->value2);
	}

	void nspell::_handle_spell_damage(uint32 index)
	{
		m_damage += m_caster_ptr->calculate_spell_damage(m_unit_target, m_hit_info, m_effect_info_ptr[index]->value1, m_effect_info_ptr[index]->value2);
	}

	void nspell::_handle_heal(uint32 index)
	{

	}

	void nspell::_handle_mod_power(uint32 index)
	{

	}

	void nspell::_handle_heal_pct(uint32 index)
	{

	}

	void nspell::_handle_mod_power_pct(uint32 index)
	{

	}

	void nspell::_handle_dispel(uint32 index)
	{

	}

	void nspell::_handle_resurrect(uint32 index)
	{

	}

	void nspell::_handle_knock_down(uint32 index)
	{

	}

	void nspell::_handle_knock_back(uint32 index)
	{

	}

} // namespace nexus
