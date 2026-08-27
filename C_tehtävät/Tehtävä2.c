#include <stdio.h>

int main(void)
{
	double tulot[12];
	double verot[12];
	double veroprosentti;
	double tuloraja;
	double korkeampiVeroprosentti;
	double vuositulot = 0.0;

	printf("anna veroprosentti: ");
	scanf("%lf", &veroprosentti);

	printf("anna tuloraja: ");
	scanf("%lf", &tuloraja);

	printf("anna tulorajan ylittavien tulojen veroprosentti: ");
	scanf("%lf", &korkeampiVeroprosentti);

	for (int kuukausi = 0; kuukausi < 12; kuukausi++) {
		printf("anna kuukauden %d tulot: ", kuukausi + 1);
		scanf("%lf", &tulot[kuukausi]);
	}

	for (int kuukausi = 0; kuukausi < 12; kuukausi++) {
		double tulorajanAlla = 0.0;
		double tulorajanYli;

		if (vuositulot < tuloraja) {
			tulorajanAlla = tulot[kuukausi];
			if (vuositulot + tulorajanAlla > tuloraja) {
				tulorajanAlla = tuloraja - vuositulot;
			}
		}

		tulorajanYli = tulot[kuukausi] - tulorajanAlla;
		verot[kuukausi] = tulorajanAlla * veroprosentti / 100.0
			+ tulorajanYli * korkeampiVeroprosentti / 100.0;
		vuositulot += tulot[kuukausi];
	}

	printf("\n%-10s%12s%12s\n", "kuukausi", "tulot", "vero");
	for (int kuukausi = 0; kuukausi < 12; kuukausi++) {
		printf("%-10d%12.2f%12.2f\n", kuukausi + 1, tulot[kuukausi], verot[kuukausi]);
	}

	return 0;
}
