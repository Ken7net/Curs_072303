/*
Navicat MySQL Data Transfer

Source Server         : MYCURS
Source Server Version : 50505
Source Host           : 127.0.0.1:3306
Source Database       : curs

Target Server Type    : MYSQL
Target Server Version : 50505
File Encoding         : 65001

Date: 2022-05-06 16:20:55
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `company`
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company` (
  `company_id` int(11) NOT NULL AUTO_INCREMENT,
  `company_name` varchar(15) NOT NULL,
  `activity` varchar(40) DEFAULT NULL,
  `finance` int(12) DEFAULT 0,
  PRIMARY KEY (`company_id`),
  UNIQUE KEY `company_name` (`company_name`),
  CONSTRAINT `company_chk_1` CHECK (`finance` > 0)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of company
-- ----------------------------
INSERT INTO `company` VALUES ('1', 'ОкнаСиль', 'Окна', '1590654'), ('2', 'Стиляги', 'Одежда', '15906'), ('3', 'IT Prog', 'IT', '1590654'), ('4', 'СООО\"Дивет\"', 'Мебель', '15902630');

-- ----------------------------
-- Table structure for `inventory`
-- ----------------------------
DROP TABLE IF EXISTS `inventory`;
CREATE TABLE `inventory` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `quantity` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of inventory
-- ----------------------------
INSERT INTO `inventory` VALUES ('2', 'orange', '154'), ('3', 'apple', '300');

-- ----------------------------
-- Table structure for `mark`
-- ----------------------------
DROP TABLE IF EXISTS `mark`;
CREATE TABLE `mark` (
  `mark_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `project1_id` int(11) NOT NULL,
  `project2_id` int(11) NOT NULL,
  `mark1` float NOT NULL,
  `mark2` float NOT NULL,
  KEY `users_id` (`user_id`),
  KEY `project1_id` (`project1_id`),
  KEY `project2_id` (`project2_id`),
  KEY `number_mark` (`mark_id`) USING BTREE,
  CONSTRAINT `mark_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `mark_ibfk_2` FOREIGN KEY (`project1_id`) REFERENCES `project` (`project_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `mark_ibfk_3` FOREIGN KEY (`project2_id`) REFERENCES `project` (`project_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `marks_chk_1` CHECK (`mark1` > -1 and `mark1` < 1.1)
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of mark
-- ----------------------------
INSERT INTO `mark` VALUES ('1', '3', '1', '2', '0.5', '0'), ('2', '3', '2', '3', '0.7', '0'), ('3', '3', '3', '1', '0.4', '0');

-- ----------------------------
-- Table structure for `project`
-- ----------------------------
DROP TABLE IF EXISTS `project`;
CREATE TABLE `project` (
  `project_id` int(11) NOT NULL AUTO_INCREMENT,
  `project_name` varchar(50) NOT NULL,
  `sum_credit` decimal(12,3) NOT NULL,
  `credit_time` int(11) NOT NULL,
  `sud_reestr` varchar(3) NOT NULL DEFAULT 'Нет',
  `application_date` date NOT NULL,
  `company_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`project_id`),
  KEY `Project_fk1` (`company_id`),
  CONSTRAINT `Project_fk1` FOREIGN KEY (`company_id`) REFERENCES `company` (`company_id`),
  CONSTRAINT `project_chk_1` CHECK (`sum_credit` > 0),
  CONSTRAINT `project_chk_2` CHECK (`credit_time` > 0 and `credit_time` < 21)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of project
-- ----------------------------
INSERT INTO `project` VALUES ('1', 'ОКна+', '15559.000', '15', 'Нет', '2022-05-01', '1'), ('2', 'Новые ткани', '155.000', '2', 'Да', '2022-05-03', '2'), ('3', 'Новое оборудование', '46698.000', '5', 'Да', '2022-05-03', '3'), ('4', 'Новые_станки', '500000.000', '20', 'нет', '2022-05-06', '4'), ('5', 'Новый_магазин', '150000.000', '20', 'Нет', '2022-05-06', '1');

-- ----------------------------
-- Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `user_name` varchar(50) NOT NULL,
  `login` varchar(40) NOT NULL,
  `pass` varchar(40) NOT NULL,
  `role` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `login` (`login`),
  KEY `role` (`role`),
  KEY `pass` (`pass`) USING BTREE,
  KEY `user_name` (`user_name`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=cp1251;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('1', 'Arina', 'Arina', '1601', 'Администратор'), ('2', 'Килбут К.', 'xam345', 'max07', 'Представитель Компании'), ('3', 'Петр I', 'Petya', '183583', 'Эксперт'), ('8', 'Chapay Vasya', 'Chaplin', '1945', 'Администратор'), ('11', 'Чапаев Василий', 'Chaply', '1917', 'Эксперт'), ('12', 'Жуков Георгий', 'Zhuchara', '1945', 'Эксперт');

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
INSERT INTO `user_role` VALUES ('Администратор'), ('Представитель Компании'), ('Эксперт');
