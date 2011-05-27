Dependencies
============
You will need LDNS and Mysql++

http://tangentsoft.net/mysql++/

http://www.nlnetlabs.nl/projects/ldns/

Create the database
===================
shell> create database ulisses_db;

Create the table
================
mysql> create table dnsperf (webserver VARCHAR(30), avgqtime INT, nrq INT, timefst VARCHAR(30), timelst VARCHAR(30));

Output
======
```c
mysql> select * from dnsperf;
+---------------+----------+------+------------+------------+
| webserver     | avgqtime | nrq  | timefst    | timelst    |
+---------------+----------+------+------------+------------+
| google.com    |       50 |  149 | 1306535380 | 1306539552 |
| facebook.com  |      144 |  149 | 1306535380 | 1306539552 |
| youtube.com   |       53 |  149 | 1306535380 | 1306539552 |
| yahoo.com     |      519 |  149 | 1306535382 | 1306539552 |
| live.com      |       91 |  149 | 1306535382 | 1306539552 |
| wikipedia.org |      160 |  149 | 1306535382 | 1306539553 |
| baidu.com     |     1097 |  149 | 1306535383 | 1306539553 |
| blogger.com   |       99 |  149 | 1306535383 | 1306539554 |
| msn.com       |      230 |  149 | 1306535383 | 1306539554 |
| qq.com        |     1110 |  149 | 1306535385 | 1306539554 |
+---------------+----------+------+------------+------------+
10 rows in set (0.00 sec)
```

