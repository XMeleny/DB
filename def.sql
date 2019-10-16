create table GOODS
(
goods_id smallint unsigned auto_increment primary key,
goods_name varchar(40),
amount smallint unsigned,
cost float,
price float,
kind varchar(20)
);

create table CUSTOMERS
(
customer_id char(11) primary key,
customer_name varchar(40),
psw varchar(20),
money float(10,2) unsigned
);


--  外键要先定义，再标记
/* create table ORDERS
(
order_id smallint unsigned,
goods_id smallint unsigned,
customer_id char(11),
foreign key(customer_id) references CUSTOMERS(customer_id),
price float,
timing timestamp,
address varchar(100),
activity_id smallint,
primary key (order_id,goods_id) 
); */

create table ORDERS
(
order_id smallint unsigned,
goods_id_list varchar(100),
customer_id char(11),
goods_amount_list varchar(100),
totoal float,
timing date,
address varchar(100),
activity_id smallint,
foreign key(customer_id) references CUSTOMERS(customer_id),
primary key (order_id) 
);

create table DISCOUNTS
(
discount_id smallint unsigned primary key,
discount_name tinytext,
kind varchar(40),
start_time date,
end_time date
);


create table SHOPPING_CHARTS
(
goods_id smallint unsigned,
customer_id char(11),
foreign key (goods_id) references GOODS(goods_id),
foreign key (customer_id) references CUSTOMERS(customer_id),
money float,
amount smallint,
primary key(customer_id,goods_id)
);


create table STOCKS
(
stock_id smallint unsigned primary key auto_increment,
stock_name varchar(40),
cost float
);

create table INSTOCK_RECORDS
(
record_id smallint unsigned primary key auto_increment,
record_name varchar(40),
amount smallint unsigned,
timing date,
cost float
);


