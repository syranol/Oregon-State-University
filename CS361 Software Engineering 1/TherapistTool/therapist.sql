# TherapyFit Database
# This follows the ERD from HW1 with a few minor changes:
# -Patient table is yet to be implemented and just focusing on therapist
# -The Insurance attribute was given its own entity since it is
#		in a many to many relationship with the therapist entity
# -A new entity was created, therapist_insurance, which represents
# 		the relationship between the therapists and the insurance they take.
# - Location, sexual orientation, specialty and religion all have
# 		their own entities while the therapist entity only has the attribute IDs
# -Address attribute removed from therapist
# -Since the example therapists are spread out among 20 cities,
#  		I larger sample in the New York market for testing purposes.

--
-- Table structure for table `location`
-- 

DROP TABLE IF EXISTS `location`;
CREATE TABLE `location` (
	`locID` int(11) AUTO_INCREMENT NOT NULL,
	`city` varchar(50) NOT NULL,
	`state` varchar(50) NOT NULL,
	`zip` int(11),
	PRIMARY KEY (`locID`)
)ENGINE=InnoDB AUTO_INCREMENT=1000 DEFAULT CHARSET=latin1;

--
-- Example data for table `specialty`
--

INSERT INTO `location` (`city`, `state`, `zip`) VALUES
	('Portland', 'Oregon', 97202), 
	('Los Angeles', 'California', 90011), 
	('Chicago', 'Illinois', 60629), 
	('Houston', 'Texas', 77084), 
	('Philadelphia', 'Pennsylvania', 19102), 
	('Phoenix', 'Arizona', 85003), 
	('San Antonio', 'Texas', 78201), 
	('San Diego', 'California', 92101), 
	('Dallas', 'Texas', 75201), 
	('San Jose', 'California', 95110),
	('Austin', 'Texas', 78701), 
	('San Francisco', 'California', 94102), 
	('Indianapolis', 'Indiana', 46201), 
	('Columbus', 'Ohio', 43085), 
	('Fort Worth', 'Texas', 76102), 
	('Charlotte', 'North Carolina', 28202),  
	('Detroit', 'Michigan', 48201),  
	('El Paso', 'Texas', 79936),
	('Seattle', 'Washington', 98101),
	('Denver', 'Colorado', 80202);

--
-- Table structure for table `sexualOrientation`
-- 

DROP TABLE IF EXISTS `sexualOrientation`;
CREATE TABLE `sexualOrientation` (
	`sexID` int(11) AUTO_INCREMENT NOT NULL,
	`name` varchar(50) NOT NULL,
	PRIMARY KEY (`sexID`)
)ENGINE=InnoDB AUTO_INCREMENT=2000 DEFAULT CHARSET=latin1;

--
-- Example data for table `specialty`
--

INSERT INTO `sexualOrientation` (`name`) VALUES
	('Heterosexual'), 
	('Homosexual'), 
	('Gay'),
	('Queer'),
	('Pansexual'),
	('Asexual'),
	('Bisexual'),
	('Lesbian');     

--
-- Table structure for table `religion`
-- 

DROP TABLE IF EXISTS `religion`;
CREATE TABLE `religion` (
	`relID` int(11) AUTO_INCREMENT NOT NULL,
	`name` varchar(50) NOT NULL,
	PRIMARY KEY (`relID`)
)ENGINE=InnoDB AUTO_INCREMENT=10000 DEFAULT CHARSET=latin1;

--
-- Example data for table `religion`
--

INSERT INTO `religion` (`name`) VALUES
	('Chistianity'), 
	('Islam'), 
	('Nonreligious(Secular/Agnostic/Atheist)'),
	('Hinduism'),
	('Chinese Traditional'),
	('Buddhism'),
	('Judaism'),
	('Radiology'),
	('Sikhism'),
	('Spirtism'),
	('Shinto'),
	('Cao Dai'),
	('Zoroastrianism'),
	('Tenrikyo'),
	('Primal-indigenous'),
	('African Traditional'),
	('Juche');

--
-- Table structure for table `specialty`
-- 

DROP TABLE IF EXISTS `specialty`;
CREATE TABLE `specialty` (
	`specID` int(11) AUTO_INCREMENT NOT NULL,
	`name` varchar(50) NOT NULL,
	PRIMARY KEY (`specID`)
)ENGINE=InnoDB AUTO_INCREMENT=20000 DEFAULT CHARSET=latin1;

--
-- Example data for table `specialty`
--

INSERT INTO `specialty` (`name`) VALUES
	('Marriage and Family'), 
	('Guidance and Career'), 
	('Rehabilitation'),
	('Mental Health'),
	('Substance Abuse'),
	('Educational'),
	('Debt'),
	('Child Development'),
	('Grief'),
	('Art Therapy'),
	('Musical Therapy'),
	('Anxiety'),
	('Attention Deficit Disorder'),
	('Bipolarity'),
	('Borderline Personality'),
	('Depression'),
	('Eating Disorders'),
	('Generalized Anxiety'),
	('Obsessive Compulsive Disorder'),
	('Panic Disorder'),
	('Post-traumatic Stress Disorder'),
	('Schizophrenia'),
	('Social Phobia');

--
-- Table structure for table `insurance`
--

DROP TABLE IF EXISTS `insurance`;
CREATE TABLE `insurance` (
	`insID` int(11) AUTO_INCREMENT NOT NULL,
	`name` varchar(50) NOT NULL,
	PRIMARY KEY (`insID`)
)ENGINE=InnoDB AUTO_INCREMENT=30000 DEFAULT CHARSET=latin1;

--
-- Example data for table `insurance`
--

INSERT INTO `insurance` (`name`) VALUES
	('Aetna'),
	('Kaiser Permanente'),
	('Blue Cross Blue Shield'),
	('United Healthgroup'),
	('Centura'),
	('Humana'),
	('HCSC'),
	('Cigna'),
	('Highmark'),
	('Coventry'),
	('HIP'),
	('Independence Blue Cross'),
	('Centene Corp'),
	('California Physicians Service'),
	('Wellpoint');

--
-- Table structure for table `therapists`
--

DROP TABLE IF EXISTS `therapists`;
CREATE TABLE `therapists` (
	`therID` int(11) AUTO_INCREMENT NOT NULL,
	`fname` varchar(50) NOT NULL,
	`lname` varchar(50) NOT NULL,
	`spec` int(11) NOT NULL,
	`sOrien` int(11) NOT NULL,
	`rel` int(11) NOT NULL, 
	`exp` int(11) NOT NULL,
	`loc` int(11) NOT NULL,
	`mail` varchar(320) NOT NULL, 	
	PRIMARY KEY (`therID`),
	FOREIGN KEY (`spec`) REFERENCES specialty (`specID`) ON DELETE CASCADE,
	FOREIGN KEY (`sOrien`) REFERENCES sexualOrientation (`sexID`) ON DELETE CASCADE,
	FOREIGN KEY (`rel`) REFERENCES religion (`relID`) ON DELETE CASCADE,
	FOREIGN KEY (`loc`) REFERENCES location (`locID`) ON DELETE CASCADE
)ENGINE=InnoDB AUTO_INCREMENT=40000 DEFAULT CHARSET=latin1;

--
-- Sample insert data for table `therapists`
--

INSERT INTO `therapists` (`fname`, `lname`, `spec`, `sOrien`, `rel`, `exp`, `loc`, `mail`) VALUES
	('Ricky', 'Smith', 20000, 2000, 10000, 2, 1000, 'example1@email.com'),
	('Bradley', 'Chubb', 20000, 2007, 10016, 10, 1019, 'example2@email.com'),
	('Marcus', 'Wyatt', 20001, 2002, 10001, 5, 1001, 'example3@email.com'),
	('Luca', 'Maestra', 20002, 2003, 10002, 4, 1002, 'example4@email.com'),
	('Ulysses', 'Jacobs', 20003, 2004, 10003, 6, 1003, 'example5@email.com'),
	('Kari', 'Jobe', 20004, 2005, 10004, 3, 1004, 'example6@email.com'),
	('John', 'Curry', 20005, 2006, 10005, 5, 1005, 'example7@email.com'),
	('Randy', 'Johnson', 20006, 2007, 10006, 8, 1006, 'example8@email.com'),
	('Jacques', 'Cousteau', 20007, 2000, 10007, 4, 1007, 'example9@email.com'),
	('Major', 'Applewhite', 20008, 2001, 10008, 1, 1008, 'example10@email.com'),
	('Kristopher', 'Alvarado', 20009, 2003, 10009, 4, 1009, 'example11@email.com'),
	('Leticia', 'Delgado', 20010, 2004, 10010, 6, 1010, 'example12@email.com'),
	('Joyce', 'Briggs', 20011, 2005, 10011, 3, 1011, 'example13@email.com'),
	('Sidney', 'Carlson', 20012, 2006, 10012, 5, 1012, 'example14@email.com'),
	('Nathaniel', 'Thornton', 20013, 2007, 10013, 8, 1013, 'example15@email.com'),
	('Larry', 'Kelley', 20014, 2000, 10014, 4, 1014, 'example16@email.com'),
	('Dwight', 'Martin', 20015, 2001, 10015, 1, 1015, 'example17@email.com'),
	('Neil', 'Hunt', 20016, 2002, 10016, 8, 1016, 'example18@email.com'),
	('Leslie', 'Barnes', 20017, 2003, 10000, 4, 1017, 'example19@email.com'),
	('Roman', 'Benson', 20018, 2004, 10001, 8, 1018, 'example20@email.com'),
	('Felicia', 'Caldwell', 20019, 2005, 10002, 4, 1019, 'example21@email.com'),
	('Jake', 'Collins', 20001, 2007, 10006, 8, 1000, 'example22@email.com'),
	('Robin', 'Houston', 20002, 2000, 10007, 4, 1000, 'example23@email.com'),
	('Vickie', 'Graham', 20003, 2007, 10006, 8, 1000, 'example24@email.com'),
	('Amos', 'Chandler', 20004, 2000, 10007, 4, 1000, 'example25@email.com'),
	('Tina', 'Gonzalez', 20005, 2007, 10006, 8, 1000, 'example26@email.com'),
	('Zachary', 'Ballard', 20006, 2000, 10007, 4, 1000, 'example27@email.com'),
	('Thelma', 'West', 20007, 2007, 10006, 8, 1000, 'example28@email.com'),
	('Doris', 'Strickland', 20008, 2000, 10007, 4, 1000, 'example29@email.com');

--
-- Table structure for table `therapist_insurance`
--

DROP TABLE IF EXISTS `therapist_insurance`;
CREATE TABLE `therapist_insurance` (
	`therID`int(11) NOT NULL,
	`insID` int(11) NOT NULL,
	FOREIGN KEY (`therID`) REFERENCES therapists (`therID`) ON DELETE CASCADE,
	FOREIGN KEY (`insID`) REFERENCES insurance (`insID`) ON DELETE CASCADE
)ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Sample insert data for table `therapist_insurance`
--

INSERT INTO `therapist_insurance` (`therID`, `insID`) VALUES
	(40000, 30014),
	(40001, 30013),
	(40002, 30012),
	(40003, 30011),
	(40004, 30010),
	(40005, 30009),
	(40006, 30008),
	(40007, 30007),
	(40008, 30006),
	(40009, 30005),
	(40009, 30004),
	(40010, 30003),
	(40011, 30002),
	(40012, 30001),
	(40013, 30000),
	(40014, 30014),
	(40015, 30013),
	(40016, 30012),
	(40017, 30011),
	(40018, 30010),
	(40019, 30009),
	(40020, 30008),
	(40021, 30002),
	(40022, 30001),
	(40023, 30000),
	(40024, 30014),
	(40025, 30013),
	(40026, 30012),
	(40027, 30011),
	(40028, 30010),
	(40000, 30009),
	(40001, 30008);

