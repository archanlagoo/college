DROP DATABASE bookstore_34;
CREATE DATABASE bookstore_34;

USE bookstore_34;

CREATE TABLE dim_book(
	bkey       VARCHAR(5),
	bname      VARCHAR(30) NOT NULL UNIQUE,
	bedition   INT(3)      NOT NULL,
	bprice     INT(10)     NOT NULL,
	bpublisher VARCHAR(30) NOT NULL,
	bauthor    VARCHAR(40) NOT NULL,
	PRIMARY KEY(bkey)
) ENGINE = InnoDB;
CREATE TABLE dim_time(
	tkey  VARCHAR(5),
	day   INT NOT NULL,
	week  INT NOT NULL,
	month INT NOT NULL,
	year  INT NOT NULL,
	PRIMARY KEY(tkey)
) ENGINE = InnoDB;
CREATE TABLE dim_category(
	ckey VARCHAR(5),
	cname VARCHAR(10) NOT NULL,
	cdiscount INT NOT NULL,
	PRIMARY KEY(ckey)
) ENGINE = InnoDB;
CREATE TABLE dim_store(
	skey     VARCHAR(5),
	sname    VARCHAR(30) NOT NULL UNIQUE,
	sarea    VARCHAR(30) NOT NULL,
	scity    VARCHAR(20) NOT NULL,
	sstate   VARCHAR(20) NOT NULL,
	PRIMARY KEY(skey)
) ENGINE = InnoDB;
CREATE TABLE dim_stock(
	stkey     VARCHAR(5),
	squantity INT NOT NULL,
	PRIMARY KEY(stkey)
) ENGINE = InnoDB;
CREATE TABLE fact_sales(
	bkey  VARCHAR(5) NOT NULL,
	ckey  VARCHAR(5) NOT NULL,
	tkey  VARCHAR(5) NOT NULL,
	skey  VARCHAR(5) NOT NULL,
	stkey VARCHAR(5) NOT NULL,
	quantity INT NOT NULL,
	sales    INT NOT NULL,
	profit   INT NOT NULL,
	FOREIGN KEY(bkey) REFERENCES dim_book(bkey),
	FOREIGN KEY(ckey) REFERENCES dim_category(ckey),
	FOREIGN KEY(tkey) REFERENCES dim_time(tkey),
	FOREIGN KEY(skey) REFERENCES dim_store(skey),
	FOREIGN KEY(stkey) REFERENCES dim_stock(stkey)
) ENGINE = InnoDB;
