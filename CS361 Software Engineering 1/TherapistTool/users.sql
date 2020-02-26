DROP TABLE IF EXISTS `patients`;

CREATE TABLE `patients` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`fname` varchar(25) NOT NULL,
	`lname` varchar(30) NOT NULL,
	`insurance` varchar(50) NOT NULL,
	`religion` varchar(50),
	`soreintation` varchar(30),
	`illness` varchar(255) NOT NULL,
	PRIMARY KEY(`id`)
)engine=InnoDB;

DROP TABLE IF EXISTS `therapists`;

CREATE TABLE `therapists` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`fname` varchar(25) NOT NULL,
	`lname` varchar(30) NOT NULL,
	`experience` INT NOT NULL,
	`specialty` varchar(255),
	`insurance` varchar(255) NOT NULL,
	`specialHistory` varchar(255),
	`address` varchar(255) NOT NULL,
	`sorientation` varchar(30),
	`religion` varchar(50),
	PRIMARY KEY(`id`)
)engine=InnoDB; 

DROP TABLE IF EXISTS `locations`;

CREATE TABLE `locations` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`tid` INT,
	`pid` INT,
	`state` varchar(30) NOT NULL,
	`city` varchar(50) NOT NULL,
	PRIMARY KEY(`id`),
	FOREIGN KEY(`tid`) REFERENCES therapists(`id`),
	FOREIGN KEY(`pid`) REFERENCES patients(`id`)
)engine=InnoDB;

