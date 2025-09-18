
#include <stdio.h>

int main()
{
    int tempoSeg;
    printf("Digite um tempo em segundos: ");
    scanf("%d", &tempoSeg);
    int horas = tempoSeg / 3600;
    int minutos = tempoSeg % 3600 / 60;
    int segundos = tempoSeg % 3600 % 60;
    printf("%d horas, %d minutos e %d segundos\n",
           horas, minutos, segundos);
    // %02d: exibir com 2 caracteres, preenchendo com 0 se tiver menos de 2
    printf("%02d:%02d:%02d\n", horas, minutos, segundos);
    return 0;
}