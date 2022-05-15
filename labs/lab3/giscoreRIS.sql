show con_name
create user U2_GIS_PDB identified by dfyef1102 container=current;
grant connect to U2_GIS_PDB;
grant all privileges to U2_GIS_PDB; 
GRANT CREATE DATABASE LINK TO U1_GIS_PDB;
GRANT CREATE PUBLIC DATABASE LINK TO U1_GIS_PDB;
GRANT CREATE DATABASE LINK TO U2_GIS_PDB;
GRANT CREATE PUBLIC DATABASE LINK TO U2_GIS_PDB;

------------------------------
create table GISCORERIS(
  x int primary key
);
 insert into GISCORERIS values(5);
 select * from GISCORERIS;
 commit;

 insert into GISCORERIS values(5);
commit


