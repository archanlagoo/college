USE bookstore_34;

/* Show the entire fact table */
SELECT 'Entire table';
SELECT * FROM fact_sales;

/*  Sales per bookstore */
SELECT 'Sales per bookstore';
SELECT SUM(fact_sales.profit) AS profit, SUM(fact_sales.quantity) AS quantity, SUM(fact_sales.sales) AS sales, dim_store.scity AS city FROM fact_sales INNER JOIN dim_store GROUP BY(dim_store.scity); 

/* Show number of books sold by the author J.K.Rowling */
SELECT SUM(quantity) AS books_sold_by_JK_Rowling FROM fact_sales WHERE bkey IN (SELECT bkey FROM dim_book WHERE bauthor='J. K. Rowling');

/* Show profit from used books */
SELECT SUM(profit) AS profit_from_used_books FROM fact_sales WHERE ckey IN (SELECT ckey FROM dim_category WHERE cname='Used');

/* Show the sales from Times Publication in Mumbai */
SELECT SUM(sales) AS sales_from_times FROM fact_sales WHERE bkey IN (SELECT bkey FROM dim_book WHERE bpublisher='Times Publication');

/* Show the number of books sold on 1/5/2018 */
SELECT SUM(quantity) AS books_on_1_5_18 FROM fact_sales WHERE tkey IN (SELECT tkey FROM dim_time WHERE day = 1 AND month = 5 AND year = 2018);
