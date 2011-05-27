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
