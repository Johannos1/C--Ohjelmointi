#include <stdio.h>

int main(void)
{
	double lipunHinta;
	double taksinHinta;
	double lompsa;
	int valinta;

	printf("anna bussilipun hinta: ");
	scanf("%lf", &lipunHinta);

	printf("anna taksimatkan hinta: ");
	scanf("%lf", &taksinHinta);

	printf("kuinka paljon sinulla on rahaa: ");
	scanf("%lf", &lompsa);

	printf("sulle jää %.2f euroa.\n", lompsa);

	while (lompsa >= lipunHinta || lompsa >= taksinHinta) {
		printf("\nHaluatko mennä mielummin\n");
		printf("1) bussilla (%.2f euroa)\n", lipunHinta);
		printf("2) taksilla (%.2f euroa)\n", taksinHinta);
		printf("valitse: ");
		scanf("%d", &valinta);

		if (valinta == 1) {
			printf("valitsit bussin.\n");
			if (lompsa >= lipunHinta) {
				lompsa -= lipunHinta;
				printf("sinulle jää %.2f euroa.\n", lompsa);
			} else {
				printf("sinulla ei ole tarpeeksi rahaa bussiin.\n");
				printf("sinulle jää %.2f euroa.\n", lompsa);
			}
		} else if (valinta == 2) {
			printf("valitsit taksin.\n");
			if (lompsa >= taksinHinta) {
				lompsa -= taksinHinta;
				printf("sinulle jää %.2f euroa.\n", lompsa);
			} else {
				printf("sinulla ei ole tarpeeksi rahaa taksiin.\n");
				printf("sinulle jää %.2f euroa.\n", lompsa);
			}
		} else {
			printf("Virheellinen valinta.\n");
		}
	}

	printf("Sun täytyy kävellä. terse!\n");
	return 0;
}
