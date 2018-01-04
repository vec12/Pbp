use salon_namestaja;

insert into zaposleni values(1, 'Pera', 'Peric', 'Bulevar 35', '1995-10-10', '1010995715342'),
       	    	      (2, 'Hary', 'Hacker', 'Milutina Milankovica 14', '1989-11-11', '1111989574343');
-- select * from zaposleni;

insert into montazer values(1000, 'viseci-element', 1),
							(100, 'opsta', 2);
-- select * from montazer;

insert into prodavnica values(1, 'Salon1', 'Bulevar 44', 'Beograd', '10:00-21:00'),
       	    	       (2, 'Salon2', 'Pariska 12', 'Beograd', '9:00-21:00');

-- select * from prodavnica;

insert into prodavac values(45000, 1, 1);
-- select * from prodavac;

insert into namestaj values(1, 'kredenac-mali', 'viseci-element', 1000, 'kuhinja'),
       	    	     (2, 'kredenac-veliki', 'viseci-element' , 3000, 'kuhinja'),
		     (3, 'trp-sto', 'opsta', 2000, 'trpezarija'),
		     (4, 'bracni-krevet', 'krevet', 10000, 'spavaca_soba');

insert into izlaze values(5, 1, 1),
       	    	   (3, 2, 1),
		   (1, 3, 1),
		   (1, 4, 1),
		   (4, 3, 2),
		   (4, 4, 2);

insert into kupac values(1, 'Nikola', 'Nikolic', 0.5),
       	    	  (2, 'Miky', 'Mouse', null);

-- select * from kupac;

insert into kupovina values(1, '2017-12-10', 0 , 1),
       	    	     (2, '2017-11-14', 0, 2);

insert into planDostave values(1, 'Dr Ivana Ribara 144', 2, '2017-12-10'),
       	    		(2, 'Dr Ivana Ribara 144', 2, '2017-12-12'),
       	    		(3, 'Dr Ivana Ribara 45', 2, '2017-12-20');

insert into kupljenNamestaj values(1, 1, 1),
       	    		    (2, 1, 1),
			    (3, 1, 2);
			    -- (4, 2, '2017-12-20');
select * from kupovina;
