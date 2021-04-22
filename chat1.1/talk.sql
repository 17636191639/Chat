/*
Navicat MySQL Data Transfer

Source Server         : trading
Source Server Version : 50018
Source Host           : localhost:3306
Source Database       : talk

Target Server Type    : MYSQL
Target Server Version : 50018
File Encoding         : 65001

Date: 2021-04-08 19:16:18
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for chat
-- ----------------------------
DROP TABLE IF EXISTS `chat`;
CREATE TABLE `chat` (
  `id` varchar(8) NOT NULL,
  `talkid` varchar(8) NOT NULL,
  `content` varchar(256) default NULL,
  PRIMARY KEY  (`id`,`talkid`),
  KEY `talkid` (`talkid`),
  CONSTRAINT `id` FOREIGN KEY (`id`) REFERENCES `user` (`id`),
  CONSTRAINT `talkid` FOREIGN KEY (`talkid`) REFERENCES `user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of chat
-- ----------------------------

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` varchar(8) NOT NULL,
  `paswd` varchar(8) default NULL,
  `name` varchar(255) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of user
-- ----------------------------
