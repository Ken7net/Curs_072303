/*
 Navicat Premium Data Transfer

 Source Server         : CURS
 Source Server Type    : MySQL
 Source Server Version : 80028
 Source Host           : 192.168.0.21:3306
 Source Schema         : curs

 Target Server Type    : MySQL
 Target Server Version : 80028
 File Encoding         : 65001

 Date: 08/05/2022 12:29:46
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for company
-- ----------------------------
DROP TABLE IF EXISTS `company`;
CREATE TABLE `company`  (
  `company_id` int NOT NULL AUTO_INCREMENT,
  `company_name` varchar(15) CHARACTER SET cp1251 COLLATE cp1251_general_ci NULL DEFAULT NULL,
  `activity` varchar(40) CHARACTER SET cp1251 COLLATE cp1251_general_ci NULL DEFAULT NULL,
  `finance` int NULL DEFAULT 0,
  PRIMARY KEY (`company_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of company
-- ----------------------------
INSERT INTO `company` VALUES (1, 'ОкнаСиль', 'Окна', 1590654);
INSERT INTO `company` VALUES (2, 'Стиляги', 'Одежда', 15906);
INSERT INTO `company` VALUES (3, 'IT Prog', 'IT', 1590654);
INSERT INTO `company` VALUES (4, 'СООО\"Дивет\"', 'Мебель', 15902630);
INSERT INTO `company` VALUES (5, 'Рога_и_копыта', 'Все_по_немногу', 15000000);

-- ----------------------------
-- Table structure for inventory
-- ----------------------------
DROP TABLE IF EXISTS `inventory`;
CREATE TABLE `inventory`  (
  `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(50) CHARACTER SET cp1251 COLLATE cp1251_general_ci NULL DEFAULT NULL,
  `quantity` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `id`(`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of inventory
-- ----------------------------
INSERT INTO `inventory` VALUES (2, 'orange', 154);
INSERT INTO `inventory` VALUES (3, 'apple', 300);

-- ----------------------------
-- Table structure for mark
-- ----------------------------
DROP TABLE IF EXISTS `mark`;
CREATE TABLE `mark`  (
  `mark_id` int NOT NULL AUTO_INCREMENT,
  `number` int NULL DEFAULT NULL,
  `user_id` int NULL DEFAULT NULL,
  `project1_id` int NULL DEFAULT NULL,
  `project2_id` int NULL DEFAULT NULL,
  `mark1` float NULL DEFAULT NULL,
  `mark2` float NULL DEFAULT NULL,
  UNIQUE INDEX `mark_id`(`mark_id`) USING BTREE,
  INDEX `mark_ibfk_1`(`user_id`) USING BTREE,
  INDEX `mark_ibfk_2`(`project1_id`) USING BTREE,
  INDEX `mark_ibfk_3`(`project2_id`) USING BTREE,
  CONSTRAINT `mark_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `user` (`user_id`) ON DELETE SET NULL ON UPDATE SET NULL,
  CONSTRAINT `mark_ibfk_2` FOREIGN KEY (`project1_id`) REFERENCES `project` (`project_id`) ON DELETE SET NULL ON UPDATE SET NULL,
  CONSTRAINT `mark_ibfk_3` FOREIGN KEY (`project2_id`) REFERENCES `project` (`project_id`) ON DELETE SET NULL ON UPDATE SET NULL
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of mark
-- ----------------------------
INSERT INTO `mark` VALUES (1, 1, 3, 1, 2, 0.5, 0.5);
INSERT INTO `mark` VALUES (2, 1, 3, 2, 3, 0.7, 0.3);
INSERT INTO `mark` VALUES (3, 1, 3, 3, 1, 0.4, 0.6);

-- ----------------------------
-- Table structure for project
-- ----------------------------
DROP TABLE IF EXISTS `project`;
CREATE TABLE `project`  (
  `project_id` int NOT NULL AUTO_INCREMENT,
  `project_name` varchar(50) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL,
  `sum_credit` int NOT NULL,
  `credit_time` int NOT NULL,
  `sud_reestr` varchar(3) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL DEFAULT 'Нет',
  `application_date` date NOT NULL,
  `company_id` int NULL DEFAULT NULL,
  PRIMARY KEY (`project_id`) USING BTREE,
  INDEX `Project_fk1`(`company_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 8 CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of project
-- ----------------------------
INSERT INTO `project` VALUES (1, 'Окна++', 15559, 15, 'Нет', '2022-05-07', 1);
INSERT INTO `project` VALUES (2, 'Новые ткани', 155, 2, 'Да', '2022-05-03', 2);
INSERT INTO `project` VALUES (3, 'Новое оборудование', 46698, 5, 'Да', '2022-05-03', 3);
INSERT INTO `project` VALUES (5, 'Новые_станки', 100500, 18, 'Нет', '2022-05-07', 1);
INSERT INTO `project` VALUES (7, 'Модернизация_производства', 1500000, 20, 'Нет', '2022-05-07', 2);
INSERT INTO `project` VALUES (8, 'Новый_магазин', 100500, 19, 'Нет', '2022-05-07', 1);

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `user_id` int NOT NULL AUTO_INCREMENT,
  `user_name` varchar(50) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL,
  `login` varchar(40) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL,
  `pass` varchar(40) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL,
  `role` varchar(40) CHARACTER SET cp1251 COLLATE cp1251_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`user_id`) USING BTREE,
  UNIQUE INDEX `login`(`login`) USING BTREE,
  INDEX `role`(`role`) USING BTREE,
  INDEX `pass`(`pass`) USING BTREE,
  INDEX `user_name`(`user_name`) USING BTREE,
  CONSTRAINT `role_fk` FOREIGN KEY (`role`) REFERENCES `user_role` (`role`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 17 CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (1, 'Arina', 'Arina', '1601', 'Администратор');
INSERT INTO `user` VALUES (2, 'Килбут К.', 'xam345', 'max07', 'Представитель Компании');
INSERT INTO `user` VALUES (3, 'Петр I', 'Petya', '183583', 'Эксперт');
INSERT INTO `user` VALUES (8, 'Chapay Vasya', 'Chaplin', '1945', 'Администратор');
INSERT INTO `user` VALUES (11, 'Чапаев Василий', 'Chaply', '1917', 'Эксперт');
INSERT INTO `user` VALUES (12, 'Жуков Георгий', 'Zhuchara', '1945', 'Эксперт');
INSERT INTO `user` VALUES (16, 'Выпивоха', 'DrinkingBoss', '123', 'Администратор');
INSERT INTO `user` VALUES (17, 'Пьянь Подзаборная', 'Drink4ever', '654', 'Эксперт');

-- ----------------------------
-- Table structure for user_role
-- ----------------------------
DROP TABLE IF EXISTS `user_role`;
CREATE TABLE `user_role`  (
  `role` varchar(40) CHARACTER SET cp1251 COLLATE cp1251_general_ci NOT NULL,
  PRIMARY KEY (`role`) USING BTREE,
  UNIQUE INDEX `name`(`role`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = cp1251 COLLATE = cp1251_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_role
-- ----------------------------
INSERT INTO `user_role` VALUES ('Администратор');
INSERT INTO `user_role` VALUES ('Представитель Компании');
INSERT INTO `user_role` VALUES ('Эксперт');

-- ----------------------------
-- View structure for view_name
-- ----------------------------
DROP VIEW IF EXISTS `view_name`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `view_name` AS select `users`.`users_id` AS `users_id`,`users`.`users_name` AS `users_name`,`users`.`login` AS `login`,`users`.`pass` AS `pass`,`users`.`role` AS `role` from `users`;

SET FOREIGN_KEY_CHECKS = 1;
