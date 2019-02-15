USE bookstore_34;

/* Roll Up */
SELECT 'Roll up on year';
SELECT SUM(fact_sales.quantity) FROM fact_sales
	INNER JOIN dim_time ON fact_sales.tkey = dim_book.tkey
GROUP BY dim_book.year;

/* Slice */
SELECT 'Slice on time 1/5/2018';
SELECT SUM(quantity) FROM fact_sales
	WHERE tkey IN (SELECT tkey FROM dim_time WHERE day=1 AND month=5 AND year=2018)
;

/* Dice */
SELECT 'Dice on category and publisher';
SELECT SUM(quantity) FROM fact_sales WHERE
	ckey IN (SELECT ckey FROM dim_category WHERE cname='Digital') AND
	bkey IN (SELECT bkey FROM dim_book WHERE bpublisher='Times Publication')
;
