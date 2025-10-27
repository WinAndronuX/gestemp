#include <stdio.h>
#include <gestemp/zone.h>
#include <gestemp/utils.h>

void zoneRegistration() {
    FILE *pF = fopen("zones.bin", "ab");
    fileValidation(&pF);

    Zone registeredZone;

    printf("Ingrese el nombre de la zona: ");
    scanf("%s", registeredZone.zoneName);

    printf("Ingrese el ID de la zona: ");
    scanf("%i", &registeredZone.zoneId);

    printf("Ingrese el volumen de la zona: ");
    scanf("%f", &registeredZone.zoneVolume);

    printf("Ingrese la temperatura inicial de la zona: ");
    scanf("%f", &registeredZone.currentTemperature);

    printf("Ingrese el umbral de la zona: ");
    scanf("%f", &registeredZone.temperatureThreshold);

    printf("Ingrese la cantidad de ventiladores de la zona: ");
    scanf("%i", &registeredZone.fanNum);

    int fanType;

    for (int i = 0; i < registeredZone.fanNum; i++) {
        printf("Ingrese el tipo de ventilador de los siguientes tipos:\n");
        fanPrintInfo(IFan);fanPrintInfo(IFanPlus);fanPrintInfo(IFanPro);
        scanf("%i", &fanType);
        do {
            switch (fanType) {
                case 0:registeredZone.fanType = IFan;break;
                case 1:registeredZone.fanType = IFanPlus;break;
                case 2:registeredZone.fanType = IFanPro;break;
                default:printf("Tipo de ventilador invalido, ingrese uno correcto e intente de nuevo\n");
            }
        }while(fanType < 0 || fanType > 2);
    }

    int nPeople, nCellphones, nComputers;
    printf("Ingrese el numero de personas, telefonos y computadoras en la zona (separados por un espacio): ");
    scanf("%i %i %i", &nPeople, &nCellphones, &nComputers);

    registeredZone.internalHeat = (float)((nPeople * 100) + (nCellphones * 4) + (nComputers * 140));

    fwrite(&registeredZone, sizeof(registeredZone), 1, pF);
    fclose(pF);
    printf("Zona agregada con exito al archivo \"zones.bin\"\n");
}
