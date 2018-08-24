-- phpMyAdmin SQL Dump
-- version 3.4.10.2
-- http://www.phpmyadmin.net
--
-- Client: localhost:3306
-- GÃ©nÃ©rÃ© le : Lun 28 Janvier 2013 Ã  22:39
-- Version du serveur: 5.1.66
-- Version de PHP: 5.3.10

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Base de donnÃ©es: `wrc`
--

-- --------------------------------------------------------

--
-- Structure de la table `saison`
--

CREATE TABLE IF NOT EXISTS `saison` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `storm_start` int(4) NOT NULL,
  `storm_end` int(4) NOT NULL,
  `cloud_start` int(4) NOT NULL,
  `cloud_end` int(4) NOT NULL,
  `date` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=912 ;

--
-- Contenu de la table `saison`
--

INSERT INTO `saison` (`id`, `storm_start`, `storm_end`, `cloud_start`, `cloud_end`, `date`) VALUES
(711, 0, 0, 60927, 62347, 1356390366),
(712, 43485, 45037, 68221, 71276, 1356476771),
(713, 39788, 40951, 57726, 59622, 1356563176),
(714, 0, 0, 57217, 58006, 1356649583),
(715, 71431, 72879, 73396, 74937, 1356735989),
(735, 0, 0, 1947177476, 1947180514, 1356902995),
(736, 0, 0, 52280, 54212, 1358981994),
(835, 0, 0, 61764, 62417, 1359241161),
(904, 0, 0, 70058, 72794, 1359303685),
(911, 47860, 50596, 65944, 68970, 1359390749);

-- --------------------------------------------------------

--
-- Structure de la table `temps`
--

CREATE TABLE IF NOT EXISTS `temps` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `temp1` float NOT NULL,
  `temp2` float NOT NULL,
  `date` int(10) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6678 ;

--
-- Contenu de la table `temps`
--

INSERT INTO `temps` (`id`, `temp1`, `temp2`, `date`) VALUES
(6677, 21.6, 24.9, 1359409018),
(6667, 21.8, 24.8, 1359396850),
(6668, 21.9, 24.9, 1359398066),
(6669, 21.8, 24.9, 1359399282),
(6670, 21.8, 24.8, 1359400499),
(6671, 21.9, 24.9, 1359401715),
(6672, 21.9, 24.9, 1359402932),
(6673, 21.9, 25, 1359404148),
(6674, 21.9, 24.9, 1359405367),
(6675, 21.8, 24.9, 1359406586),
(6676, 21.7, 24.9, 1359407802);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
