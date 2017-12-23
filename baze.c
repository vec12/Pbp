#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <stdarg.h>
#include <errno.h>

static void error_fatal (char *format, ...);
void connectToDatabase(MYSQL **konekcija, const char *host, const char *user, const char *pass, const char *db);
void printResult(void);
char showMenu(void);
void selectQuery();
void insertQuery(void);
void deleteQuery(void);
void addEmployee();
void listShop();
void showFur();
void buyQuery();
void showBuy(void);


char query[1024];
MYSQL *konekcija;
MYSQL_RES *rezultat;
MYSQL_ROW red;
MYSQL_FIELD *polje;

int main(int argc, char **argv)
{
	char buffer[80];

	connectToDatabase(&konekcija, "localhost", "ismer", "ismer", "salon_namestaja");
	while(1){
		showMenu();
		printf("Da li yelite da nastavite rad sa bazom?(y/n)\n");
		char c = getchar();
		printf("%c\n", c);
		if(c == 'n')
			break;
	}

	mysql_free_result(rezultat);
	mysql_close(konekcija);

	exit(EXIT_SUCCESS);
		
}

void printResult(void)
{
	int i;
	int broj;
	
	rezultat = mysql_use_result(konekcija);
	polje = mysql_fetch_field(rezultat);
	broj = mysql_num_fields(rezultat);
	/* printf("%d _______\n", broj); */
	for(i = 0; i < broj; i++) {
		printf("%-12s\t\t", polje[i].name);
	}
	printf("\n\n");
	/* printf("%s\n", (mysql_fetch_field(rezultat))[0].name); */

	while((red = mysql_fetch_row(rezultat)) != 0) {
		for(i = 0; i < broj; i++) {
			printf("%-12s\t\t", red[i]);
		}
		printf("\n");

	}
	printf("\n");
	
}

char showMenu(void)
{
	printf("Izaberite opciju:\ns- Prikaz stanja baze\nz- Dodaj novog zaposlenog\nk - kupi namestaj\nd- Brisanje\n");
	char c = getchar();
	/* printf("%c\n", c); */
	switch(c) {
	case 's':
		selectQuery();
		break;
	case 'z':
		addEmployee();
		/* insertQuery(); */
		break;
	case 'k':
		buyQuery();
		/* editQuery(); */
		break;
	case 'd':
		deleteQuery();
		break;
	}

}

void addEmployee()
{
	
	char name[80];
	char prezime[80], datum_rodjenja[80], jmbg[80], vrsta_namestaja[80];
	int dnevnica, id, idProdavnice, plata;
	printf("Unesite id zaposlenog\n");
	scanf("%d", &id);
	printf("Unesite ime zaposlenog\n");
	scanf("%s", name);

	/* printf("%s %d\n", name, id); */

	printf("Unesite prezime zaposlenog\n");
	scanf("%s", prezime);
	printf("Unesite adresu zaposlenog\n");
	/* scanf("%s", adresa); */
	char *adresa = NULL;
	size_t size;
	getchar();
	if (getline(&adresa, &size, stdin) == -1) {
		printf("No line\n");
	}
	free(adresa);
	
	printf("Unesite datum rodjenja zaposlenog\n");
	scanf("%s", datum_rodjenja);
	printf("Unesite jmbg zaposlenog\n");
	scanf("%s", jmbg);
	printf("Unesite vrstu zaposlenog (montazer/prodavac)\n");
	getchar();
	char c = getchar();
	if (c == 'm') {
		printf("Unesite dnevnicu zaposlenog\n");
		scanf("%d", &dnevnica);
		printf("Unesite vrstu namestaja koju montira zaposleni\n");
		scanf("%s", vrsta_namestaja);
	} else {
		listShop();
		printf("Unesite mesecnu platu zaposlenog\n");
		scanf("%d", &plata);
		printf("Unestite id prodavnice\n");
		scanf("%d", &idProdavnice);
		sprintf(query, "insert into prodavac values(%d, %d, %d)", plata, id, idProdavnice);
	}

	sprintf(query, "insert into zaposleni values(%d, \"%s\", \"%s\", \"%s\", \"%s\", \"%s\")", id, name, prezime, adresa, datum_rodjenja, jmbg);

	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}
	printf("%s \n", query);
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

}

void listShop()
{
	sprintf(query, "select * from prodavnica");
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printResult();

}

void showFur()
{
	sprintf(query, "select * from namestaj");
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printResult();
}
void showBuy()
{
	sprintf(query, "select * from kupac");
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printResult();
	
}

void buyQuery()
{
	int idKupca, idNamestaja, idKupovine;
	char datum[80];
	
	/* showFur(); */
	showBuy();
	printf("Unesite id kupca\n");
	scanf("%d", &idKupca);

	printf("Unesite datum kupovine\n");
	scanf("%s", datum);

	printf("Unestite id kupovine\n");
	scanf("%d", &idKupovine);

	sprintf(query, "insert into kupovina values(%d, \"%s\", 0, %d)", idKupovine, datum, idKupca);
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printf("Molimo vas napravite plan dostave\n");

	char dostava[80];
	int idMontazera;
	printf("Unesite datum dostave\n");
	scanf("%s", dostava);

	printf("Unesite adresu\n");
	/* scanf("%s", adresa); */
	char *adresa = NULL;
	size_t size;
	getchar();
	if (getline(&adresa, &size, stdin) == -1) {
		printf("No line\n");
	}
	free(adresa);

	printf("Unesite id montazera\n");
	scanf("%d", &idMontazera);

	sprintf(query, "insert into planDostave values(\"%s\", \"%s\", %d)", dostava, adresa, idMontazera);
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}


	printf("Izaberite namestaj\n");
	char query1[1024];
	int end = 0;
	sprintf(query1, "insert into kupljenNamestaj values");
	while(!end) {
		showFur();
		printf("Unestite id namestaja koji zelite da kupite\n");
		scanf("%d", &idNamestaja);
		char query2[1024];
		
		sprintf(query2, " (%d, %d, \"%s\")",idNamestaja, idKupovine, dostava);
		strcat(query1, query2);

		printf("Da li zelite da kupite jos neki namestaj (y/n)\n");
		getchar();
		char c = getchar();
		if(c == 'n') {
			end = 1;
		} else {
			strcat(query1, ",");
		}

		
	}

	sprintf(query, "%s", query1);
	printf("%s -------", query);
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}
	
}

void selectQuery()
{
	printf("Unesite ime tabele koju zelite da vidite\n");
	char tableName[256];
	scanf("%s", tableName);

	printf("Unesite imena polja koja zelite da vidite ili * za sva polja\n");
	char *line = NULL;
	size_t size;
	getchar();
	if (getline(&line, &size, stdin) == -1) {
		printf("No line\n");
	} else {
		/* printf("%s\n", line); */
		sprintf(query, "select %s from salon_namestaja.%s", line, tableName);

	}

	char *line2 = NULL;
	size = 0;
	
	printf("Unestite dodatan uslov pretrage\n");
	if(getline(&line2, &size, stdin) > 1) {
		strcat(strcat(query," where "), line2);
	}
	
	char *line3 = NULL;
	size = 0;
	
	printf("Unesite kolonu po kojoj zelite da sortirate rezultat\n");
	if(getline(&line3, &size, stdin) > 1) {
		printf("%s\n", line3);
		strcat(query, " order by ");
		strcat(query, line3);
	}
	free(line);
	free(line2);
	free(line3);

	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printResult();

	printf("%s\n", query);
	
}

void insertQuery(void)
{
	printf("Unesite ime tabele u koju zelite da unesete podatke\n");
	char tableName[256];
	scanf("%s", tableName);

	printf("Unesite podatke koje treba uneti u tabelu\n");
	getchar();
	char * line = NULL;
	size_t size;
	if(getline(&line, &size, stdin) > 1) {
		strcat(query, "insert into ");
		strcat(query, tableName);
		strcat(query, " values(");
		strcat(query, line);
		strcat(query, ")");
	}
	printf("%s\n", query);

	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printf("Podaci su uspesno uneti\n");

}

void deleteQuery(void)
{
	printf("Unesite ime tabele iz koje zelite da brisete podatke\n");
	char tableName[256];
	scanf("%s", tableName);

	sprintf(query, "delete from salon_namestaja.%s", tableName);

	char *line2 = NULL;
	size_t size;
	
	printf("Unestite dodatan uslov pretrage\n");
	getchar();
	if(getline(&line2, &size, stdin) > 1) {
		strcat(strcat(query," where "), line2);
	}

	printf("%s\n", query);
	if(mysql_query(konekcija, query) != 0) {
		error_fatal("Greska u upitu %s\n", mysql_error(konekcija));
	}

	printf("Podaci su uspesno izbrisan\n");

}

void connectToDatabase(MYSQL **konekcija, const char *host, const char *user, const char *pass, const char *db)
{
	*konekcija = mysql_init(NULL);

	if(mysql_real_connect(*konekcija, host, user, pass, db, 0, NULL, 0) == NULL) {
		error_fatal("Greska u konekciji %s\n", mysql_error(*konekcija));
	}
		
}

static void error_fatal (char *format, ...)
{
	va_list arguments;

	va_start (arguments, format);
	vfprintf(stderr, format, arguments);
	va_end (arguments);

	

	exit(EXIT_FAILURE);
}
