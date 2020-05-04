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
docker pull ibmcom/informix-developer-database
```
after download completed issue following command to set up the image for the first time. ```--name ifx``` will be the name of this image for future refrence, ```-h ifx``` will be the name of machine host, donot miss the ```-e LICENSE=accept``` part or your server will not works!

```sh
docker run -it --name ifx -h ifx					\
      -p 9088:9088                                  \
      -p 9089:9089                                  \
      -p 27017:27017                                \ 
      -p 27018:27018                                \ 
      -p 27883:27883                                \ 
      -e LICENSE=accept                             \
      ibmcom/informix-developer-database:latest
```
if everything goes well you will have a working server by now, lets see.
Issue following command to get a bash shell on your docker.

```sh
docker exec -it ifx bash
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
syscdr_state       syscdrack_buf      syscdrack_txn      syscdrcntrlqprog  
syscdrctrl_buf     syscdrctrl_txn     syscdrlatency      syscdrprog        
syscdrq            syscdrrecv_buf     syscdrrecv_stats   syscdrrecv_txn    
syscdrrecvqprog    syscdrs            syscdrsend_buf     syscdrsend_txn    
syscdrsync_buf     syscdrsync_txn     syscdrtsapply      syscdrtx          
syscfgtab          syscheckpoint      syschfree          syschkextents     
syschkio           syschktab          syschktab_fast     syschunks         
syschunks_fast     sysckptinfo        syscluster         syscmsm           
syscmsmsla         syscmsmtab         syscmsmunit        syscompdicts      
syscompdicts_full  sysconblock        sysconditions      sysconfig         
sysconlst          sysconq            syscrtadt          sysdatabases      
sysdbslocale       sysdbspaces        sysdbspartn        sysdbstab         
sysdic             sysdiccache        sysdistcache       sysdrcb           
sysdri             sysdsc             sysdual            sysdwacoord       
sysenv             sysenvses          sysextents         sysextspaces      
sysfeatures        sysfileinfo        sysgrid            sysha_lagtime     
sysha_nodes        sysha_type         sysha_workload     sysiohistory      
sysipl             syslcktab          syslicenseinfo     syslocks          
syslocktab         syslogfil          syslogs            syslowmemorymgr   
syslrus            sysmachineinfo     sysmchktab         sysmchktab_fast   
sysmgmgates        sysmgminfo         sysmgmquery        sysmtxlst         
sysmutexes         sysmutq            sysnetclienttype   sysnetglobal      
sysnetworkio       sysonlinelog       sysopendb          syspaghdr         
sysphyspaghdrs     sysplog            syspoollst         syspools          
sysprc             sysprobecolumns    sysprobejds        sysprobejps       
sysprobetables     sysproccache       sysprof_address    sysprof_func      
sysprof_percentage sysprofile         sysproftab         sysproxyagents    
sysproxydistribut+ sysproxysessions   sysproxytxnops     sysproxytxns      
sysptnbit          sysptncol          sysptnext          sysptnhdr         
sysptnkey          sysptntab          sysptprof          sysrawdsk         
sysreorgops        sysrepevtreg       sysrepstats        sysrsslog         
sysrstcb           sysscblst          syssdblock         sysseglst         
syssegments        syssesappinfo      syssesprof         syssessions       
sysseswts          sysshmem           sysshmhdr          sysshmvals        
syssltdat          sysslttab          syssmx             syssmxses         
syssqexplain       syssqlcacheprof    syssqlcurall       syssqlcurses      
syssqlhosts        syssqlstat         syssqltrace        syssqltrace_hvar  
syssqltrace_info   syssqltrace_iter   syssqscb           syssrcrss         
syssrcsds          syssscelem         sysstoragemgr      syssymtab         
systabextents      systabinfo         systabnames        systabpagtypes    
systcblst          systhreads         systhreadwaits     systraces         
systrans           systrgrss          systrgsds          systwaits         
systxptab          sysuserthreads     sysvplst           sysvpprof         
sysxatab           

```

ok we are done now, our server is running in the folloeing section we will learn some informix sql basics.

## dbaccess, dbschema and sql
## installing informix client sdk
## Connecting to Server from outside world
## summary
