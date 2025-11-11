// Q2

typedef unsigned char pFixo;

unsigned int reverso(unsigned int valor);
float fixoParaFlutuante(pFixo valor);

int main(int argc, char const *argv[])
{
    reverso(25);
    return 0;
}

unsigned int reverso(unsigned int valor)
{
    unsigned int rev = 0;
    for (int b = 0; b <= 32; b++)
    {
        unsigned char bit = valor & 0xFF;
        if (!bit) rev &= 1;

        rev >>= 1;
        valor >>= 1;
    }
    return reverso;
}



float fixoParaFlutuante(pFixo valor)
{
    float res = 0.0;

    res = valor >> 4;
    float val = 0.5;
    for (int bit = 3; bit >= 0; bit--)
    {
        if((1 << bit) & valor)
        {

        }
    }
    
}






