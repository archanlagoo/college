source ddl.sql;

INSERT INTO dim_book VALUES
	('B001','Harry Potter', 5, 6, 'MAgic World Publication', 'J. K. Rowling'),
	('B002','Chronicles of Narnia', 5, 6, 'Penguin Publication', 'C.W.Lewis'),
	('B003','Hunger Games', 5, 6, 'Ketan Publication', 'Susuanne Colling'),
	('B004','Precy Jackson', 5, 6, 'Times Publication', 'Rick Riordan')
;
INSERT INTO dim_time VALUES
	('T001', 1,1,5,2018),
	('T002', 2,2,6,2018),
	('T003', 3,3,3,2017),
	('T004', 4,4,4,2017)
;
INSERT INTO dim_category VALUES
	('C001','Used',60),
	('C002','New',0),
	('C003','Digital',75),
	('C004','Rental',70)
;
INSERT INTO dim_stock VALUES
	('S001',10),
	('S002',15),
	('S003',11),
	('S004',12)
;	
INSERT INTO dim_store VALUES
	('BS001','Somaiya BS','Vidhyavihar','Mumbai','Maharashtra'),
	('BS002','Bhagat BS','Noida','Delhi','Delhi'),
	('BS003','gandhi Son BS','Maninagar','Ahmedabed','Gujrat'),
	('BS004','Mithibai BS','Vileparle','Mumbai','Maharashtra')
;
INSERT INTO fact_sales VALUES
	('B001','C001','T003','BS004','S004',30,15,750),
	('B002','C002','T001','BS002','S002',20,15,600),
	('B003','C002','T004','BS001','S001',17,13,650),
	('B004','C003','T001','BS003','S003',30,10,1000)
;
