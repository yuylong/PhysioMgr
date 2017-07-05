CREATE DATABASE  IF NOT EXISTS `ysphysiodb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `ysphysiodb`;
-- MySQL dump 10.13  Distrib 5.7.17, for Win64 (x86_64)
--
-- Host: 10.3.3.157    Database: ysphysiodb
-- ------------------------------------------------------
-- Server version	5.7.18-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `departments`
--

DROP TABLE IF EXISTS `departments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `departments` (
  `id` varchar(64) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `departments`
--

LOCK TABLES `departments` WRITE;
/*!40000 ALTER TABLE `departments` DISABLE KEYS */;
INSERT INTO `departments` VALUES ('EK','儿科'),('FK','妇科'),('NK','内科'),('WK','外科');
/*!40000 ALTER TABLE `departments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `doctors`
--

DROP TABLE IF EXISTS `doctors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `doctors` (
  `id` varchar(64) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `isnurse` tinyint(4) DEFAULT NULL,
  `type` varchar(32) DEFAULT NULL,
  `depid` varchar(64) DEFAULT NULL,
  `depname` varchar(64) DEFAULT NULL,
  `phonenum` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `doctors`
--

LOCK TABLES `doctors` WRITE;
/*!40000 ALTER TABLE `doctors` DISABLE KEYS */;
/*!40000 ALTER TABLE `doctors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `hospi_physio`
--

DROP TABLE IF EXISTS `hospi_physio`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hospi_physio` (
  `hospi_id` bigint(20) NOT NULL,
  `physio_id` varchar(64) DEFAULT NULL,
  `physio_name` varchar(64) DEFAULT NULL,
  `freq_day` int(11) DEFAULT NULL,
  `freq_cnt` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `hospi_physio`
--

LOCK TABLES `hospi_physio` WRITE;
/*!40000 ALTER TABLE `hospi_physio` DISABLE KEYS */;
/*!40000 ALTER TABLE `hospi_physio` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `hospi_records`
--

DROP TABLE IF EXISTS `hospi_records`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hospi_records` (
  `id` bigint(20) NOT NULL,
  `pati_id` varchar(64) DEFAULT NULL,
  `pati_name` varchar(64) DEFAULT NULL,
  `room_id` varchar(64) DEFAULT NULL,
  `doct_id` varchar(64) DEFAULT NULL,
  `doct_name` varchar(64) DEFAULT NULL,
  `nurse_id` varchar(64) DEFAULT NULL,
  `nurse_name` varchar(64) DEFAULT NULL,
  `startdate` date DEFAULT NULL,
  `enddate` date DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `hospi_records`
--

LOCK TABLES `hospi_records` WRITE;
/*!40000 ALTER TABLE `hospi_records` DISABLE KEYS */;
/*!40000 ALTER TABLE `hospi_records` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `patients`
--

DROP TABLE IF EXISTS `patients`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `patients` (
  `id` varchar(64) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `dob` date DEFAULT NULL,
  `phonenum` varchar(32) DEFAULT NULL,
  `address` varchar(128) DEFAULT NULL,
  `comment` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `patients`
--

LOCK TABLES `patients` WRITE;
/*!40000 ALTER TABLE `patients` DISABLE KEYS */;
/*!40000 ALTER TABLE `patients` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `physio_items`
--

DROP TABLE IF EXISTS `physio_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `physio_items` (
  `id` varchar(64) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `price` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `physio_items`
--

LOCK TABLES `physio_items` WRITE;
/*!40000 ALTER TABLE `physio_items` DISABLE KEYS */;
INSERT INTO `physio_items` VALUES ('AJ','艾灸',1.00),('BBG','白拔罐',1.00),('CDB','超短波',1.00),('CJB','冲击波',1.00),('FXBG','放血拔罐',1.00),('FZ','飞针',1.00),('JTN','颈推拿',1.00),('JTN2','肩推拿',1.00),('LL','腊疗',1.00),('LXY','卢向阳',1.00),('MB','米波',1.00),('NMB','纳米波',1.00),('QY','牵引',1.00),('SBG','石拔罐',1.00),('SXZTN','上下肢推拿',1.00),('WB','微波',1.00),('WF','外敷',1.00),('WFKD','外敷烤电',1.00),('XGTN','膝盖推拿',1.00),('XJY','修靖延',1.00),('YTN','腰推拿',1.00),('ZJ','针灸',1.00),('ZP','中频',1.00),('ZYXZ','中药熏蒸',1.00);
/*!40000 ALTER TABLE `physio_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `physio_rec`
--

DROP TABLE IF EXISTS `physio_rec`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `physio_rec` (
  `pati_id` varchar(64) NOT NULL,
  `pati_name` varchar(64) DEFAULT NULL,
  `physio_id` varchar(64) NOT NULL,
  `physio_name` varchar(64) DEFAULT NULL,
  `optime` datetime NOT NULL,
  PRIMARY KEY (`pati_id`,`physio_id`,`optime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `physio_rec`
--

LOCK TABLES `physio_rec` WRITE;
/*!40000 ALTER TABLE `physio_rec` DISABLE KEYS */;
/*!40000 ALTER TABLE `physio_rec` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'ysphysiodb'
--

--
-- Dumping routines for database 'ysphysiodb'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-07-05 16:01:04
