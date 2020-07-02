/*database nexus_game  create tables which is need*/

/*ɾ��nexus_game.item��Ʒ��*/
DROP TABLE IF EXISTS `item`;

/*����nexus_game.item��Ʒ��(size = 18)*/
CREATE TABLE `item` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'��ƷΨһGUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'��Ʒ����������',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ģ��ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ǰ�ѵ���Ŀ',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����е�λ��',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��������',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��־λ��Ϣ(�󶨣�����������(�ǽ������������))',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '����ɾ��ʱ��' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='��Ʒ��';

/*ɾ��nexus_game.equipװ����*/
DROP TABLE IF EXISTS `equip`;

/*����nexus_game.equipװ����((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'װ��ΨһGUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'װ������������',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ģ��ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ǰ�ѵ���Ŀ',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����е�λ��',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��������',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��־λ��Ϣ(�󶨣�����������(�ǽ������������))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'Ʒ����ǿ���ȼ�����һ��',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ���ȼ�',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'�;�',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ��',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ħID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'����ɾ��ʱ��',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',

PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='װ����';


/*ɾ��nexus_game.equipװ����*/
DROP TABLE IF EXISTS `equip_cmp`;

/*����nexus_game.equipװ����((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip_cmp` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'��ƷΨһGUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'װ������������',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ģ��ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ǰ�ѵ���Ŀ',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����е�λ��',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��������',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��־λ��Ϣ(�󶨣�����������(�ǽ������������))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'Ʒ����ǿ���ȼ�����һ��',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ���ȼ�',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'�;�',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ��',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ħID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'����ɾ��ʱ��',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='���ڲ���Ŀ�ĵ�װ����';


/*��������nexus_login.equip_cmp�ļ��insert�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `test_insert_equip`;

DELIMITER ;;
CREATE PROCEDURE `test_insert_equip`(
 IN guid BIGINT UNSIGNED, IN info_id MEDIUMINT UNSIGNED, IN count SMALLINT UNSIGNED,
 IN slot SMALLINT UNSIGNED, IN bag TINYINT UNSIGNED, IN flag TINYINT UNSIGNED,
 IN quality TINYINT UNSIGNED, IN level TINYINT UNSIGNED, IN durability MEDIUMINT UNSIGNED, 
 IN weight SMALLINT UNSIGNED, IN sharp SMALLINT UNSIGNED, IN solid SMALLINT UNSIGNED, IN tenacity TINYINT UNSIGNED,
 IN stat_type0 TINYINT UNSIGNED,IN stat_type1 TINYINT UNSIGNED,IN stat_type2 TINYINT UNSIGNED,IN stat_type3 TINYINT UNSIGNED,
 IN stat_type4 TINYINT UNSIGNED,IN stat_type5 TINYINT UNSIGNED,IN stat_type6 TINYINT UNSIGNED,IN stat_type7 TINYINT UNSIGNED,
 IN stat_value0 SMALLINT UNSIGNED,IN stat_value1 SMALLINT UNSIGNED,IN stat_value2 SMALLINT UNSIGNED,IN stat_value3 SMALLINT UNSIGNED,
 IN stat_value4 SMALLINT UNSIGNED,IN stat_value5 SMALLINT UNSIGNED,IN stat_value6 SMALLINT UNSIGNED,IN stat_value7 SMALLINT UNSIGNED,
 IN gem0 SMALLINT UNSIGNED,IN gem1 SMALLINT UNSIGNED,IN gem2 SMALLINT UNSIGNED,
 IN gem3 SMALLINT UNSIGNED,IN gem4 SMALLINT UNSIGNED,IN gem5 SMALLINT UNSIGNED,
 IN spell0 SMALLINT UNSIGNED,IN spell1 SMALLINT UNSIGNED,IN spell2 SMALLINT UNSIGNED,IN spell3 SMALLINT UNSIGNED,
 IN spell_rate0 SMALLINT UNSIGNED,IN spell1_rate SMALLINT UNSIGNED,IN spell_rate2 SMALLINT UNSIGNED,
 IN enchantment SMALLINT UNSIGNED,
 IN creator SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip_cmp`(guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator) 
					VALUES (	guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator);
	END;;
DELIMITER ;



/*ɾ��nexus_game.equipװ����*/
DROP TABLE IF EXISTS `equip_myisam`;

/*����nexus_game.equip_innodbװ����((size = 18(item) + 74(equip_extend))*/
CREATE TABLE `equip_myisam` (
`guid` BIGINT UNSIGNED ZEROFILL NOT NULL COMMENT'��ƷΨһGUID',
`owner_id` INT UNSIGNED ZEROFILL NOT NULL COMMENT'װ������������',
`info_id` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ģ��ID',
`count`  SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ǰ�ѵ���Ŀ',
`slot` SMALLINT(9) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����е�λ��',
`bag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��������',
`flag` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��־λ��Ϣ(�󶨣�����������(�ǽ������������))',

`quality` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'Ʒ����ǿ���ȼ�����һ��',
`level` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ���ȼ�',
`durability` MEDIUMINT(24) UNSIGNED ZEROFILL NOT NULL DEFAULT'0' COMMENT'�;�',
`weight` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`sharp` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`solid` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����',
`tenacity` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'ǿ��',

`stat_type0` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type1` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type2` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type3` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type4` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type5` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type6` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',
`stat_type7` TINYINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������������',

`stat_value0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value6` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',
`stat_value7` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'����������ֵ',

`gem0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem4` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',
`gem5` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��Ƕ�ı�ʯ',

`spell0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',
`spell3` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���Ӽ���',

`spell_rate0` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate1` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',
`spell_rate2` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'���ӱ������ܴ�������',

`enchantment` SMALLINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'��ħID',
`creator` BIGINT UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT'������ID',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'����ɾ��ʱ��',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',
PRIMARY KEY(`guid`),
UNIQUE KEY `guid`(`guid`),
INDEX `owner_id`(`owner_id`)
) ENGINE=MyISAM DEFAULT CHARACTER SET UTF8 COMMENT='ͨ��MyISAM����洢�����ں�Innodb�Ƚ�';


/*��������nexus_login.equip_cmp�ļ��insert�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `test_insert_equip_myisam`;

DELIMITER ;;
CREATE PROCEDURE `test_insert_equip_myisam`(
 IN guid BIGINT UNSIGNED, IN info_id MEDIUMINT UNSIGNED, IN count SMALLINT UNSIGNED,
 IN slot SMALLINT UNSIGNED, IN bag TINYINT UNSIGNED, IN flag TINYINT UNSIGNED,
 IN quality TINYINT UNSIGNED, IN level TINYINT UNSIGNED, IN durability MEDIUMINT UNSIGNED, 
 IN weight SMALLINT UNSIGNED, IN sharp SMALLINT UNSIGNED, IN solid SMALLINT UNSIGNED, IN tenacity TINYINT UNSIGNED,
 IN stat_type0 TINYINT UNSIGNED,IN stat_type1 TINYINT UNSIGNED,IN stat_type2 TINYINT UNSIGNED,IN stat_type3 TINYINT UNSIGNED,
 IN stat_type4 TINYINT UNSIGNED,IN stat_type5 TINYINT UNSIGNED,IN stat_type6 TINYINT UNSIGNED,IN stat_type7 TINYINT UNSIGNED,
 IN stat_value0 SMALLINT UNSIGNED,IN stat_value1 SMALLINT UNSIGNED,IN stat_value2 SMALLINT UNSIGNED,IN stat_value3 SMALLINT UNSIGNED,
 IN stat_value4 SMALLINT UNSIGNED,IN stat_value5 SMALLINT UNSIGNED,in stat_value6 SMALLINT UNSIGNED,IN stat_value7 SMALLINT UNSIGNED,
 IN gem0 SMALLINT UNSIGNED,IN gem1 SMALLINT UNSIGNED,IN gem2 SMALLINT UNSIGNED,
 IN gem3 SMALLINT UNSIGNED,IN gem4 SMALLINT UNSIGNED,IN gem5 SMALLINT UNSIGNED,
 IN spell0 SMALLINT UNSIGNED,IN spell1 SMALLINT UNSIGNED,IN spell2 SMALLINT UNSIGNED,IN spell3 SMALLINT UNSIGNED,
 IN spell_rate0 SMALLINT UNSIGNED,IN spell1_rate SMALLINT UNSIGNED,IN spell_rate2 SMALLINT UNSIGNED,
 IN enchantment SMALLINT UNSIGNED,
 IN creator SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip_myisam`(guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator) 
					VALUES (	guid,info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator);
	END;;
DELIMITER ;


/*ɾ��nexus_game.character��ɫ��Ϣ��*/
DROP TABLE IF EXISTS `character`;

/*����nexus_game.character��ɫ��Ϣ��*/
CREATE TABLE `character`(
`role_id` INT(11) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '��ɫID',
`account_id` INT(11) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '�˺�ID',
`name` VARCHAR(32) NOT NULL DEFAULT '' COMMENT '��ɫ����',
`sex` TINYINT(3) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '0:Ů 1:��',
`race` TINYINT(3) unsigned NOT NULL DEFAULT '0' COMMENT '����',
`class_type` TINYINT(3) unsigned NOT NULL DEFAULT '0'COMMENT 'ְҵ',

`map_id` SMALLINT(10) NOT NULL DEFAULT '-1' COMMENT '��ɫ���ڵ�ͼID',
`x` FLOAT(10) ZEROFILL NOT NULL COMMENT 'X����',
`y` FLOAT(10) ZEROFILL NOT NULL COMMENT 'y����',
`z` FLOAT(10) ZEROFILL NOT NULL  COMMENT 'z����',
`orient` FLOAT(10) ZEROFILL NOT NULL  COMMENT '����',

`hp` MEDIUMINT(8) ZEROFILL NOT NULL DEFAULT '0' COMMENT 'Ѫ��',
`mp` MEDIUMINT(8) ZEROFILL NOT NULL DEFAULT '0' COMMENT '������',
`level` TINYINT(3) NOT NULL DEFAULT '1' COMMENT '��ǰ�ȼ�',
`cur_exp` INT(10) unsigned NOT NULL DEFAULT '0' COMMENT '��ǰӵ�еľ���ֵ',

`create_time` INT(10) NOT NULL COMMENT '��ɫ����ʱ��',
`login_time` INT(10) NOT NULL DEFAULT '0' COMMENT '����ʱ��',
`logout_time` INT(10) NOT NULL DEFAULT '0' COMMENT '����ʱ��',
`online_time` INT(10) NOT NULL DEFAULT '0' COMMENT '������ʱ��(��λ:��)',
`cur_online_time` INT(10) NOT NULL DEFAULT '0' COMMENT '��ǰ����ʱ��(��λ:��)',

`remove_flag` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '�Ƿ���ɾ��״̬(��״̬��Ҳ��ᱻ������Ϸ����)',
`remove_time` INT(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT 'ɾ��ʱ��',

`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT'����ɾ��ʱ��',
`destroy` TINYINT ZEROFILL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',

PRIMARY KEY (`role_id`),
UNIQUE KEY `role_id`(`role_id`),
INDEX `account_id`(`account_id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT '��ɫ��Ϣ��(������Ϸ����)';


/*�������ڻ�ȡnexus_game.character��role_id���ֵ�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `get_role_max`;

DELIMITER ;;
CREATE PROCEDURE `get_role_max`()
	BEGIN
		SELECT MAX(role_id) FROM `character`;
	END;; 
DELIMITER ;

/*��������nexus_game.character��account_id�����ֵ�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `get_account_max`;

DELIMITER ;;
CREATE PROCEDURE `get_account_max`()
	BEGIN
		SELECT MAX(account_id) FROM `character`;
	END;; 
DELIMITER ;

/*��������nexus_game.character��character_role�����ݻ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `character_role`;

DELIMITER ;;
CREATE PROCEDURE `character_role`()
	BEGIN
		SELECT account_id, role_id FROM `character` WHERE `character`.`destroy`=0;
	END;; 
DELIMITER ;


/*��������nexus_game.character�����ݻ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `character_cache`;

DELIMITER ;;
CREATE PROCEDURE `character_cache`(IN role_idV INT UNSIGNED)
	BEGIN
		-- SELECT	[0]account_id,[1]role_id,[2]name,
		--			[3]sex,[4]race,[5]class_type,[6]level,[7]cur_exp,[8]hp,[9]mp,
		--			[10]map_id,[11]x,[12]y,[13]z,[14]orient,
		--			[15]create_time,[16]login_time,[17]logout_time,[18]online_time FROM `character` WHERE `character`.`role_id`=role_idV and `character`.`destroy`=0;
		SELECT  account_id,role_id,name,
				sex,race,class_type,level,cur_exp,hp,mp,
				map_id,x,y,z,orient,
				create_time,login_time,logout_time,online_time FROM `character` WHERE `character`.`role_id`=role_idV and `character`.`destroy`=0;
	END;;
DELIMITER ;

/*��������nexus_game.character�ĸ��´洢����*/
DROP PROCEDURE IF EXISTS `character_update`;

DELIMITER ;;
--	[1]role_id,[2]level,[3]cur_exp,[4]hp,[5]mp,
--	[6]map_id,[7]x,[8]y,[9]z,[10]orient,
--	[11]login_time,[12]logout_time,[13]online_time
CREATE PROCEDURE `character_update`(
IN role_idV INT UNSIGNED,
IN levelV TINYINT UNSIGNED, IN cur_expV INT UNSIGNED,IN hpV MEDIUMINT UNSIGNED, IN mpV MEDIUMINT UNSIGNED,
IN map_idV SMALLINT UNSIGNED,IN xV FLOAT, IN yV FLOAT,IN zV FLOAT, IN orientV FLOAT,
IN login_timeV INT, IN logout_timeV INT, IN online_timeV INT)
	BEGIN
		UPDATE `character` SET level=levelV,cur_exp=cur_expV,hp=hpV,mp=mpV,
						map_id=map_idV,x=xV,y=yV,z=zV,orient=orientV,
						login_time=login_timeV,logout_time=logout_timeV,online_time=online_timeV
				WHERE `character`.`role_id` = role_idV;
	END;;
DELIMITER ;

/*��������nexus_game.character�����Ӵ洢����*/
DROP PROCEDURE IF EXISTS `character_insert`;

DELIMITER ;;
--	[1]account_id,[2]role_id,
--	[3]name,[4]sex,[5]race,[6]class_type,
--	[7]level,[8]cur_exp,[9]hp,[10]mp,
--	[11]map_id,[12]x,[13]y,[14]z,[15]orient,
--	[16]create_time, [17]login_time,[18]logout_time,[19]online_time
CREATE PROCEDURE `character_insert`(
IN account_idV INT UNSIGNED,IN role_idV INT UNSIGNED, nameV VARCHAR(32),
IN sexV TINYINT UNSIGNED, raceV TINYINT UNSIGNED, class_typeV TINYINT UNSIGNED,
IN levelV TINYINT UNSIGNED, IN cur_expV INT UNSIGNED,IN hpV MEDIUMINT UNSIGNED, IN mpV MEDIUMINT UNSIGNED,
IN map_idV SMALLINT UNSIGNED,IN xV FLOAT, IN yV FLOAT,in zV FLOAT, IN orientV FLOAT,
IN create_timeV INT, IN login_timeV INT, IN logout_timeV INT, IN online_timeV INT)
	BEGIN
		INSERT INTO `character` (account_id, role_id, name, sex, race, class_type,
				level,cur_exp,hp,mp,
				map_id,x,y,z,orient,
				create_time,login_time,logout_time,online_time)
				VALUES (account_idV, role_idV, nameV, sexV, raceV, class_typeV,
				levelV,cur_expV,hpV,mpV,
				map_idV,xV,yV,xV,orientV,
				create_timeV,login_timeV,logout_timeV,online_timeV);
	END;;
DELIMITER ;

/*��������nexus_game.character���Ƴ���ɫ�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `character_delete`;

DELIMITER ;;
CREATE PROCEDURE `character_delete`(
IN role_idV INT UNSIGNED)
	BEGIN
		UPDATE `character` SET destroy_time=NOW(), destroy=1 WHERE `character`.`role_id`=role_idV;
		UPDATE `item` SET destroy_time=NOW(), destroy=1 WHERE `item`.`owner_id`=role_idV and `item`.`destroy`=0;
		UPDATE `equip` SET destroy_time=NOW(), destroy=1 WHERE `equip`.`owner_id`=role_idV and `equip`.`destroy`=0;
		UPDATE `skill` SET destroy_time=NOW(), destroy=1 WHERE `skill`.`role`=role_idV and `skill`.`destroy`=0;
	END;;
DELIMITER ;


/*��������nexus_game.item�����ݻ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `item_cache`;

DELIMITER ;;
CREATE PROCEDURE `item_cache`(IN owner_idV INT UNSIGNED)
	BEGIN
		-- SELECT [0]guid,[1]info_id,[2]count,[3]slot,[4]bag,[5]flag FROM `item` WHERE `item`.`owner_id`=owner_idV and `item`.`destroy`=0;
		SELECT guid,info_id,count,slot,bag,flag FROM `item` WHERE `item`.`owner_id`=owner_idV and `item`.`destroy`=0;
	END;;
DELIMITER ;

/*��������nexus_game.item�ĸ��µĴ洢����*/
DROP PROCEDURE IF EXISTS `item_update`;

DELIMITER ;;
--	[1]guid,[2]owner_id,
--	[3]info_id,[4]count,
--	[5]slot,[6]bag,[7]flag
CREATE PROCEDURE `item_update`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED
 )
	BEGIN
		UPDATE `item` SET info_id=info_idV,count=countV,slot=slotV,bag=bagV,flag=flagV WHERE `item`.`guid`=guidV;
	END;;
DELIMITER ;

/*��������nexus_game.item�����ӵĴ洢����*/
DROP PROCEDURE IF EXISTS `item_insert`;

DELIMITER ;;
--	[1]guid,[2]owner_id,
--	[3]info_id,[4]count,
--	[5]slot,[6]bag,[7]flag
CREATE PROCEDURE `item_insert`(
IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
IN info_idV INT UNSIGNED, IN countV SMALLINT UNSIGNED, IN slotV SMALLINT UNSIGNED,
IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED
)
	BEGIN
		INSERT INTO `item` (guid, owner_id, info_id, count, slot, bag, flag)
						VALUES (guidV,owner_idV,info_idV, countV, slotV, bagV, flagV);
	END;;
DELIMITER ;

/*��������nexus_game.item��ɾ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `item_delete`;

DELIMITER ;;
CREATE PROCEDURE `item_delete`(IN guidV BIGINT UNSIGNED)
	BEGIN
		UPDATE `item` SET destroy_time=NOW(), destroy=1 WHERE `item`.`guid`=guidV;
	END;;
DELIMITER ;


/*��������nexus_game.equip�����ݻ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `equip_cache`;

DELIMITER ;;
CREATE PROCEDURE `equip_cache`(IN owner_idV INT UNSIGNED)
	BEGIN
		-- SELECT	[0]guid,[1]info_id,[2]count,[3]slot,[4]bag,[5]flag,
		--			[6]quality,[7]level,[8]durability,[9]weight,[10]sharp,[11]solid,[12]tenacity,
		--			[13]stat_type0,[14]stat_type1,[15]stat_type2,[16]stat_type3,
		--			[17]stat_type4,[18]stat_type5,[19]stat_type6,[20]stat_type7,
		--			[21]stat_value0,[22]stat_value1,[23]stat_value2,[24]stat_value3,
		--			[25]stat_value4,[26]stat_value5,[27]stat_value6,[28]stat_value7,
		--			[29]gem0,[30]gem1,[31]gem2,[32]gem3,[33]gem4,[34]gem5,
		--			[35]spell0,[36]spell1,[37]spell2,[38]spell3,
		--			[39]spell_rate0,[40]spell_rate1,[41]spell_rate2,
		--			[42]enchantment,[43]creator FROM `equip` WHERE `equip`.`owner_id`=owner_idV and `equip`.`destroy`=0;
		SELECT		guid,info_id,count,slot,bag,flag,
					quality,level,durability,weight,sharp,solid,tenacity,
					stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
					stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
					gem0,gem1,gem2,gem3,gem4,gem5,
					spell0,spell1,spell2,spell3,
					spell_rate0,spell_rate1,spell_rate2,
					enchantment,creator FROM `equip` WHERE `equip`.`owner_id`=owner_idV and `equip`.`destroy`=0;
	END;;
DELIMITER ;

/*��������nexus_game.equip�ĸ��´洢����*/
DROP PROCEDURE IF EXISTS `equip_update`;

DELIMITER ;;
-- 	[1]guid,[2]owner_id
--  [3]info_id,[4]count,
--  [5]slot,[6]bag,[7]flag,
--	[8]quality,[9]level,[10]durability,
--  [11]weight,[12]sharp,[13]solid,[14]tenacity,
--	[15]stat_type0,[16]stat_type1,[17]stat_type2,[18]stat_type3,
--	[19]stat_type4,[20]stat_type5,[21]stat_type6,[22]stat_type7,
--	[23]stat_value0,[24]stat_value1,[25]stat_value2,[26]stat_value3,
--	[27]stat_value4,[28]stat_value5,[29]stat_value6,[30]stat_value7,
--	[31]gem0,[32]gem1,[33]gem2,
--  [34]gem3,[35]gem4,[36]gem5,
--	[37]spell0,[38]spell1,[39]spell2,[40]spell3,
--	[41]spell_rate0,[42]spell_rate1,[43]spell_rate2,
--	[44]enchantment,[45]creator
CREATE PROCEDURE `equip_update`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED,
 IN qualityV TINYINT UNSIGNED, IN levelV TINYINT UNSIGNED, IN durabilityV MEDIUMINT UNSIGNED, 
 IN weightV SMALLINT UNSIGNED, IN sharpV SMALLINT UNSIGNED, IN solidV SMALLINT UNSIGNED, IN tenacityV TINYINT UNSIGNED,
 IN stat_type0V TINYINT UNSIGNED,IN stat_type1V TINYINT UNSIGNED,IN stat_type2V TINYINT UNSIGNED,IN stat_type3V TINYINT UNSIGNED,
 IN stat_type4V TINYINT UNSIGNED,IN stat_type5V TINYINT UNSIGNED,IN stat_type6V TINYINT UNSIGNED,IN stat_type7V TINYINT UNSIGNED,
 IN stat_value0V SMALLINT UNSIGNED,IN stat_value1V SMALLINT UNSIGNED,IN stat_value2V SMALLINT UNSIGNED,IN stat_value3V SMALLINT UNSIGNED,
 IN stat_value4V SMALLINT UNSIGNED,IN stat_value5V SMALLINT UNSIGNED,IN stat_value6V SMALLINT UNSIGNED,IN stat_value7V SMALLINT UNSIGNED,
 IN gem0V SMALLINT UNSIGNED,IN gem1V SMALLINT UNSIGNED,IN gem2V SMALLINT UNSIGNED,
 IN gem3V SMALLINT UNSIGNED,IN gem4V SMALLINT UNSIGNED,IN gem5V SMALLINT UNSIGNED,
 IN spell0V SMALLINT UNSIGNED,IN spell1V SMALLINT UNSIGNED,IN spell2V SMALLINT UNSIGNED,IN spell3V SMALLINT UNSIGNED,
 IN spell_rate0V SMALLINT UNSIGNED,IN spell_rate1V SMALLINT UNSIGNED,IN spell_rate2V SMALLINT UNSIGNED,
 IN enchantmentV SMALLINT UNSIGNED,IN creatorV SMALLINT UNSIGNED
 )
	BEGIN
		UPDATE `equip` SET		info_id=info_idV,count=countV,slot=slotV,bag=bagV,flag=flagV,
								quality=qualityV,level=levelV,durability=durabilityV,
								weight=weightV,sharp=sharpV,solid=solidV,tenacity=tenacityV,
								stat_type0=stat_type0V,stat_type1=stat_type1V,stat_type2=stat_type2V,stat_type3=stat_type3V,
								stat_type4=stat_type4V,stat_type5=stat_type5V,stat_type6=stat_type6V,stat_type7=stat_type7V,
								stat_value0=stat_value0V,stat_value1=stat_value1V,stat_value2=stat_value2V,stat_value3=stat_value3V,
								stat_value4=stat_value4V,stat_value5=stat_value5V,stat_value6=stat_value6V,stat_value7=stat_value7V,
								gem0=gem0V,gem1=gem1V,gem2=gem2V,
								gem3=gem3V,gem4=gem4V,gem5=gem5V,
								spell0=spell0V,spell1=spell1V,
								spell2=spell2V,spell3=spell3V,
								spell_rate0=spell_rate0V,spell_rate1=spell_rate1V,spell_rate2=spell_rate2V,
								enchantment=enchantmentV,creator=creatorV WHERE `equip`.`guid`=guidV;
	END;;
DELIMITER ;


/*��������nexus_game.equip�����Ӵ洢����*/
DROP PROCEDURE IF EXISTS `equip_insert`;

DELIMITER ;;
-- 	[1]guid,[2]owner_id
--  [3]info_id,[4]count,
--  [5]slot,[6]bag,[7]flag,
--	[8]quality,[9]level,[10]durability,
--  [11]weight,[12]sharp,[13]solid,[14]tenacity,
--	[15]stat_type0,[16]stat_type1,[17]stat_type2,[18]stat_type3,
--	[19]stat_type4,[20]stat_type5,[21]stat_type6,[22]stat_type7,
--	[23]stat_value0,[24]stat_value1,[25]stat_value2,[26]stat_value3,
--	[27]stat_value4,[28]stat_value5,[29]stat_value6,[30]stat_value7,
--	[31]gem0,[32]gem1,[33]gem2,
--  [34]gem3,[35]gem4,[36]gem5,
--	[37]spell0,[38]spell1,[39]spell2,[40]spell3,
--	[41]spell_rate0,[42]spell_rate1,[43]spell_rate2,
--	[44]enchantment,[45]creator
CREATE PROCEDURE `equip_insert`(
 IN guidV BIGINT UNSIGNED, IN owner_idV INT UNSIGNED,
 IN info_idV MEDIUMINT UNSIGNED, IN countV SMALLINT UNSIGNED,
 IN slotV SMALLINT UNSIGNED, IN bagV TINYINT UNSIGNED, IN flagV TINYINT UNSIGNED,
 IN qualityV TINYINT UNSIGNED, IN levelV TINYINT UNSIGNED, IN durabilityV MEDIUMINT UNSIGNED, 
 IN weightV SMALLINT UNSIGNED, IN sharpV SMALLINT UNSIGNED, IN solidV SMALLINT UNSIGNED, IN tenacityV TINYINT UNSIGNED,
 IN stat_type0V TINYINT UNSIGNED,IN stat_type1V TINYINT UNSIGNED,IN stat_type2V TINYINT UNSIGNED,IN stat_type3V TINYINT UNSIGNED,
 IN stat_type4V TINYINT UNSIGNED,IN stat_type5V TINYINT UNSIGNED,IN stat_type6V TINYINT UNSIGNED,IN stat_type7V TINYINT UNSIGNED,
 IN stat_value0V SMALLINT UNSIGNED,IN stat_value1V SMALLINT UNSIGNED,IN stat_value2V SMALLINT UNSIGNED,IN stat_value3V SMALLINT UNSIGNED,
 IN stat_value4V SMALLINT UNSIGNED,IN stat_value5V SMALLINT UNSIGNED,IN stat_value6V SMALLINT UNSIGNED,IN stat_value7V SMALLINT UNSIGNED,
 IN gem0V SMALLINT UNSIGNED,IN gem1V SMALLINT UNSIGNED,in gem2V SMALLINT UNSIGNED,
 IN gem3V SMALLINT UNSIGNED,IN gem4V SMALLINT UNSIGNED,in gem5V SMALLINT UNSIGNED,
 IN spell0V SMALLINT UNSIGNED,IN spell1V SMALLINT UNSIGNED,in spell2V SMALLINT UNSIGNED,IN spell3V SMALLINT UNSIGNED,
 IN spell_rate0V SMALLINT UNSIGNED,IN spell_rate1V SMALLINT UNSIGNED,IN spell_rate2V SMALLINT UNSIGNED,
 IN enchantmentV SMALLINT UNSIGNED,IN creatorV SMALLINT UNSIGNED
 )
	BEGIN
		INSERT INTO `equip` ( guid,owner_id, info_id,count,slot,bag,flag,
								quality,level,durability,weight,sharp,solid,tenacity,
								stat_type0,stat_type1,stat_type2,stat_type3,stat_type4,stat_type5,stat_type6,stat_type7,
								stat_value0,stat_value1,stat_value2,stat_value3,stat_value4,stat_value5,stat_value6,stat_value7,
								gem0,gem1,gem2,gem3,gem4,gem5,
								spell0,spell1,
								spell2,spell3,
								spell_rate0,spell_rate1,spell_rate2,
								enchantment,creator )
								VALUES ( guidV,owner_idV,info_idV,countV,slotV,bagV,flagV,
								qualityV,levelV,durabilityV,weightV,sharpV,solidV,tenacityV,
								stat_type0V,stat_type1V,stat_type2V,stat_type3V,stat_type4V,stat_type5V,stat_type6V,stat_type7V,
								stat_value0V,stat_value1V,stat_value2V,stat_value3V,stat_value4V,stat_value5V,stat_value6V,stat_value7V,
								gem0V,gem1V,gem2V,gem3V,gem4V,gem5V,
								spell0V,spell1V,spell2V,spell3V,
								spell_rate0V,spell_rate1V,spell_rate2V,
								enchantmentV,creatorV );
	END;;
DELIMITER ;


/*��������nexus_game.equip��ɾ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `equip_delete`;

DELIMITER ;;
CREATE PROCEDURE `equip_delete`(IN guidV BIGINT UNSIGNED)
	BEGIN
		UPDATE `equip` SET destroy_time=NOW(), destroy=1 WHERE `equip`.`guid`=guidV;
	END;;
DELIMITER ;


/*ɾ��nexus_game.skill��ɫ���ܱ�*/
DROP TABLE IF EXISTS `skill`;

/*����nexus_game.skill��ɫ���ܱ�*/

CREATE TABLE `skill` (
`role` INT(10) UNSIGNED ZEROFILL NOT NULL DEFAULT '0' COMMENT '��ɫID',
`id` INT(10) UNSIGNED NOT NULL DEFAULT '0' COMMENT '����ģ��ID',
`cd` MEDIUMINT(11) UNSIGNED NOT NULL COMMENT '������ȴʱ��',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '����ɾ��ʱ��' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',
PRIMARY KEY (`role`, `id`),
UNIQUE KEY `role_id`(`role`,`id`)
) ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='��ɫ���ܱ�';

/*��������nexus_game.skill�����ݻ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `skill_cache`;

DELIMITER ;;
CREATE PROCEDURE `skill_cache`(IN roleV INT UNSIGNED)
	BEGIN
		-- SELECT [0]id,[1]cd FROM `skill` WHERE `skill`.`role`=roleV and `skill`.`destroy`=0;
		SELECT id,cd FROM `skill` WHERE `skill`.`role`=roleV and `skill`.`destroy`=0;
	END;;
DELIMITER ;

/*��������nexus_game.skill�ĸ��´洢����*/
DROP PROCEDURE IF EXISTS `skill_update`;

DELIMITER ;;
CREATE PROCEDURE `skill_update`(IN roleV INT UNSIGNED, IN idV INT UNSIGNED, IN cdV MEDIUMINT UNSIGNED)
	BEGIN
		UPDATE `skill` SET cd=cdV WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
	END;;
DELIMITER ;

/*��������nexus_game.skill�����ӵĴ洢����*/
DROP PROCEDURE IF EXISTS `skill_insert`;

DELIMITER ;;
CREATE PROCEDURE `skill_insert`(IN roleV INT UNSIGNED,IN idV INT UNSIGNED,IN cdV MEDIUMINT UNSIGNED)
	BEGIN
		DECLARE num INT UNSIGNED DEFAULT 0; 
		SELECT COUNT(*) INTO num FROM `skill` WHERE  `skill`.`role`=roleV and `skill`.`id`=idV and `skill`.`destroy`=1;
		IF num=1 THEN
			UPDATE `skill` SET destroy=0 WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
		ELSE
			INSERT INTO `skill` (role, id, cd) VALUES (roleV, idV, cdV);
		END IF;
	END;;
DELIMITER ;


/*��������nexus_game.skill��ɾ���Ĵ洢����*/
DROP PROCEDURE IF EXISTS `skill_delete`;

DELIMITER ;;
CREATE PROCEDURE `skill_delete`(IN roleV INT UNSIGNED, IN idV INT UNSIGNED)
	BEGIN
		UPDATE `skill` SET destroy_time=NOW(), destroy=1 WHERE `skill`.`role`=roleV and `skill`.`id`=idV;
	END;;
DELIMITER ;



/*ɾ��nexus_game.buff��ɫBUFF��*/
DROP TABLE IF EXISTS `buff`;

/*����nexus_game.buff��ɫBUFF��*/
CREATE TABLE `buff` (
`dest` INT(10) unsigned NOT NULL COMMENT '����Ŀ��ID',
`src` INT(10) unsigned NOT NULL COMMENT '����ԴID',
`skill_id` INT(10) unsigned NOT NULL COMMENT '������BUFF�ļ���ID',
`buff_id` INT(10) unsigned NOT NULL COMMENT 'Buff ID',
`levels` TINYINT(3) NOT NULL COMMENT '��ǰ���Ӵ���',
`leave_time` FLOAT(4)ZEROFILL DEFAULT '0' COMMENT 'ʣ��ʱ��',
`destroy_time` DATETIME DEFAULT '0000-00-00 00:00:00' COMMENT '����ɾ��ʱ��' ,
`destroy` TINYINT(3) ZEROFILL NOT NULL DEFAULT '0' COMMENT'�����Ƿ�ɾ��(0:����ɾ��״̬ 1:ɾ��״̬)',
PRIMARY KEY  (`dest`, `buff_id`)
)ENGINE=InnoDB DEFAULT CHARACTER SET UTF8 COMMENT='��ɫbuff��';



