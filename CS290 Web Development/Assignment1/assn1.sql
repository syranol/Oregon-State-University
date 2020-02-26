Create TABLE `client`(
    `id` int(11) AUTO_INCREMENT PRIMARY KEY,
    `first_name` varchar(255) NOT NULL,
    `last_name` varchar(255) NOT NULL,
    `dob` date NOT NULL,
    UNIQUE KEY`full_name` (`first_name`, `last_name`)
)ENGINE=InnoDB;


Create TABLE `employee`(
	`id` int(11) AUTO_INCREMENT PRIMARY KEY,
	`first_name` varchar(255) NOT NULL,
    `last_name` varchar(255) NOT NULL,
    `dob` date NOT NULL,
    `date_joined` date NOT NULL,
     UNIQUE KEY`full_name` (`first_name`, `last_name`)
)ENGINE=InnoDB;


Create TABLE `project`(
	`id` int(11) AUTO_INCREMENT PRIMARY KEY,
	`cid` int,
	FOREIGN KEY (`cid`) references client(`id`),
	`name` varchar(255) NOT NULL,
	UNIQUE KEY (`name`),
	`notes` text
)ENGINE=InnoDB;

Create TABLE `works_on`(
	`eid` int(11),
	`pid` int(11),
	FOREIGN KEY(`eid`) references employee(`id`),
	FOREIGN KEY(`pid`) references project(`id`),
	`start_date` date NOT NULL,
	PRIMARY KEY (`eid`, `pid`)
)ENGINE=InnoDB;
