drop database if exists salon_namestaja;
create database if not exists salon_namestaja;
use salon_namestaja;
create table zaposleni (
       idZaposlenog int not null primary key,
    ime varchar(45) not null,
    prezime varchar(45) not null,
    adresa varchar(45) not null,
    datum_rodjenja date,
    jmbg char(13)
);

create table montazer (
       dnevnica int not null,
       vrsta_namestaja varchar(45),
       id_zaposlenog int not null,
       foreign key (id_zaposlenog) references zaposleni (idZaposlenog) on delete cascade on update cascade
);

create table prodavnica (
       idProdavnice int not null primary key,
       ime varchar(45) not null,
       adresa varchar(45) not null,
       grad varchar(45) not null,
       radno_vreme varchar(45) not null
);

create table prodavac (
       mesecna_plata int not null,
       id_zaposlenog int not null,
       prodavnica int not null,
       foreign key (id_zaposlenog) references zaposleni (idZaposlenog) on delete cascade on update cascade,
       foreign key (prodavnica) references prodavnica (idProdavnice) 
);

create table namestaj (
       idNamestaja int not null primary key,
       ime varchar(45) not null,
       vrsta_namestaja varchar(45) not null,
       cena int not null,
       kategorija varchar(45) not null
);

create table izlaze (
       kolicina int not null,
       id_namestaja int not null,
       id_prodavnice int not null,
       primary key (id_namestaja, id_prodavnice),
       foreign key (id_namestaja) references namestaj (idNamestaja),
       foreign key (id_prodavnice) references prodavnica (idProdavnice)
);

create table kupac (
       idKupca int not null primary key,
       ime varchar(45) not null,
       prezime varchar(45) not null,
       popust double
       );

create table kupovina (
       idKupovine int not null primary key,
       datum date not null,
       cena double not null default 0,
       id_kupca int not null,
       foreign key (id_kupca) references kupac(idKupca)
       );

create table planDostave (
       datum date not null primary key,
       adresa varchar(45) not null,
       id_montazera int not null,
       foreign key (id_montazera) references montazer(id_zaposlenog)
);

create table kupljenNamestaj (
       id_namestaja int not null,
       id_kupovine int not null,
       datum date not null,
       primary key (id_namestaja, id_kupovine),
       foreign key (id_namestaja) references namestaj (idNamestaja),
       foreign key (id_kupovine) references kupovina (idKupovine),
       foreign key (datum) references planDostave (datum)
);

delimiter $$
create trigger cena_trigger before insert on kupljenNamestaj
       for each row
       	   begin
		update kupovina set  kupovina.cena = kupovina.cena +
		       		     (select n.cena - n.cena * coalesce(popust, 0)
		    		      from namestaj n, kupac k
				      where new.id_namestaja = n.idNamestaja and
				      	    kupovina.id_kupca = k.idKupca)
		where kupovina.idKupovine = new.id_kupovine;
	   end
$$

create trigger montazer_trigger before insert on kupljenNamestaj
       for each row
       	   begin
		declare vrsta varchar(45);
		declare vrstaMon varchar(45);
		set vrsta = (select vrsta_namestaja
		    	    from namestaj n
			    where n.idNamestaja = new.id_namestaja);
		set vrstaMon = (select vrsta_namestaja
		    	       from montazer mon
			       where mon.id_zaposlenog = (select id_montazera
			       	     		       	  from planDostave pd
							  where pd.datum = new.datum));
		if((vrsta != vrstaMon) && (vrsta not like 'opsta')) then
			 signal sqlstate '45000' set message_text='montazer ne moze montirati izabrani namestaj';
		end if;
	   end
$$
delimiter ;
