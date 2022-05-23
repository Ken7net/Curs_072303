/*
Navicat MySQL Data Transfer

Source Server         : ArinaCurs
Source Server Version : 80022
Source Host           : b3gnmkwrbyubgxcx55rc-mysql.services.clever-cloud.com:3306
Source Database       : b3gnmkwrbyubgxcx55rc

Target Server Type    : MYSQL
Target Server Version : 80022
File Encoding         : 65001

Date: 2022-05-23 16:56:02
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `company`
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `company_id` int NOT NULL AUTO_INCREMENT,
  `company_name` varchar(15) NOT NULL,
  `activity` varchar(40) DEFAULT NULL,
  `finance` int DEFAULT '0',
  PRIMARY KEY (`company_id`),
  UNIQUE KEY `company_name` (`company_name`),
  CONSTRAINT `company_chk_1` CHECK ((`finance` > 0))
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of company
-- ----------------------------
INSERT INTO `company` VALUES ('1', 'ОкнаСиль', 'Окна', '1590654');
INSERT INTO `company` VALUES ('2', 'Стиляги', 'Одежда', '15906');
INSERT INTO `company` VALUES ('3', 'IT Prog', 'IT', '1590654');
INSERT INTO `company` VALUES ('4', 'СООО\"Дивет\"', 'Мебель', '15902630');
INSERT INTO `company` VALUES ('5', 'БьютиБэй', 'Косметика', '156230');
INSERT INTO `company` VALUES ('6', 'Маркформель', 'Одежда', '15600');
INSERT INTO `company` VALUES ('7', 'Ариэль', 'Техника', '15698');

-- ----------------------------
-- Table structure for `mark`
-- ----------------------------
DROP TABLE IF EXISTS `mark`;
CREATE TABLE `mark` (
  `mark_id` int NOT NULL AUTO_INCREMENT,
  `number` int NOT NULL,
  `user_id` int DEFAULT NULL,
  `project1_id` int DEFAULT NULL,
  `project2_id` int DEFAULT NULL,
  `mark1` float DEFAULT NULL,
  `mark2` float DEFAULT NULL,
  PRIMARY KEY (`mark_id`),
  KEY `number_mark` (`mark_id`) USING BTREE,
  KEY `mark_ibfk1` (`user_id`),
  KEY `mark_ibfk2` (`project1_id`),
  KEY `mark_ibfk3` (`project2_id`),
  CONSTRAINT `mark_ibfk1` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`) ON DELETE CASCADE,
  CONSTRAINT `mark_ibfk2` FOREIGN KEY (`project1_id`) REFERENCES `project` (`project_id`) ON DELETE CASCADE,
  CONSTRAINT `mark_ibfk3` FOREIGN KEY (`project2_id`) REFERENCES `project` (`project_id`) ON DELETE CASCADE,
  CONSTRAINT `marks_chk_1` CHECK (((`mark1` > -(1)) and (`mark1` < 1.1)))
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of mark
-- ----------------------------
INSERT INTO `mark` VALUES ('1', '0', '3', '1', '2', '0.5', '0.5');
INSERT INTO `mark` VALUES ('2', '0', '3', '2', '3', '0.7', '0.3');
INSERT INTO `mark` VALUES ('3', '0', '3', '3', '1', '0.4', '0.6');
INSERT INTO `mark` VALUES ('4', '2', '3', '3', '4', '0.61', '0.39');
INSERT INTO `mark` VALUES ('5', '2', '3', '3', '1', '0.69', '0.31');
INSERT INTO `mark` VALUES ('6', '2', '3', '4', '1', '0.3', '0.7');
INSERT INTO `mark` VALUES ('7', '2', '11', '3', '4', '0.57', '0.43');
INSERT INTO `mark` VALUES ('8', '2', '11', '3', '1', '0.75', '0.25');
INSERT INTO `mark` VALUES ('9', '2', '11', '4', '1', '0.4', '0.6');
INSERT INTO `mark` VALUES ('10', '2', '12', '3', '4', '0.59', '0.41');
INSERT INTO `mark` VALUES ('11', '2', '12', '3', '1', '0.72', '0.28');
INSERT INTO `mark` VALUES ('12', '2', '12', '4', '1', '0.23', '0.77');
INSERT INTO `mark` VALUES ('23', '3', '3', '3', '5', '0.45', '0.55');
INSERT INTO `mark` VALUES ('24', '3', '3', '3', '4', '0.26', '0.74');
INSERT INTO `mark` VALUES ('25', '3', '3', '5', '4', '0.75', '0.25');
INSERT INTO `mark` VALUES ('26', '3', '11', '3', '5', '0.75', '0.25');
INSERT INTO `mark` VALUES ('27', '3', '11', '3', '4', '0.6', '0.4');
INSERT INTO `mark` VALUES ('28', '3', '11', '5', '4', '0.2', '0.8');
INSERT INTO `mark` VALUES ('29', '3', '12', '3', '5', '0.87', '0.13');
INSERT INTO `mark` VALUES ('30', '3', '12', '3', '4', '0.25', '0.75');
INSERT INTO `mark` VALUES ('31', '3', '12', '5', '4', '0.56', '0.44');
INSERT INTO `mark` VALUES ('32', '3', '17', '3', '5', '0.36', '0.64');
INSERT INTO `mark` VALUES ('33', '3', '17', '3', '4', '0.25', '0.75');
INSERT INTO `mark` VALUES ('34', '3', '17', '5', '4', '0.13', '0.87');
INSERT INTO `mark` VALUES ('35', '3', '18', '3', '5', '0.9', '0.1');
INSERT INTO `mark` VALUES ('36', '3', '18', '3', '4', '0.8', '0.2');
INSERT INTO `mark` VALUES ('37', '3', '18', '5', '4', '0.1', '0.9');
INSERT INTO `mark` VALUES ('44', '4', '12', '5', '4', '0.45', '0.55');
INSERT INTO `mark` VALUES ('45', '4', '12', '5', '3', '0.48', '0.52');
INSERT INTO `mark` VALUES ('46', '4', '12', '5', '2', '0.84', '0.16');
INSERT INTO `mark` VALUES ('47', '4', '12', '4', '3', '0.52', '0.48');
INSERT INTO `mark` VALUES ('48', '4', '12', '4', '2', '0.78', '0.22');
INSERT INTO `mark` VALUES ('49', '4', '12', '3', '2', '0.82', '0.18');
INSERT INTO `mark` VALUES ('50', '4', '18', '5', '4', '0.43', '0.57');
INSERT INTO `mark` VALUES ('51', '4', '18', '5', '3', '0.47', '0.53');
INSERT INTO `mark` VALUES ('52', '4', '18', '5', '2', '0.82', '0.18');
INSERT INTO `mark` VALUES ('53', '4', '18', '4', '3', '0.57', '0.43');
INSERT INTO `mark` VALUES ('54', '4', '18', '4', '2', '0.77', '0.23');
INSERT INTO `mark` VALUES ('55', '4', '18', '3', '2', '0.84', '0.16');

-- ----------------------------
-- Table structure for `project`
-- ----------------------------
DROP TABLE IF EXISTS `project`;
CREATE TABLE `project` (
  `project_id` int NOT NULL AUTO_INCREMENT,
  `project_name` varchar(50) NOT NULL,
  `sum_credit` int NOT NULL,
  `credit_time` int NOT NULL,
  `sud_reestr` varchar(3) NOT NULL DEFAULT 'Нет',
  `application_date` date NOT NULL,
  `company_id` int DEFAULT NULL,
  PRIMARY KEY (`project_id`),
  UNIQUE KEY `ProjectId` (`project_id`),
  KEY `Project_fk1` (`company_id`),
  CONSTRAINT `Project_fk1` FOREIGN KEY (`company_id`) REFERENCES `company` (`company_id`) ON DELETE CASCADE,
  CONSTRAINT `project_chk_1` CHECK ((`sum_credit` > 0)),
  CONSTRAINT `project_chk_2` CHECK (((`credit_time` > 0) and (`credit_time` < 21)))
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of project
-- ----------------------------
INSERT INTO `project` VALUES ('1', 'ОКна+', '15559', '15', 'Нет', '2022-05-01', '1');
INSERT INTO `project` VALUES ('2', 'Новые ткани', '155', '2', 'Да', '2022-05-03', '2');
INSERT INTO `project` VALUES ('3', 'Новое оборудование', '46698', '5', 'Да', '2022-05-03', '3');
INSERT INTO `project` VALUES ('4', 'Новые станки', '500000', '20', 'нет', '2022-05-06', '4');
INSERT INTO `project` VALUES ('5', 'Новый магазин', '150000', '20', 'Нет', '2022-05-06', '1');
INSERT INTO `project` VALUES ('6', 'Улыбка', '8000', '13', 'Да', '2022-05-20', '6');
INSERT INTO `project` VALUES ('7', 'АриэлькА', '15600', '18', 'Да', '2022-05-21', '7');

-- ----------------------------
-- Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `user_id` int NOT NULL AUTO_INCREMENT,
  `user_name` varchar(50) NOT NULL,
  `login` varchar(40) NOT NULL,
  `pass` varchar(40) NOT NULL,
  `role` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `login` (`login`),
  KEY `role` (`role`),
  KEY `pass` (`pass`) USING BTREE,
  KEY `user_name` (`user_name`) USING BTREE,
  CONSTRAINT `fk1` FOREIGN KEY (`role`) REFERENCES `user_role` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('1', 'Корнилова Арина', 'Arina', 'MdcL<;b<', 'Менеджер');
INSERT INTO `user` VALUES ('2', 'Городная Юлия', 'Salem', '999', 'Консультант банка');
INSERT INTO `user` VALUES ('3', 'Петр I', 'Petya', '<5>85>', 'Эксперт');
INSERT INTO `user` VALUES ('8', 'Chapay Vasya', 'Chaplin', '<498', 'Консультант банка');
INSERT INTO `user` VALUES ('11', 'Чапаев Василий', 'Chaply', '<4<:', 'Эксперт');
INSERT INTO `user` VALUES ('12', 'Жуков Георгий', 'Zhuchara', '<498', 'Эксперт');
INSERT INTO `user` VALUES ('16', 'Василий', 'Vasya007', '<?>', 'Менеджер');
INSERT INTO `user` VALUES ('17', 'Афанасьев Анатолий', 'Anatolii', '<8>?', 'Эксперт');
INSERT INTO `user` VALUES ('18', 'Степанова Евгения', 'Stepash', '<8>?', 'Эксперт');
INSERT INTO `user` VALUES ('23', 'Подопригора Антон', 'Prigorok', ']<j=bF', 'Эксперт');
INSERT INTO `user` VALUES ('26', 'Лавникович Дарья', 'Plavnick', '<84:8>', 'Консультант банка');

-- ----------------------------
-- Table structure for `user_role`
-- ----------------------------
DROP TABLE IF EXISTS `user_role`;
CREATE TABLE `user_role` (
  `name` varchar(40) NOT NULL,
  PRIMARY KEY (`name`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of user_role
-- ----------------------------
INSERT INTO `user_role` VALUES ('Консультант банка');
INSERT INTO `user_role` VALUES ('Менеджер');
INSERT INTO `user_role` VALUES ('Эксперт');
