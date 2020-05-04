# informix-tutorial-step-by-step-guide-for-beginners
This tutorial is for ```IBM Informix Developer Edition```, It will guide you step by step through runing a docker image, installing a client SDK, and connecting to database server.

```IBM Informix Developer Edition```  is free database software for application development and prototyping.

Last week I started to learn @Informix for a project in my work place, but unfortunately I did not find any quik stat guide so I decided to write this short tutorial.

## Server Installation & Configuration
The simple and fast way to have an informix developer edition server running is by using prepared docker image.
you can download such image from Docker Hub.
there is a ``` informix dockerhub readme``` on github that has compelete guide on how to use different version of docker images.

>[IBM Informix Developer Edition Docker Containers](https://hub.docker.com/r/ibmcom/informix-developer-database/)
>[informix dockerhub readme](https://github.com/informix/informix-dockerhub-readme)

I will use the latest version which was 14.10.FC1 at this time, but I will guess using other image is quite similar.
I am using LUbuntu 18.04, you can find tutorial on how to install and use Docker anywhere on the web, for example.

>[how to install and use docker on ubuntu 18.04](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04)

issue following command on your terminal to download image

```sh
raha@lab:~$docker pull ibmcom/informix-developer-database
```
after download completed issue following command to set up the image for the first time. ```--name ifx``` will be the name of this image for future refrence, ```-h ifx``` will be the name of machine host, donot miss the ```-e LICENSE=accept``` part or your server will not works!

```sh
raha@lab:~$docker run -it --name ifx -h ifx					\
      -p 9088:9088                                  \
      -p 9089:9089                                  \
      -p 27017:27017                                \ 
      -p 27018:27018                                \ 
      -p 27883:27883                                \ 
      -e LICENSE=accept                             \
      ibmcom/informix-developer-database:latest
```
if everything goes well you will have a working server by now, lets see.
open another terminal and issue following command to get a bash shell on your docker.

```sh
raha@lab:~$docker exec -it ifx bash
```
you will get following prompt, for user informix on ifx host.

```sh
informix@ifx:~$
```
ok now a quiek test to see if server is running, Issue dbaccess command with two seperate dash to enter in cli mode. and you will get dbaccess prompt.
at db-access prompt issue ```database SysMaster;``` to select SysMaster database.

```sh
informix@ifx:~$ dbaccess - -
> database SysMaster;

Database selected.
```

then again issu ```INFO TABLES;``` to see list of tables on this database.

```sh
> INFO TABLES;


Table name

flags_text         logmessage         smi_build_status   sysactptnhdr      
sysadtinfo         sysaqt             sysaqtjp           sysaqttab         
sysaudit           sysbaract_log      sysbtcreq          sysbufhdr         
sysbufpool         sysbufprofile      syscdr_ats         syscdr_atsdir     
syscdr_ddr         syscdr_nif         syscdr_rcv         syscdr_ris        
syscdr_risdir      syscdr_rqm         syscdr_rqmhandle   syscdr_rqmstamp   
...

```
if you are getting ```bash: dbaccess: command not found``` when issuing dbaccess, you should update your PATH environment variable. 

```sh
informix@ifx:~$cd /home/informix
informix@ifx:~$vi .profile
```
then add these line to the end of  ```.profile``` file. assuming informix in docker image installed on ```/opt/ibm/informix```

```sh
if [ -d "/opt/ibm/informix/bin" ] ; then
    PATH="/opt/ibm/informix/bin:$PATH"
fi
```
then issue

```sh
informix@ifx:~$source .profile
```

you can type exit to exit the docker shell.
```sh
informix@ifx:~$exit
```

if you want to shutdown the docker instance, you have two option.
either press ctrl+c on the same terminal that you initially run it, or issue following command on another terminal


```sh
raha@lab:~$docker stop ifx 
```
if you want to start this mashine for the future time just issue start command
```sh
raha@lab:~$docker start ifx 
```
if you provide ```-i``` option you will see some logs on your terminal which may be helpful later for debugging

```sh
raha@lab:~$docker start -i ifx 
```

to see the list of available docker instance on your system issue following command.
```sh
raha@lab:~$docker ps -a
```
ok we are done now, our server is running in the following section we will learn some informix sql basics.

## dbaccess, dbschema and sql
```sh
raha@lab:~$docker start ifx 
```
```sh
raha@lab:~$docker exec -it ifx bash
```

```sh
informix@ifx:~$ dbaccess - -
```

```sql
> create database sales_demo;

Database created.
```

```sql
> create table customer (
 customer_code integer,
 customer_name char(31),
 company_name char(20)
);
> > > > 
Table created.
```
```sql
> insert into customer values (102, "Carole Sadler", "Sports Spot");

1 row(s) inserted.
```
```sql
> select * from customer;


customer_code customer_name                   company_name

          102 Carole Sadler                   Sports Spot         

1 row(s) retrieved.
```
press ctrl+d to exit dbaccess

```sh
informix@ifx:~$ dbschema -d sales_demo -t customer

DBSCHEMA Schema Utility       INFORMIX-SQL Version 14.10.FC3

{ TABLE "informix".customer row size = 55 number of columns = 3 index size = 0 }

create table "informix".customer 
  (
    customer_code integer,
    customer_name char(31),
    company_name char(20)
  );

revoke all on "informix".customer from "public" as "informix";

```
```sh
informix@ifx:~$ dbaccess - -
```
```sql
> database sales_demo;

Database selected.
```

```sql
> database sales_demo;

Database selected.
```

```sql
> INFO TABLES;


Table name

customer
```

```sql
> INFO COLUMNS FOR customer;

Column name          Type                                    Nulls

customer_code        integer                                 yes
customer_name        char(31)                                yes
company_name         char(20)                                yes
```

```sql
> INFO INDEXES FOR customer;

Index_name         Owner    Type/Clstr Access_Method      Columns
```

```sh
informix@ifx:~$ dbaccess sales_demo - <<EOT!
select * from customer;
EOT!

Database selected.



customer_code customer_name                   company_name         

          102 Carole Sadler                   Sports Spot         

1 row(s) retrieved.



Database closed.

```

## installing informix client sdk
## Connecting to Server from outside world
## summary
