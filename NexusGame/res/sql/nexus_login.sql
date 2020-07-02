/*database nexus_login  create tables which is need*/

/*ɾ��nexus_login.account�˺ű�*/
DROP TABLE IF EXISTS `account`;

/*����nexus_login.account�˺ű�*/
CREATE TABLE `account` (
`id` INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT COMMENT '�˺�ID',
`name` CHAR(32) CHARACTER SET UTF8 NOT NULL COMMENT '�˺�����',
`psd`  CHAR(16) CHARACTER SET UTF8 NOT NULL COMMENT '�˺�����(md5ֵ)',
`privilege` TINYINT(4) UNSIGNED NOT NULL DEFAULT '0' COMMENT'�˺�Ȩ��',
`last_login` INT DEFAULT '0' COMMENT'�ϴε�½��ʱ��',
`last_ip` INT UNSIGNED ZEROFILL DEFAULT NULL COMMENT'�ϴε�½IP',
`login_status` TINYINT(3) UNSIGNED COMMENT'��ɫ��¼״̬��Ϣ',
`joindate` DATETIME COMMENT'�����˺ŵ�ʱ��',
`mac_0` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_1` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_2` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_3` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_4` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_5` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_6` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mac_7` TINYINT UNSIGNED DEFAULT '0' COMMENT'mac��ַ',
`mibao` blob COMMENT '�ܱ�',
PRIMARY KEY (`id`),
UNIQUE KEY `id`(`id`),
UNIQUE KEY `name`(`name`)
) ENGINE=MYISAM DEFAULT CHARACTER SET UTF8;


/*��������nexus_login.account�Ļ����˺ŵĴ洢����*/
DROP PROCEDURE IF EXISTS `account_cache`;
DELIMITER ;;
--	[1]name,[2]psd,[3]privilege
--	[4]last_login,[5]last_ip,[6]login_status,
--	[7]mac_0,[8]mac_1,[9]mac_2,[10]mac_3
--	[11]mac_4,[12]mac_5,[13]mac_6,[14]mac_7
CREATE PROCEDURE `account_cache`(IN idV INT UNSIGNED)
BEGIN
	SELECT	name, psd, privilege,
			last_login, last_ip, login_status,
			mac_0, mac_1, mac_2, mac_3,
			mac_4, mac_5, mac_6, mac_7 FROM `account` WHERE id=idV;
END;;
DELIMITER ;

/*��������nexus_login.account�ĵ�½��־�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `fix_login_status`;
DELIMITER ;;
CREATE PROCEDURE `fix_login_status`(IN idV INT UNSIGNED, IN login_statusV TINYINT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=login_statusV WHERE `account`.`id`=idV;
END;;
DELIMITER ;

/*��������nexus_login.account���޸������˺�Ϊָ��״̬�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `fix_all_login_status`;
DELIMITER ;;
CREATE PROCEDURE `fix_all_login_status`(IN login_statusV TINYINT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=login_statusV;
END;;
DELIMITER ;


/*��������nexus_login.account�ĵǳ��Ĵ洢����*/
DROP PROCEDURE IF EXISTS `account_login_out`;
DELIMITER ;;
CREATE PROCEDURE `account_login_out`(IN idV INT UNSIGNED, IN last_ipV INT UNSIGNED)
BEGIN
	UPDATE `account` SET login_status=0, last_login=NOW(), last_ip=last_ipV WHERE `account`.`id`=idV;
END;;
DELIMITER ;


/*��������nexus_login.account���˺��Ƿ���ڵĴ洢����*/
DROP PROCEDURE IF EXISTS `account_exist`;
DELIMITER ;;
CREATE PROCEDURE `account_exist`(IN idV INT UNSIGNED)
BEGIN
	SELECT id FROM `account` WHERE id=idV;
END;;
DELIMITER ;


/*��������nexus_login.account�Ļ�ȡ�˺�����洢����*/
DROP PROCEDURE IF EXISTS `account_psd`;
DELIMITER ;;
CREATE PROCEDURE `account_psd`(IN idV INT UNSIGNED)
BEGIN
	SELECT psd FROM `account` WHERE id=idV;
END;;
DELIMITER ;

/*��������nexus_login.account�Ļ�ȡ�˺�Ȩ�޴洢����*/
DROP PROCEDURE IF EXISTS `account_privilege`;
DELIMITER ;;
CREATE PROCEDURE `account_privilege`(IN idV INT UNSIGNED)
BEGIN
	SELECT privilege FROM `account` WHERE id=idV;
END;;
DELIMITER ;



/*��������nexus_login.account�Ļ�ȡ��ǰ�˺������洢����*/
DROP PROCEDURE IF EXISTS `account_num`;
DELIMITER ;;
CREATE PROCEDURE `account_num`()
BEGIN
	SELECT COUNT(*) FROM `account`;
END;;
DELIMITER ;


/*��������nexus_login.account�Ĵ����˺ŵĴ洢����*/
DROP PROCEDURE IF EXISTS `account_insert`;
DELIMITER ;;
--	[1]id,[2]name,[3]psd,[4]privilege
--	[5]last_login,[6]last_ip,[7]login_status,
--	[8]mac_0V,[9]mac_1V,[10]mac_2V,[11]mac_3V
--	[12]mac_4V,[13]mac_5V,[14]mac_6V,[15]mac_7V
CREATE PROCEDURE `account_insert`(
IN idV INT UNSIGNED, IN nameV CHAR(36), IN psdV char(16), IN privilegeV TINYINT UNSIGNED,
IN last_loginV INT UNSIGNED, IN last_ipV INT UNSIGNED, IN login_statusV TINYINT UNSIGNED,
IN mac_0V TINYINT UNSIGNED, IN mac_1V TINYINT UNSIGNED, IN mac_2V TINYINT UNSIGNED, IN mac_3V TINYINT UNSIGNED,
IN mac_4V TINYINT UNSIGNED, IN mac_5V TINYINT UNSIGNED, IN mac_6V TINYINT UNSIGNED, IN mac_7V TINYINT UNSIGNED)
BEGIN
	INSERT INTO `account` (	id, name, psd, privilege,
							last_login, last_ip, login_status,
							mac_0,mac_1,mac_2,mac_3,
							mac_4,mac_5,mac_6,mac_7,
							joindate) 
							VALUES (idV, nameV, psdV, privilegeV, 
									last_loginV, last_ipV, login_statusV,
									mac_0V, mac_1V, mac_2V, mac_3V,
									mac_4V, mac_5V, mac_6V, mac_7V,
									NOW());
END;;
DELIMITER ;





























/*ɾ��nexus_login.account_cmp�˺ű�*/
DROP TABLE IF EXISTS `account_cmp`;

/*����nexus_login.account_cmp�˺ű������ñ��Ŀ����Ϊ�˽�����صĲ���*/
CREATE TABLE `account_cmp` (
`id` int(11) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT COMMENT '�˺�ID',
`name` char(36) CHARACTER SET UTF8 NOT NULL COMMENT '�˺�����',
`psd`  char(32) CHARACTER SET UTF8 NOT NULL COMMENT '�˺�����',
`privilege` tinyint(4) UNSIGNED NOT NULL DEFAULT '0' COMMENT'�˺�Ȩ��',
`last_login` DATETIME DEFAULT NULL COMMENT'�ϴε�½��ʱ��',
`last_ip` int UNSIGNED ZEROFILL DEFAULT NULL COMMENT'�ϴε�½IP',
`login_status` tinyint(3) COMMENT'��ɫ��¼״̬��Ϣ',
`joindate` DATETIME COMMENT'�����˺ŵ�ʱ��',
`mibao` blob COMMENT '�ܱ�',
PRIMARY KEY (`id`),
UNIQUE KEY `name`(`name`)
) ENGINE=MYISAM DEFAULT CHARACTER SET UTF8;

/*��������nexus_login.account_cmp�ļ��insert�Ĵ洢����*/
DROP PROCEDURE IF EXISTS `test_insert`;
DELIMITER ;;
CREATE PROCEDURE `test_insert`(in id int, in name char(36), in privilege int)
BEGIN
	INSERT INTO `account_cmp`(id, name, privilege) VALUES (id, name, privilege);
END;;
DELIMITER ;
