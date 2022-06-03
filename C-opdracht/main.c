#include <stdio.h>
#include <stdlib.h>

#define BMPINPUT "test.bmp"
#define BMPOUTPUT "outputSmoothing.bmp"
#define BMPOUTPUTZW "outputZW.bmp"
#define BMPOUTPUTINV "outpuINV.bmp"

unsigned char header[54] = {0};
signed long hoogte = 0;
signed long breedte = 0;
unsigned char * pixels = NULL;
unsigned char * pixelsnew = NULL;
unsigned char * pixelsnewZw = NULL;
unsigned char * pixelsnewInv = NULL;
long totaalAantalPixels = 0;
signed long Hoeveel_Pixels_onderste_rij = 0;
signed long Hoeveel_Pixels_bovenste_rij = 0;
signed long lengte_newWrite = 0;

void Pixel_Waarde_Weergeven();
void Onderste_Rij_Smoothen();
void Bovenste_Rij_Smoothen();
void Smoothen();
void Kleuren_Inverteren();
void Zwart_Wit();
void Nieuwe_Arrays_schrijven();

int main(int argc, char const *argv[])
{
    FILE * inputBMP = fopen(BMPINPUT, "rb");
    FILE * outputBMP = fopen(BMPOUTPUT, "wb");
    FILE * outputBMPINV = fopen(BMPOUTPUTINV, "wb");
    FILE * outputBMPZW = fopen(BMPOUTPUTZW, "wb");

    if(inputBMP == NULL)
    {
        printf("%s\n", "ERROR: can't open the file");
        return -1;
    }

    fread(header, 1, 54, inputBMP);

    breedte = header[21] << 24 | header[20] << 16 | header[19] << 8 | header[18];
    printf("De breedte van mijn afbeelding is = %ld\n", breedte);
    hoogte = header[25] << 24 | header[24] << 16 | header[23] << 8 | header[22];
    printf("De hoogte van mijn afbeelding is = %ld\n", hoogte);

    totaalAantalPixels = breedte * hoogte;
    pixels = (unsigned char *) malloc(totaalAantalPixels*3);

    pixelsnew = (unsigned char *) malloc(totaalAantalPixels*3); // ervoor zorgen dat de array van nieuwe pixels dezelfde grootte heeft
    pixelsnewZw = (unsigned char *) malloc(totaalAantalPixels*3); // ervoor zorgen dat de array van nieuwe pixels dezelfde grootte heeft
    pixelsnewInv = (unsigned char *) malloc(totaalAantalPixels*3); // ervoor zorgen dat de array van nieuwe pixels dezelfde grootte heeft

    Hoeveel_Pixels_bovenste_rij = (breedte * 3);
    Hoeveel_Pixels_onderste_rij = (breedte * 3);

    if(pixels == NULL)
    {
        printf("ERROR: memory allocation Failed\n");
        return -2;
    }

    fread(pixels, 1, (totaalAantalPixels*3), inputBMP);
    printf("INFO: Heap memory allocated = %ld (bytes)\n", totaalAantalPixels*3);

    Pixel_Waarde_Weergeven();
    Onderste_Rij_Smoothen();
    Bovenste_Rij_Smoothen();
    Smoothen();
    Kleuren_Inverteren();
    Zwart_Wit();

    //-----------1 lijst maken van header en nieuwe pixels-----
    lengte_newWrite = 54 + (totaalAantalPixels * 3);
    unsigned char newWrite[lengte_newWrite];

    for(signed long i =0; i < 55; i++)
    {
        newWrite[i] = header[i];
    }

    for(signed long i =0; i < (totaalAantalPixels * 3); i++)
    {
        newWrite[54 + i] = pixelsnew[i];
    }
    //----------------------------------------------------
    unsigned char newWriteInv[lengte_newWrite];

    for(signed long i =0; i < 55; i++)
    {
        newWriteInv[i] = header[i];
    }

    for(signed long i =0; i < (totaalAantalPixels * 3); i++)
    {
        newWriteInv[54 + i] = pixelsnewInv[i];
    }
    //------------------------------------------------------------
    unsigned char newWriteZw[lengte_newWrite];

    for(signed long i =0; i < 55; i++)
    {
        newWriteZw[i] = header[i];
    }

    for(signed long i =0; i < (totaalAantalPixels * 3); i++)
    {
        newWriteZw[54 + i] = pixelsnewZw[i];
    }
    //------------------------------------------------------------


    //----------------schrijven naar bmp files------------
     fwrite(newWrite, lengte_newWrite, 1, outputBMP);
     fwrite(newWriteInv, lengte_newWrite, 1, outputBMPINV);
     fwrite(newWriteZw, lengte_newWrite, 1, outputBMPZW);
    //---------------------------------------------------


    fclose(inputBMP);
    printf("INFO: File %s CLOSED\n", BMPINPUT);
    fclose(outputBMP);
    printf("INFO: File %s CLOSED\n", BMPOUTPUT);
    fclose(outputBMPZW);
    printf("INFO: File %s CLOSED\n", BMPOUTPUTZW);
    fclose(outputBMPINV);
    printf("INFO: File %s CLOSED\n", BMPOUTPUTINV);


    free(pixels);
    free(pixelsnew);

    printf("INFO: Heap memory Freed = %ld (bytes)\n", totaalAantalPixels*3);
    return 0;
}

void Pixel_Waarde_Weergeven()
{
    signed long imageSize = breedte * hoogte * 3;
    for(signed long i =0; i < imageSize-2; i+=3)
        {
            printf("pixel %ld: B= %d, G=%d, R=%d\n", i, pixels[i], pixels[i+1], pixels[i+2]);
        }
}

void Onderste_Rij_Smoothen()
{
    //Om de onderste rij te smoothen houd ik enkel rekening met de pixels erlangs en erboven. We kunnen niet kijken naar de rij eronder want die is er niet
    for(signed long i = 3; i < Hoeveel_Pixels_onderste_rij; i++)
    {
        pixelsnew[i] = (pixels[i]+pixels[(i+3)]+pixels[(i-3)]+pixels[(i+(breedte * 3))]+pixels[(i+3+(breedte * 3))]+pixels[(i-3)+(breedte * 3)])/6;
    }
}

void Bovenste_Rij_Smoothen()
{
    //Om de bovenste rij te smoothen houd ik enkel rekening met de pixels erlangs en eronder. We kunnen niet kijken naar de rij er boven want die is er niet
    for(signed long i = ((totaalAantalPixels * 3) - Hoeveel_Pixels_bovenste_rij); i < ((totaalAantalPixels * 3)); i++)
    {
        pixelsnew[i] = (pixels[i]+pixels[(i+3)]+pixels[(i-3)]+pixels[(i-(breedte * 3))]+pixels[(i+3-(breedte * 3))]+pixels[(i-3)-(breedte * 3)])/6;
    }
}

void Smoothen()
{
    //Smoothing automatisch
    for(signed long i = (breedte * 3); i < ((totaalAantalPixels * 3) - Hoeveel_Pixels_bovenste_rij); i++)
    {
        if ((i / (breedte * 3) == 1) || ((i-1) / (breedte * 3) == 1) || ((i-2) / (breedte * 3) == 1))
        {
            pixelsnew[i] = (pixels[i]+pixels[(i+3)]+pixels[(i+(breedte * 3))]+pixels[(i+3+(breedte * 3))]+pixels[(i-(breedte * 3))]+pixels[(i+3-(breedte * 3))])/6;
        }

        else if (((i+3) / (breedte * 3) == 1) || ((i+2) / (breedte * 3) == 1) || ((i+1) / (breedte * 3) == 1))
        {
            pixelsnew[i] = (pixels[i]+pixels[(i-3)]+pixels[(i+(breedte * 3))]+pixels[(i-3)+(breedte * 3)]+pixels[(i-(breedte * 3))]+pixels[(i-3)-(breedte * 3)])/6;
        }

        else
        {
            pixelsnew[i] = (pixels[i]+pixels[(i+3)]+pixels[(i-3)]+pixels[(i+(breedte * 3))]+pixels[(i+3+(breedte * 3))]+pixels[(i-3)+(breedte * 3)]+pixels[(i-(breedte * 3))]+pixels[(i+3-(breedte * 3))]+pixels[(i-3)-(breedte * 3)])/9;
        }
    }
}

void Kleuren_Inverteren()
{
    //kleuren inverteren
    for(signed long i = 0; i < (totaalAantalPixels*3); i++)
    {
        pixelsnewInv[i] = 255 - pixels[i];
    }
}

void Zwart_Wit()
{
    //Zwart wit
    int blauw = 0;
    int groen = 0;
    int rood = 0;
    int ZwWaarde = 0;

    for(signed long i = 0; i < (totaalAantalPixels*3); i = i+3)
    {
        blauw = pixels[i];
        groen = pixels[i+1];
        rood = pixels[i+2];

        ZwWaarde = (blauw + groen + rood)/3;

        pixelsnewZw[i] = ZwWaarde;
        pixelsnewZw[i+1] = ZwWaarde;
        pixelsnewZw[i+2] = ZwWaarde;
    }
}


