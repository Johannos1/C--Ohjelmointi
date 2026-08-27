#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int opiskelijoita;

	printf("Kuinka monta opiskelijaa: ");
	while (scanf("%d", &opiskelijoita) != 1 || opiskelijoita <= 0) {
		while (getchar() != '\n') {
		}
		printf("Virheellinen määrä. Anna positiivinen kokonaisluku: ");
	}

	int *arvosanat = malloc(opiskelijoita * sizeof(int));
	if (arvosanat == NULL) {
		printf("Muistin varaaminen epäonnistui.\n");
		return 1;
	}

	for (int i = 0; i < opiskelijoita; i++) {
		arvosanat[i] = -1;
	}

	int opiskelijanumero;
	do {
		printf("Anna opiskelijan numero (1-%d) tai 0 lopettaaksesi: ", opiskelijoita);
		if (scanf("%d", &opiskelijanumero) != 1) {
			while (getchar() != '\n') {
			}
			printf("Virheellinen opiskelijan numero!\n");
			continue;
		}

		if (opiskelijanumero < 0 || opiskelijanumero > opiskelijoita) {
			printf("Virheellinen opiskelijan numero!\n");
			continue;
		}

		if (opiskelijanumero != 0) {
			int arvosana;
			do {
				printf("Anna arvosana (0-5) opiskelijalle %d tai -1 peruuttaaksesi: ", opiskelijanumero);
				if (scanf("%d", &arvosana) != 1) {
					while (getchar() != '\n') {
					}
					printf("Virheellinen arvosana!\n");
					arvosana = -2;
					continue;
				}

				if (arvosana < -1 || arvosana > 5) {
					printf("Virheellinen arvosana!\n");
				}
			} while (arvosana < -1 || arvosana > 5);

			arvosanat[opiskelijanumero - 1] = arvosana;
		}
	} while (opiskelijanumero != 0);

	printf("\n%-10s %s\n", "Opiskelija", "Arvosana");
	for (int i = 0; i < opiskelijoita; i++) {
		if (arvosanat[i] == -1) {
			printf("%-10d N/A\n", i + 1);
		} else {
			printf("%-10d %d\n", i + 1, arvosanat[i]);
		}
	}

	free(arvosanat);
	return 0;
}
