# informix-tutorial-step-by-step-guide-for-beginners

## summary
This tutorial is for ```IBM Informix Developer Edition```, It will guide you step by step through runing a docker image, installing a client SDK, and connecting to database server.
I will put guides and sample code for connceting to this server from C/C++ and Perl clients on each folder above on this project.

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
raha@lab:~$docker run -it --name ifx -h ifx			\
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

```sql
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
if you want to start this mashine in future time just issue ```start``` command, DO NOT issue ```run``` again
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
informix@ifx:~$ dbaccess database commands.sql
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
informix@ifx:~$ echo "select * from customer" | dbaccess sales_demo

Database selected.

customer_code customer_name                   company_name

          102 Carole Sadler                   Sports Spot         

1 row(s) retrieved.

Database closed.
```

```sh
informix@ifx:~$ dbaccess sales_demo - <<EOT!
> insert into customer values (103, "John Doe", "John Company");
> select * from customer;
> EOT!

Database selected.

1 row(s) inserted.

customer_code customer_name                   company_name

          102 Carole Sadler                   Sports Spot         
          103 John Doe                        John Company        

2 row(s) retrieved.

Database closed.

```

## installing informix client sdk
Go to this link and download the client sdk for your platform, you should login, its free, and may need vpn and some luck :)

>[IBM Informix Developer Tools](https://www.ibm.com/products/informix/developer-tools)
I have used ```clientsdk.4.10.FC14.linux-x86_64.tar``` then extract the archive an run installer script
```sh
raha@lab:~$tar -xvf clientsdk.4.10.FC14.linux-x86_64.tar
raha@lab:~$./installclientsdk
```
it will ask you a series of question which you may accept their defaults 
lets assume you have installed it on ```/opt/ibm/informix```
look around in ```/opt/ibm/informix``` there is 
* bin for excutable
* etc for informix client configs
* incl for header files
* docs & demo which have some useful informations

ok let's configure some enviroment variable to tell the system where to find this SDK
first add these line to the end of ```.profile``` file in your home directory
```sh
if [ -d "/opt/ibm/informix/bin" ] ; then
	export INFORMIXSERVER=informix
	export INFORMIXDIR=/opt/ibm/informix
	PATH="/opt/ibm/informix/bin:$PATH"
fi
```
```sh
raha@lab:~$vi .profile
raha@lab:~$source .profile
```

now you will be able to start dbaccess from your host machine
```sh
raha@lab:~$dbaccess - -
```
but still you cannot connect to your docker instance, so bear with me.
## Connecting to Server from outside world
```sh
raha@lab:~$sudo docker start ifx
raha@lab:~$sudo docker exec -it ifx bash
```
now check your informix server IP address for me its ```172.17.0.2``` you may have different address
```sh
informix@ifx:~$ ip address show
# or
informix@ifx:~$ ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.17.0.2  netmask 255.255.0.0  broadcast 172.17.255.255
        ether 02:42:ac:11:00:02  txqueuelen 0  (Ethernet)
        RX packets 73  bytes 10280 (10.0 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```
now try to ping this address from your host machine
```sh
raha@lab:~$ ping 172.17.0.2
PING 172.17.0.2 (172.17.0.2) 56(84) bytes of data.
64 bytes from 172.17.0.2: icmp_seq=1 ttl=64 time=0.177 ms
64 bytes from 172.17.0.2: icmp_seq=2 ttl=64 time=0.108 ms
64 bytes from 172.17.0.2: icmp_seq=3 ttl=64 time=0.126 ms
^C
--- 172.17.0.2 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2051ms
rtt min/avg/max/mdev = 0.108/0.137/0.177/0.029 ms
```
so everything looks good. lets tell the linux the name of this machine
```sh
sudo cp -vf /etc/hosts /etc/hosts.old
echo "172.17.0.2 ifx" | sudo tee -a /etc/hosts
```
now edit this ```/opt/ibm/informix/etc/sqlhosts``` file
```sh
raha@lab:~$sudo cp -vf /opt/ibm/informix/etc/sqlhosts /opt/ibm/informix/etc/sqlhosts.old
raha@lab:~$sudo cat > test.txt<<EOT!
informix        onsoctcp        ifx         9088
informix_dr     drsoctcp        ifx         9089
EOT!
```
the result should look likes these lines
```sh
raha@lab:~$ cat /opt/ibm/informix/etc/sqlhosts
informix        onsoctcp        ifx         9088
informix_dr     drsoctcp        ifx         9089
```
here we are telling client side that the informix database server ```@informix``` on machine ```ifx``` listening to port ```9088``` for incomming tcp connections.
ok we are ready to connect to our database server.
issue dbaccess on you host machine, and then conncet to the database, the password for user ```informix``` is ```in4mix```
```sh
informix@ifx:~$ dbaccess - -
> connect to '@informix' user 'informix';
   ENTER PASSWORD:

Connected.

```
we have done now, you can access this database server on your host machine. 

